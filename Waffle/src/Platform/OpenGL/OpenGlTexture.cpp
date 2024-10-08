#include "wfpch.h"
#include "OpenGlTexture.h"

#include "stb_image.h"

#include <glad/glad.h>

namespace Waffle {
	
	/*OpenGlTexture2D::OpenGlTexture2D(uint32_t width, uint32_t height)
		: m_Width(width), m_Height(height)
	{
		WF_PROFILE_FUNCTION();

		m_InternalFormat = GL_RGBA8, m_DataFormat = GL_RGBA;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}*/

	OpenGlTexture2D::OpenGlTexture2D(uint32_t width, uint32_t height, TextureFilter filter)
		: m_Width(width), m_Height(height)
	{
		WF_PROFILE_FUNCTION();

		m_InternalFormat = GL_RGBA8, m_DataFormat = GL_RGBA;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

		GLenum minFilter = (filter == TextureFilter::Nearest) ? GL_NEAREST : GL_LINEAR;
		GLenum magFilter = (filter == TextureFilter::Nearest) ? GL_NEAREST : GL_LINEAR;

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, minFilter);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, magFilter);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	OpenGlTexture2D::OpenGlTexture2D(const std::string& path, TextureFilter filter)
		: m_Path(path)
	{
		WF_PROFILE_FUNCTION();

		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = nullptr;
		{
			WF_PROFILE_SCOPE("stbi_load - OpenGlTexture2D::OpenGlTexture2D(const std::string& path)");
			data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		}
		WF_CORE_ASSERT(data, "Failed to load image!");

		m_Width = width;
		m_Height = height;

		GLenum internalFormat = 0, dataFormat = 0;
		if (channels == 4)
		{
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}
		else if (channels == 3)
		{
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}

		m_InternalFormat = internalFormat;
		m_DataFormat = dataFormat;

		WF_CORE_ASSERT(internalFormat & dataFormat, "Format not supported");

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

		GLenum minFilter = (filter == TextureFilter::Nearest) ? GL_NEAREST : GL_LINEAR;
		GLenum magFilter = (filter == TextureFilter::Nearest) ? GL_NEAREST : GL_LINEAR;

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, minFilter);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, magFilter);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);


		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}

	OpenGlTexture2D::~OpenGlTexture2D()
	{
		WF_PROFILE_FUNCTION();

		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGlTexture2D::SetData(void* data, uint32_t size)
	{
		WF_PROFILE_FUNCTION();

		uint32_t bpc = m_DataFormat == GL_RGBA ? 4 : 3;
		WF_CORE_ASSERT(size == m_Width * m_Height * bpc, "Data must be entire texture!");
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
	}

	void OpenGlTexture2D::Bind(uint32_t slot) const
	{
		WF_PROFILE_FUNCTION();

		glBindTextureUnit(slot, m_RendererID);
	}
}