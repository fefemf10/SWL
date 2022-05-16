#pragma once
#include <string>
#include <GLEW/glew.h>
#include "stb_image.hpp"

class Texture
{
public:
	Texture();
	Texture(Texture& other);
	Texture(Texture&& other);
	Texture(int width, int height, GLuint internalFormat);
	Texture(const std::string& name);
	~Texture();
	void editTexture(int x, int y, int width, int height, GLuint internalFormat, GLenum type, const char* buffer);
	GLuint getId() const;
	void bind();
	void unbind();
private:
	GLuint id;
	int width, height, bpp;
	GLuint internalFormat;
	unsigned char* data;
};