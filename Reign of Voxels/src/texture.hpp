#pragma once
#include <assimp/scene.h>
struct Texture
{
	GLuint id;
	std::string type;
	aiString path;
};

GLuint LoadTexture(const char * filepath);
GLuint LoadTexture(const char *filename, const char * filepath);
GLuint loadDDS(const char * imagepath);
