#include "texture.h"
#include "GLSL.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(GLenum textureTarget, const std::string& fileName)
{
	m_textureTarget = textureTarget;
	m_fileName = fileName;
}

Texture::Texture(GLenum textureTarget)
{
	m_textureTarget = textureTarget;
}

bool Texture::load()
{
	stbi_set_flip_vertically_on_load(1);
	int width = 0, height = 0, bpp = 0;
	unsigned char* imageData = stbi_load(m_fileName.c_str(), &width, &height, &bpp, 0);

	if (!imageData)
	{
		printf("Can't load texture '%s'\n", m_fileName.c_str());
	}

	glGenTextures(1, &m_textureObj);
	glBindTexture(m_textureTarget, m_textureObj);
	
	if (m_textureTarget == GL_TEXTURE_2D)
	{
		
		switch (bpp)
		{
		case 1: glTexImage2D(m_textureTarget, 0, GL_RGB, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, imageData);
			break;

		case 3: glTexImage2D(m_textureTarget, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
			break;
		}
	}

	glTexParameterf(m_textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(m_textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(m_textureTarget, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(m_textureTarget, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glBindTexture(m_textureTarget, 0);

	stbi_image_free(imageData);

	return true;
}

void Texture::loadRaw(int width, int height, int BPP, const unsigned char* pImageData)
{
	m_imageWidth = width;
	m_imageHeight = height;
	m_imageBPP = BPP;

	loadInternal(pImageData);
}

void Texture::bind(GLenum textureUnit) 
{
	glActiveTexture(textureUnit);
	glBindTexture(m_textureTarget, m_textureObj);
}

void Texture::bindInternalDSA(GLenum textureUnit)
{
	glBindTextureUnit(textureUnit - GL_TEXTURE0, m_textureObj);
}

void Texture::loadInternal(const void* pImageData)
{
	if (isGLVersionHigher(4, 5))
		printf("higher than 4,5");

	loadInternalDSA(pImageData);
}

void Texture::loadInternalDSA(const void* pImageData)
{
	glCreateTextures(m_textureTarget, 1, &m_textureObj);

	int levels = std::min(5, (int)log2f((float)std::max(m_imageWidth, m_imageHeight)));

	if (m_textureTarget == GL_TEXTURE_2D) 
	{
		switch (m_imageBPP) 
		{
		case 1:
			glTextureStorage2D(m_textureObj, levels, GL_R8, m_imageWidth, m_imageHeight);
			glTextureSubImage2D(m_textureObj, 0, 0, 0, m_imageWidth, m_imageHeight, GL_RED, GL_UNSIGNED_BYTE, pImageData);
			break;

		case 2:
			glTextureStorage2D(m_textureObj, levels, GL_RG8, m_imageWidth, m_imageHeight);
			glTextureSubImage2D(m_textureObj, 0, 0, 0, m_imageWidth, m_imageHeight, GL_RG, GL_UNSIGNED_BYTE, pImageData);
			break;

		case 3:
			glTextureStorage2D(m_textureObj, levels, GL_RGB8, m_imageWidth, m_imageHeight);
			glTextureSubImage2D(m_textureObj, 0, 0, 0, m_imageWidth, m_imageHeight, GL_RGB, GL_UNSIGNED_BYTE, pImageData);
			break;

		case 4:
			glTextureStorage2D(m_textureObj, levels, GL_RGBA8, m_imageWidth, m_imageHeight);
			glTextureSubImage2D(m_textureObj, 0, 0, 0, m_imageWidth, m_imageHeight, GL_RGBA, GL_UNSIGNED_BYTE, pImageData);
			break;

		default:
			break;
		}
	}
	else {
		printf("Support for texture target %x is not implemented\n", m_textureTarget);
		exit(1);
	}

	glTextureParameteri(m_textureObj, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTextureParameteri(m_textureObj, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTextureParameterf(m_textureObj, GL_TEXTURE_BASE_LEVEL, 0);
	glTextureParameteri(m_textureObj, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(m_textureObj, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glGenerateTextureMipmap(m_textureObj);
	
}
