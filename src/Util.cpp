
#include "Util.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <gl/glew.h>
#include "stb_image.h"

void fatalError(std::string message)
{
    if (message != "")
        std::cerr << "'" << message << "' ";
    std::cerr << "Fatal Error" << std::endl;
#if _WIN32
    system("pause");
#endif
    exit(1);
}

void checkError(std::string message)
{
    GLenum error = glGetError();
    if (error != GL_NO_ERROR)
    {
        if (message != "")
            std::cerr << "'" << message << "' ";
        std::cerr << "GL Error: " << std::hex << error << std::endl;
#if _WIN32
        system("pause");
#endif
        exit(1);
    }
}

std::string readFile(std::string filename)
{
    std::ifstream in;
    std::string str;

    in.open(filename, std::ios::in | std::ios::binary);
    in.seekg(0, std::ios::end);
    str.resize(in.tellg());
    in.seekg(0, std::ios::beg);
    in.read(&str[0], str.size());
    in.close();

    return str;
}

GLuint loadProgram(std::string vFile, std::string fFile)
{
    GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
    std::string vSource = readFile(vFile);
    const char *vSourceP = vSource.c_str();
    glShaderSource(vShader, 1, &vSourceP, 0);
    glCompileShader(vShader);

    GLint success;
    glGetShaderiv(vShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[1024];
        glGetShaderInfoLog(vShader, 1024, 0, infoLog);
        fatalError(infoLog);
    }

    GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
    std::string fSource = readFile(fFile);
    const char *fSourceP = fSource.c_str();
    glShaderSource(fShader, 1, &fSourceP, 0);
    glCompileShader(fShader);

    glGetShaderiv(fShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[1024];
        glGetShaderInfoLog(fShader, 1024, 0, infoLog);
        fatalError(infoLog);
    }

    GLuint program = glCreateProgram();
    glAttachShader(program, vShader);
    glAttachShader(program, fShader);
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        char infoLog[1024];
        glGetProgramInfoLog(program, 1024, 0, infoLog);
        fatalError(infoLog);
    }

    return program;
}

std::vector<Vertex> LoadOBJ(const std::string &filename)
{
    std::vector<Vertex> vertices;

    std::ifstream in(filename, std::ios::in);
    if (in)
    {
        std::vector<gl::Vector3> positions;
        std::vector<gl::Vector2> textureCoords;
        std::vector<gl::Vector3> normals;

        std::string line;
        while (std::getline(in, line))
        {
            std::stringstream lineIn(line);
            std::string token;
            lineIn >> token;
            if (token == "v")
            {
                gl::Vector3 v;
                lineIn >> v[0] >> v[1] >> v[2];
                positions.push_back(v);
            }
            else if (token == "vt")
            {
                gl::Vector2 vt;
                lineIn >> vt[0] >> vt[1];
                textureCoords.push_back(vt);
            }
            else if (token == "vn")
            {
                gl::Vector3 vn;
                lineIn >> vn[0] >> vn[1] >> vn[2];
                normals.push_back(vn);
            }
            else if (token == "f")
            {
                for (int i = 0; i < 3; ++i)
                {
                    unsigned int v, vt, vn;
                    char c;
                    lineIn >> v >> c >> vt >> c >> vn;
                    if (v <= positions.size() && vt <= textureCoords.size() && vn <= normals.size())
                    {
                        Vertex vertex;
                        vertex.position = positions[v - 1];
                        vertex.textureCoord = textureCoords[vt - 1];
                        vertex.normal = normals[vn - 1];
                        vertices.push_back(vertex);
                    }
                }
            }
        }
    }

    if (vertices.size() == 0)
        fatalError("Failed to load model '" + filename + "'");
    std::cout << "Loaded Model '" << filename << "' vertices: " << vertices.size() << std::endl;

    return vertices;
}

void loadModel(unsigned int name, const std::string &filename)
{
    auto vertices = LoadOBJ(filename);
    vao_count[name] = vertices.size();
    vao_mode[name] = GL_TRIANGLES;
    glBindVertexArray(vao[name]);
    glBindBuffer(GL_ARRAY_BUFFER, vao_buffer[name]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vao_count[name], &vertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), OFFSET(Vertex, position));
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), OFFSET(Vertex, textureCoord));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), OFFSET(Vertex, normal));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);
}

void loadTexture(unsigned int name, const std::string &filename)
{
    int width, height, comp;
    stbi_uc *pixels = stbi_load(filename.c_str(), &width, &height, &comp, 4);
    if (pixels == NULL)
        fatalError("Failed to load texture '" + filename + "'");
    std::cout << "Loaded Texture '" << filename << "' width: " << width << " height: " << height << " components: " << comp << std::endl;
    glBindTexture(GL_TEXTURE_2D, tex[name]);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA, width, height);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    stbi_image_free(pixels);
    glBindTexture(GL_TEXTURE_2D, 0);
}
