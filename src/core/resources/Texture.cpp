#include "Texture.h"
#include <GL/GLEW.h>
#include <iostream>

// TODO: Add STB library for image loading
// #define STB_IMAGE_IMPLEMENTATION
// #include <stb_image.h>

Texture::Texture(const std::string& path)
    : m_FilePath(path), m_RendererID(0)
{
    // TODO: Implement texture loading with STB library
    std::cout << "Texture loading not yet implemented: " << path << std::endl;
    m_RendererID = 0;
    m_Width = 0;
    m_Height = 0;
    m_Channels = 0;
}

Texture::~Texture()
{
    glDeleteTextures(1, &m_RendererID);
}

void Texture::Bind(unsigned int slot) const
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_RendererID);
}

void Texture::Unbind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}