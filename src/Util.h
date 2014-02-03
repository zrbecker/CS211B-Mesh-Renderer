
#ifndef UTIL_H
#define UTIL_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include <gl/glew.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#define GLUT_PLATFORM_FLAG  GLUT_3_2_CORE_PROFILE
#else
#include <gl/glut.h>
#define GLUT_PLATFORM_FLAG  0
#endif

#include "Math.h"

struct Vertex
{
    gl::Vector3 position;
    gl::Vector2 textureCoord;
    gl::Vector3 normal;
};

struct Entity
{
    gl::Vector3 translation;
    gl::Vector3 rotation;
    gl::Vector3 scale;

    gl::Vector3 diffuseColor;
    gl::Vector3 specularColor;
    float shininess;

    GLuint mesh;
    GLuint texture;
    GLuint objectID;
};

#define OFFSET(Type, member) \
    (GLvoid *) (&((Type *) 0)->member)

#define RADIANS(angle) \
    ((angle) * 3.1415 / 180)

enum
{
    CUBE_MESH,
    SKELETON_MESH,
    TABLE_MESH,
    FLOOR_MESH,
    WALL_MESH,
    CHAIR_MESH,
    SHELVES_MESH,
    CHEST_MESH,
    SPHERE_MESH,
    NUM_VERTEX_OBJECTS
};

enum
{
    SMILE_TEXTURE,
    SKELETON_TEXTURE,
    TABLE_TEXTURE,
    FLOOR_TEXTURE,
    WALL_TEXTURE,
    CHAIR_TEXTURE,
    SHELVES_TEXTURE,
    CHEST_TEXTURE,
    SPHERE_TEXTURE,
    PICK_COLOR_TEXTURE,
    PICK_DEPTH_TEXTURE,
    NUM_TEXTURES
};

enum { PICK_FRAMEBUFFER, NUM_FRAMEBUFFERS };

extern GLuint vao[NUM_VERTEX_OBJECTS];
extern GLuint vao_mode[NUM_VERTEX_OBJECTS];
extern GLuint vao_count[NUM_VERTEX_OBJECTS];
extern GLuint vao_buffer[NUM_VERTEX_OBJECTS];
extern GLuint tex[NUM_TEXTURES];
extern GLuint fbo[NUM_FRAMEBUFFERS]; 

void fatalError(std::string message = "");
void checkError(std::string message = "");
std::string readFile(std::string filename);
GLuint loadProgram(std::string vFile, std::string fFile);
std::vector<Vertex> LoadOBJ(const std::string &filename);
void loadModel(unsigned int name, const std::string &filename);
void loadTexture(unsigned int name, const std::string &filename);

#endif
