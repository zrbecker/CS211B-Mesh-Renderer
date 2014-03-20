
#include "Util.h"
#include "gbuffer.h"

#include <cmath>
#include <iostream>

int screenWidth = 1024;
int screenHeight = 768;

GLuint vao[NUM_VERTEX_OBJECTS];
GLuint vao_mode[NUM_VERTEX_OBJECTS];
GLuint vao_count[NUM_VERTEX_OBJECTS];
GLuint vao_buffer[NUM_VERTEX_OBJECTS];
GLuint tex[NUM_TEXTURES];
GLuint fbo[NUM_FRAMEBUFFERS];

GLuint drawProgram;
GLuint pickProgram;
GLuint geometryProgram;
GLuint renderPassProgram;

GBuffer gbuffer;

gl::Matrix4Stack modelview;
gl::Matrix4Stack projection;

std::vector<Entity> entities;

const unsigned int NUM_LIGHTS = 4;

gl::Vector3 ambientLight(0.2, 0.2, 0.2);

gl::Vector4 globalLightPositions[NUM_LIGHTS]
{
    gl::Vector4(1, 1, 1, 0),
    gl::Vector4(-4.5, 4.5, 0, 1),
    gl::Vector4(0, 4.5, -4.5, 1),
    gl::Vector4(-4.5, 4.5, -4.5, 1)
};
gl::Vector4 lightPositions[NUM_LIGHTS];

gl::Vector3 lightColors[NUM_LIGHTS]
{
    gl::Vector3(0.5, 0.5, 1.0),
    gl::Vector3(0.5, 0.1, 0.1),
    gl::Vector3(0.1, 0.1, 0.5),
    gl::Vector3(0.1, 0.1, 0.5)
};

bool mouseRotate = false;
bool mouseZoom = false;
bool mouseTranslate = false;

int lastRX;
int lastRY;
int lastZY;
int lastTX;
int lastTY;

float xRot;
float yRot;
float zoom;

gl::Vector3 offset;
gl::Vector3 eye;
gl::Vector3 center;
gl::Vector3 up;

bool hidecursor = true;

unsigned int selected = 0;
unsigned int selectedIndex = 0;

enum Mode
{
    TRANSLATE,
    ROTATE,
    SCALE
};

Mode editMode = TRANSLATE;

Entity CreateEntity(GLuint mesh, GLuint texture, GLuint objectID)
{
    Entity entity;

    entity.translation = gl::Vector3(0, 0, 0);
    entity.rotation = gl::Vector3(0, 0, 0);
    entity.scale = gl::Vector3(1, 1, 1);

    entity.diffuseColor = gl::Vector3(1, 1, 1);
    entity.specularColor = gl::Vector3(0, 0, 0);
    entity.shininess = 0;

    entity.mesh = mesh;
    entity.texture = texture;
    entity.objectID = objectID;
    entity.cull = GL_BACK;

    return entity;
}

void DrawEntity(const Entity &entity, GLuint program)
{
    modelview.push();
    modelview.translate(entity.translation);
    modelview.rotate(entity.rotation[1], 0, 1, 0);
    modelview.rotate(entity.rotation[2], 0, 0, 1);
    modelview.rotate(entity.rotation[0], 1, 0, 0);
    modelview.scale(entity.scale);

    if (entity.cull == GL_NONE)
        glDisable(GL_CULL_FACE);
    else
    {
        glEnable(GL_CULL_FACE);
        glCullFace(entity.cull);
    }

    GLint locDiffuseColor = glGetUniformLocation(program, "diffuseColor");
    if (locDiffuseColor >= 0)
        glUniform3fv(locDiffuseColor, 1, &entity.diffuseColor[0]);

    GLint locSpecularColor = glGetUniformLocation(program, "specularColor");
    if (locSpecularColor >= 0)
        glUniform3fv(locSpecularColor, 1, &entity.specularColor[0]);

    GLint locShininess = glGetUniformLocation(program, "shininess");
    if (locShininess >= 0)
        glUniform1f(locShininess, entity.shininess);

    GLint locAmbientLight = glGetUniformLocation(program, "ambientLight");
    if (locAmbientLight >= 0)
        glUniform3fv(locAmbientLight, 1, &ambientLight[0]);

    GLint locNumLights = glGetUniformLocation(program, "numLights");
    if (locNumLights >= 0)
        glUniform1ui(locNumLights, NUM_LIGHTS);

    GLint locLightPositions = glGetUniformLocation(program, "lightPositions");
    if (locLightPositions >= 0)
        glUniform4fv(locLightPositions, NUM_LIGHTS, &lightPositions[0][0]);

    GLint locLightColors = glGetUniformLocation(program, "lightColors");
    if (locLightColors >= 0)
        glUniform3fv(locLightColors, NUM_LIGHTS, &lightColors[0][0]);

    GLint locSelectedID = glGetUniformLocation(program, "selectedID");
    if (locSelectedID >= 0)
        glUniform1ui(locSelectedID, selected);

    glUniform1ui(glGetUniformLocation(program, "objectID"), entity.objectID);
    glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE, &projection.top()[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(program, "modelview"), 1, GL_FALSE, &modelview.top()[0][0]);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex[entity.texture]);
    glBindVertexArray(vao[entity.mesh]);
    glDrawArrays(vao_mode[entity.mesh], 0, vao_count[entity.mesh]);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);

    modelview.pop();

    checkError("End of Draw Entity");
}

void resetCamera()
{
    xRot = 45;
    yRot = 20;
    zoom = 5;

    offset = gl::Vector3(0, 1, 0);
    eye = gl::Vector3(zoom * sin(RADIANS(xRot)) * cos(RADIANS(yRot)),
                      zoom * sin(RADIANS(yRot)),
                      zoom * cos(RADIANS(xRot)) * cos(RADIANS(yRot)));
    center = gl::Vector3(0, 0, 0);
    up = gl::Vector3(0, 1, 0);
}

void init()
{
    resetCamera();

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // Program Data
    drawProgram = loadProgram("resources/shaders/draw.vert", "resources/shaders/draw.frag");
    pickProgram = loadProgram("resources/shaders/pick.vert", "resources/shaders/pick.frag");
    geometryProgram = loadProgram("resources/shaders/geometry_pass.vert", "resources/shaders/geometry_pass.frag");
    renderPassProgram = loadProgram("resources/shaders/render_pass.vert", "resources/shaders/render_pass.frag");

    // Generate OpenGL objects
    glGenVertexArrays(NUM_VERTEX_OBJECTS, vao);
    glGenBuffers(NUM_VERTEX_OBJECTS, vao_buffer);
    glGenTextures(NUM_TEXTURES, tex);
    glGenFramebuffers(NUM_FRAMEBUFFERS, fbo);

    // Load models
    loadModel(CUBE_MESH, "resources/models/cube.obj");
    loadModel(SKELETON_MESH, "resources/models/skeleton.obj");
    loadModel(TABLE_MESH, "resources/models/table.obj");
    loadModel(FLOOR_MESH, "resources/models/floor.obj");
    loadModel(WALL_MESH, "resources/models/wall.obj");
    loadModel(CHAIR_MESH, "resources/models/chair.obj");
    loadModel(SHELVES_MESH, "resources/models/shelves.obj");
    loadModel(CHEST_MESH, "resources/models/chest.obj");
    loadModel(SPHERE_MESH, "resources/models/sphere.obj");

    // Load textures
    loadTexture(SMILE_TEXTURE, "resources/textures/smile.png");
    loadTexture(SKELETON_TEXTURE, "resources/textures/skeleton.png");
    loadTexture(TABLE_TEXTURE, "resources/textures/table.png");
    loadTexture(FLOOR_TEXTURE, "resources/textures/floor.png");
    loadTexture(WALL_TEXTURE, "resources/textures/wall.png");
    loadTexture(CHAIR_TEXTURE, "resources/textures/chair.png");
    loadTexture(SHELVES_TEXTURE, "resources/textures/shelves.png");
    loadTexture(CHEST_TEXTURE, "resources/textures/chest.png");
    loadTexture(SPHERE_TEXTURE, "resources/textures/sphere.png");

    gbuffer.Init(screenWidth, screenHeight);

    // Load entities
    Entity floor = CreateEntity(FLOOR_MESH, FLOOR_TEXTURE, 2);
    floor.rotation = gl::Vector3(90, 0, 0);
    entities.push_back(floor);

    Entity wall1 = CreateEntity(WALL_MESH, WALL_TEXTURE, 3);
    wall1.translation = gl::Vector3(0, 0, -5);
    entities.push_back(wall1);

    Entity wall2 = CreateEntity(WALL_MESH, WALL_TEXTURE, 4);
    wall2.translation = gl::Vector3(-5, 0, 0);
    wall2.rotation = gl::Vector3(0, 90, 0);
    entities.push_back(wall2);

    Entity table = CreateEntity(TABLE_MESH, TABLE_TEXTURE, 5);
    table.translation = gl::Vector3(0, 0.368736, 0);
    table.specularColor = gl::Vector3(0.9, 0.9, 0.9);
    table.shininess = 30;
    entities.push_back(table);

    Entity chair1 = CreateEntity(CHAIR_MESH, CHAIR_TEXTURE, 7);
    chair1.translation = gl::Vector3(0, 0.555590, -1);
    chair1.specularColor = gl::Vector3(0.9, 0.9, 0.9);
    chair1.shininess = 30;
    chair1.cull = GL_NONE;
    entities.push_back(chair1);

    Entity chair2 = CreateEntity(CHAIR_MESH, CHAIR_TEXTURE, 8);
    chair2.translation = gl::Vector3(0, 0.555590, 1);
    chair2.rotation = gl::Vector3(0, 180, 0);
    chair2.specularColor = gl::Vector3(0.9, 0.9, 0.9);
    chair2.shininess = 30;
    chair2.cull = GL_NONE;
    entities.push_back(chair2);

    Entity skeleton1 = CreateEntity(SKELETON_MESH, SKELETON_TEXTURE, 9);
    skeleton1.translation = gl::Vector3(1.5, 0, 0);
    skeleton1.scale = gl::Vector3(0.2, 0.2, 0.2);
    skeleton1.rotation = gl::Vector3(0, -90, 0);
    entities.push_back(skeleton1);

    Entity skeleton2 = CreateEntity(SKELETON_MESH, SKELETON_TEXTURE, 10);
    skeleton2.translation = gl::Vector3(-1.5, 0, 0);
    skeleton2.scale = gl::Vector3(0.2, 0.2, 0.2);
    skeleton2.rotation = gl::Vector3(0, 90, 0);
    entities.push_back(skeleton2);

    Entity shelves1 = CreateEntity(SHELVES_MESH, SHELVES_TEXTURE, 11);
    shelves1.translation = gl::Vector3(-4.5, 1.09167975, 2);
    shelves1.scale = gl::Vector3(0.75, 0.75, 0.75);
    shelves1.rotation = gl::Vector3(0, -90, 0);
    shelves1.specularColor = gl::Vector3(0.9, 0.9, 0.9);
    shelves1.shininess = 30;
    shelves1.cull = GL_NONE;
    entities.push_back(shelves1);

    Entity shelves2 = CreateEntity(SHELVES_MESH, SHELVES_TEXTURE, 12);
    shelves2.translation = gl::Vector3(-4.5, 1.09167975, -2);
    shelves2.scale = gl::Vector3(0.75, 0.75, 0.75);
    shelves2.rotation = gl::Vector3(0, -90, 0);
    shelves2.specularColor = gl::Vector3(0.9, 0.9, 0.9);
    shelves2.shininess = 30;
    shelves2.cull = GL_NONE;
    entities.push_back(shelves2);

    Entity shelves3 = CreateEntity(SHELVES_MESH, SHELVES_TEXTURE, 13);
    shelves3.translation = gl::Vector3(-2, 1.09167975, -4.5);
    shelves3.scale = gl::Vector3(0.75, 0.75, 0.75);
    shelves3.rotation = gl::Vector3(0, 180, 0);
    shelves3.specularColor = gl::Vector3(0.9, 0.9, 0.9);
    shelves3.shininess = 30;
    shelves3.cull = GL_NONE;
    entities.push_back(shelves3);

    Entity shelves4 = CreateEntity(SHELVES_MESH, SHELVES_TEXTURE, 14);
    shelves4.translation = gl::Vector3(2, 1.09167975, -4.5);
    shelves4.scale = gl::Vector3(0.75, 0.75, 0.75);
    shelves4.rotation = gl::Vector3(0, 180, 0);
    shelves4.specularColor = gl::Vector3(0.9, 0.9, 0.9);
    shelves4.shininess = 30;
    shelves4.cull = GL_NONE;
    entities.push_back(shelves4);

    Entity chest = CreateEntity(CHEST_MESH, CHEST_TEXTURE, 15);
    chest.translation = gl::Vector3(0, 0.271628, 2.5);
    chest.rotation = gl::Vector3(0, 180, 0);
    chest.specularColor = gl::Vector3(0.9, 0.9, 0.9);
    chest.shininess = 30;
    entities.push_back(chest);

    Entity sphere = CreateEntity(SPHERE_MESH, SPHERE_TEXTURE, 6);
    sphere.translation = gl::Vector3(0, 1.3, 0);
    sphere.scale = gl::Vector3(0.45, 0.45, 0.45);
    sphere.specularColor = gl::Vector3(0.9, 0.9, 0.9);
    sphere.shininess = 50;
    entities.push_back(sphere);

    checkError("End of Init");
}

void reshape(int w, int h)
{
    glViewport(0, 0, w, h);
    screenWidth = w;
    screenHeight = h;

    // Pick Framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, fbo[PICK_FRAMEBUFFER]);

    glBindTexture(GL_TEXTURE_2D, tex[PICK_COLOR_TEXTURE]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32UI, screenWidth, screenHeight, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, NULL);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, tex[PICK_COLOR_TEXTURE], 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glBindTexture(GL_TEXTURE_2D, tex[PICK_DEPTH_TEXTURE]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, screenWidth, screenHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, tex[PICK_DEPTH_TEXTURE], 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (Status != GL_FRAMEBUFFER_COMPLETE)
        fatalError("Framebuffer Status Error");

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    gbuffer.Init(screenWidth, screenHeight);
}

void draw(GLuint program)
{
    glUseProgram(program);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    projection.loadIdentity();
    projection.prespective(60, float(screenWidth) / screenHeight, 0.01, 100);

    modelview.loadIdentity();
    modelview.lookAt(offset + eye, offset + center, up);

    for (int i = 0; i < NUM_LIGHTS; ++i)
        lightPositions[i] = modelview.top() * globalLightPositions[i];

    if (!hidecursor)
    {
        Entity cursor = CreateEntity(CUBE_MESH, SMILE_TEXTURE, 0xFFFFFF);
        cursor.translation = offset;
        cursor.scale = gl::Vector3(0.05, 0.05, 0.05);
        DrawEntity(cursor, program);
    }

    for (Entity entity : entities)
        DrawEntity(entity, program);
}

void pick()
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo[PICK_FRAMEBUFFER]);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    draw(pickProgram);

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

    checkError("End of Pick");
}

void drawGeometryBuffers()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    gbuffer.BindForReading();

    GLsizei HalfWidth = (GLsizei) (screenWidth / 2.0f);
    GLsizei HalfHeight = (GLsizei) (screenHeight / 2.0f);

    gbuffer.SetReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_POSITION);
    glBlitFramebuffer(0, 0, screenWidth, screenHeight,
                      0, 0, HalfWidth, HalfHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);

    gbuffer.SetReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_DIFFUSE);
    glBlitFramebuffer(0, 0, screenWidth, screenHeight,
                      0, HalfHeight, HalfWidth, screenHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);

    gbuffer.SetReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_NORMAL);
    glBlitFramebuffer(0, 0, screenWidth, screenHeight,
                      HalfWidth, HalfHeight, screenWidth, screenHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);

    gbuffer.SetReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_SPECULAR_COLOR);
    glBlitFramebuffer(0, 0, screenWidth, screenHeight,
                      HalfWidth, 0, screenWidth, HalfHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);

    gbuffer.UnbindForReading();
}

void display1()
{
    pick();

    glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
    draw(drawProgram);

    glFlush();
    glutSwapBuffers();

    checkError("End of Display");
}

void display2()
{
    glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
    gbuffer.BindForWriting();
    draw(geometryProgram);
    gbuffer.UnbindForWriting();

    drawGeometryBuffers();

    glFlush();
    glutSwapBuffers();

    checkError("End of Display");
}

void display3()
{
    pick();

    gbuffer.BindForWriting();
    draw(geometryProgram);
    gbuffer.UnbindForWriting();

    gbuffer.BindForRender();
    glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(renderPassProgram);

    GLint locScreenWidth = glGetUniformLocation(renderPassProgram, "screenWidth");
    if (locScreenWidth >= 0)
        glUniform1f(locScreenWidth, (float) screenWidth);

    GLint locScreenHeight = glGetUniformLocation(renderPassProgram, "screenHeight");
    if (locScreenHeight >= 0)
        glUniform1f(locScreenHeight, (float) screenHeight);

    GLint locTexPosition = glGetUniformLocation(renderPassProgram, "texPosition");
    if (locTexPosition >= 0)
        glUniform1i(locTexPosition, 0);

    GLint locTexDiffuse = glGetUniformLocation(renderPassProgram, "texDiffuse");
    if (locTexDiffuse >= 0)
        glUniform1i(locTexDiffuse, 1);

    GLint locTexNormal = glGetUniformLocation(renderPassProgram, "texNormal");
    if (locTexNormal >= 0)
        glUniform1i(locTexNormal, 2);

    GLint locTexDiffuseColor = glGetUniformLocation(renderPassProgram, "texDiffuseColor");
    if (locTexDiffuseColor >= 0)
        glUniform1i(locTexDiffuseColor, 4);

    GLint locTexSpecularColor = glGetUniformLocation(renderPassProgram, "texSpecularColor");
    if (locTexSpecularColor >= 0)
        glUniform1i(locTexSpecularColor, 5);

    GLint locTexShininess = glGetUniformLocation(renderPassProgram, "texShininess");
    if (locTexShininess >= 0)
        glUniform1i(locTexShininess, 6);

    GLint locAmbientLight = glGetUniformLocation(renderPassProgram, "ambientLight");
    if (locAmbientLight >= 0)
        glUniform3fv(locAmbientLight, 1, &ambientLight[0]);

    GLint locNumLights = glGetUniformLocation(renderPassProgram, "numLights");
    if (locNumLights >= 0)
        glUniform1ui(locNumLights, NUM_LIGHTS);

    GLint locLightPositions = glGetUniformLocation(renderPassProgram, "lightPositions");
    if (locLightPositions >= 0)
        glUniform4fv(locLightPositions, NUM_LIGHTS, &lightPositions[0][0]);

    GLint locLightColors = glGetUniformLocation(renderPassProgram, "lightColors");
    if (locLightColors >= 0)
        glUniform3fv(locLightColors, NUM_LIGHTS, &lightColors[0][0]);

    GLint locSelectedID = glGetUniformLocation(renderPassProgram, "selectedID");
    if (locSelectedID >= 0)
        glUniform1ui(locSelectedID, selected);
    
    GLuint qVAO, qVBO;
    glGenVertexArrays(1, &qVAO);
    glBindVertexArray(qVAO);

    glGenBuffers(1, &qVBO);
    glBindBuffer(GL_ARRAY_BUFFER, qVBO);

    float vertex[] = {
        -1, -1, 0,
         1, -1, 0,
         1,  1, 0,
        -1,  1, 0
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    glDeleteBuffers(1, &qVBO);
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &qVAO);

    glFlush();
    glutSwapBuffers();

    checkError("End of Display");
}

void (*currentDisplay)() = display1;
void display()
{
    currentDisplay();
}

void mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        mouseRotate = true;
        lastRX = x;
        lastRY = y;
    }
    else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
    {
        mouseRotate = false;
    }
    else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
    {
        mouseZoom = true;
        lastZY = y;
    }
    else if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP)
    {
        mouseZoom = false;
    }
    else if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN)
    {
        mouseTranslate = true;
        lastTX = x;
        lastTY = y;
    }
    else if (button == GLUT_MIDDLE_BUTTON && state == GLUT_UP)
    {
        mouseTranslate = false;
    }

    checkError("End of Mouse");
}

void mouseMovement(int x, int y)
{
    if (mouseRotate)
    {
        const float speed = 4.0f;

        float dx = (x - lastRX == 0) ? 0 : abs(x - lastRX) / (x - lastRX);
        float dy = (y - lastRY == 0) ? 0 : abs(y - lastRY) / (y - lastRY);

        lastRX = x;
        lastRY = y;

        xRot += -speed * dx;
        yRot += speed * dy;

        if (xRot > 360)
            xRot -= 360;
        else if (xRot < 0)
            xRot += 360;

        if (yRot > 90)
            yRot = 90;
        else if (yRot < -90)
            yRot = -90;

        eye = gl::Vector3(zoom * sin(RADIANS(xRot)) * cos(RADIANS(yRot)),
                          zoom * sin(RADIANS(yRot)),
                          zoom * cos(RADIANS(xRot)) * cos(RADIANS(yRot)));
    }

    if (mouseZoom)
    {
        const float speed = 0.2f;

        float dy = (y - lastZY == 0) ? 0 : abs(y - lastZY) / (y - lastZY);
        lastZY = y;

        zoom += speed * dy;

        eye = gl::Vector3(zoom * sin(RADIANS(xRot)) * cos(RADIANS(yRot)),
                          zoom * sin(RADIANS(yRot)),
                          zoom * cos(RADIANS(xRot)) * cos(RADIANS(yRot)));
    }

    if (mouseTranslate)
    {
        const float speed = 0.2f;

        float dx = (x - lastTX == 0) ? 0 : abs(x - lastTX) / (x - lastTX);
        float dy = (y - lastTY == 0) ? 0 : abs(y - lastTY) / (y - lastTY);

        lastTX = x;
        lastTY = y;

        offset[0] += speed * (dx * cos(RADIANS(xRot)) + dy * sin(RADIANS(xRot)));
        offset[2] += speed * (dx * -sin(RADIANS(xRot)) + dy * cos(RADIANS(xRot)));
    }
}

void key(unsigned char key, int x, int y)
{
    if (key == '1')
    {
        currentDisplay = display1;
    }
    else if (key == '2')
    {
        currentDisplay = display2;
    }
    else if (key == '3')
    {
        currentDisplay = display3;
    }
    else if (key == 'p')
    {
        unsigned int id;
        glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo[PICK_FRAMEBUFFER]);
        glReadPixels(x, screenHeight - y, 1, 1, GL_RED_INTEGER, GL_UNSIGNED_INT, &id);
        glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
        selected = id;
        for (unsigned int i = 0; i < entities.size(); ++i)
        {
            if (entities[i].objectID == selected)
            {
                selectedIndex = i;
                break;
            }
        }
    }
    else if (key == 'o')
    {
        selected = 0;
    }
    else if (key == 'r')
    {
        resetCamera();
    }
    else if (key == '=')
    {
        offset[1] += 0.1f;
    }
    else if (key == '-')
    {
        offset[1] -= 0.1f;
    }
    else if (key == 'h')
    {
        hidecursor = !hidecursor;
    }
    else if (selected != 0)
    {
        if (key == 'w')
        {
            switch (editMode)
            {
            case TRANSLATE: entities[selectedIndex].translation[2] += 0.1f; break;
            case ROTATE: entities[selectedIndex].rotation[2] += 2.0f; break;
            case SCALE: entities[selectedIndex].scale[2] += 0.1f; break;
            }
        }
        else if (key == 's')
        {
            switch (editMode)
            {
            case TRANSLATE: entities[selectedIndex].translation[2] -= 0.1f; break;
            case ROTATE: entities[selectedIndex].rotation[2] -= 2.0f; break;
            case SCALE: entities[selectedIndex].scale[2] -= 0.1f; break;
            }
        }
        else if (key == 'a')
        {
            switch (editMode)
            {
            case TRANSLATE: entities[selectedIndex].translation[0] += 0.1f; break;
            case ROTATE: entities[selectedIndex].rotation[0] += 2.0f; break;
            case SCALE: entities[selectedIndex].scale[0] += 0.1f; break;
            }
        }
        else if (key == 'd')
        {
            switch (editMode)
            {
            case TRANSLATE: entities[selectedIndex].translation[0] -= 0.1f; break;
            case ROTATE: entities[selectedIndex].rotation[0] -= 2.0f; break;
            case SCALE: entities[selectedIndex].scale[0] -= 0.1f; break;
            }
        }
        else if (key == 'q')
        {
            switch (editMode)
            {
            case TRANSLATE: entities[selectedIndex].translation[1] += 0.1f; break;
            case ROTATE: entities[selectedIndex].rotation[1] += 2.0f; break;
            case SCALE: entities[selectedIndex].scale[1] += 0.1f; break;
            }
        }
        else if (key == 'e')
        {
            switch (editMode)
            {
            case TRANSLATE: entities[selectedIndex].translation[1] -= 0.1f; break;
            case ROTATE: entities[selectedIndex].rotation[1] -= 2.0f; break;
            case SCALE: entities[selectedIndex].scale[1] -= 0.1f; break;
            }
        }
        else if (key == 'z')
        {
            std::cout << "Translate Edit Mode" << std::endl;
            editMode = TRANSLATE;
        }
        else if (key == 'x')
        {
            std::cout << "Rotate Edit Mode" << std::endl;
            editMode = ROTATE;
        }
        else if (key == 'c')
        {
            std::cout << "Scale Edit Mode" << std::endl;
            editMode = SCALE;
        }
    }
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_PLATFORM_FLAG);
    glutInitWindowSize(screenWidth, screenHeight);
    glutCreateWindow("CS211B - Project 1");

    glutDisplayFunc(display);
    glutIdleFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutMotionFunc(mouseMovement);
    glutKeyboardFunc(key);

#ifdef __APPLE__
    glewExperimental = GL_TRUE;
#endif
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW." << std::endl;
        return 1;
    }
#ifdef __APPLE__
    glGetError(); // glewInit() causes error on Mac. This clears it
#endif

    if (!glewIsSupported("GL_VERSION_3_3"))
        fatalError(std::string("OpenGL Version String: '") + (char *)glGetString(GL_VERSION) + "' Open GL 3.3 is not supported.");
    else
        std::cout << "Open GL 3.3 is supported" << std::endl;

    init();
    reshape(screenWidth, screenHeight);

    glutMainLoop();

    return 0;
}
