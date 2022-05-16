#include "Texture.hpp"

Texture::Texture()
{
}

Texture::Texture(Texture& other)
{
	this->bpp = other.bpp;
	this->width = other.width;
	this->height = other.height;
	this->id = other.id;
	this->internalFormat = other.internalFormat;
	other.bpp = other.width = other.height = other.internalFormat = other.id = 0;
}

Texture::Texture(Texture&& other)
{
	this->bpp = other.bpp;
	this->width = other.width;
	this->height = other.height;
	this->id = other.id;
	this->internalFormat = other.internalFormat;
	other.bpp = other.width = other.height = other.internalFormat = other.id = 0;
}

Texture::Texture(int width, int height, GLuint internalFormat) : width(width), height(height), internalFormat(internalFormat)
{
	glCreateTextures(GL_TEXTURE_2D, 1, &id);
	/*switch (internalFormat)
	{
	case GL_RED:
	case GL_GREEN:
	case GL_BLUE:
		bpp = 1;
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		break;
	case GL_RG:
		bpp = 2;
		glPixelStorei(GL_UNPACK_ALIGNMENT, 2);
		break;
	case GL_RGB:
		bpp = 3;
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
		break;
	case GL_RGBA:
	case GL_R32F:
		bpp = 4;
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
		break;
	}*/
	glTextureStorage2D(id, 1, internalFormat, width, height);
	glTextureParameteri(id, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(id, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glGenerateTextureMipmap(id);
}

Texture::Texture(const std::string& name)
{
	stbi_set_flip_vertically_on_load(true);
	data = stbi_load(name.c_str(), &width, &height, &bpp, 0);
	internalFormat = GL_RGBA8;
	glCreateTextures(GL_TEXTURE_2D, 1, &id);
	glTextureStorage2D(id, 1, GL_RGBA8, width, height);
	glTextureSubImage2D(id, 0, 0, 0, width, height, bpp == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, static_cast<const void*>(data));
	glTextureParameteri(id, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(id, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glGenerateTextureMipmap(id);
	stbi_image_free(data);
}

Texture::~Texture()
{
	glDeleteTextures(1, &id);
}

void Texture::editTexture(int x, int y, int width, int height, GLuint internalFormat, GLenum type, const char* buffer)
{
	if ((x + width > this->width) || (y + height > this->height))
		return;
	glTextureSubImage2D(id, 0, x, y, width, height, internalFormat, type, static_cast<const void*>(buffer));
}

GLuint Texture::getId() const
{
	return id;
}

void Texture::bind()
{
	glBindTextureUnit(0, id);
}

void Texture::unbind()
{
	glBindTextureUnit(0, 0);
}