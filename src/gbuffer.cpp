
#include "gbuffer.h"
#include <iostream>

GBuffer::GBuffer()
{
    m_fbo = 0;
    for (GLuint i = 0; i < GBUFFER_NUM_TEXTURES; ++i)
        m_textures[i] = 0;
    m_depthTexture = 0;
}

GBuffer::~GBuffer()
{

}

bool GBuffer::Init(unsigned int WindowWidth, unsigned int WindowHeight)
{
    if (m_fbo == 0)
    {
        glGenFramebuffers(1, &m_fbo);
        glGenTextures(GBUFFER_NUM_TEXTURES, m_textures);
        glGenTextures(1, &m_shininessTexture);
        glGenTextures(1, &m_depthTexture);
    }

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);

    for (GLuint i = 0; i < GBUFFER_NUM_TEXTURES; ++i)
    {
        glBindTexture(GL_TEXTURE_2D, m_textures[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, WindowWidth, WindowHeight, 0, GL_RGB, GL_FLOAT, NULL);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_textures[i], 0);
    }

    glBindTexture(GL_TEXTURE_2D, m_shininessTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, WindowWidth, WindowHeight, 0, GL_RED, GL_FLOAT, NULL);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + GBUFFER_NUM_TEXTURES, GL_TEXTURE_2D, m_shininessTexture, 0);

    glBindTexture(GL_TEXTURE_2D, m_depthTexture); 
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, WindowWidth, WindowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthTexture, 0);

    GLenum DrawBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT5, GL_COLOR_ATTACHMENT6 };
    glDrawBuffers(sizeof(DrawBuffers) / sizeof(DrawBuffers[0]), DrawBuffers);

    GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (Status != GL_FRAMEBUFFER_COMPLETE)
        fatalError("Failed to set up gbuffer.\n");

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

    return true;
}

void GBuffer::BindForWriting()
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);
}

void GBuffer::UnbindForWriting()
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

void GBuffer::BindForReading()
{
    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fbo);
}

void GBuffer::UnbindForReading()
{
    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
}

void GBuffer::SetReadBuffer(GBUFFER_TEXTURE_TYPE TextureType)
{
    glReadBuffer(GL_COLOR_ATTACHMENT0 + TextureType);
}

void GBuffer::BindForRender()
{
    for (GLuint i = 0; i < GBUFFER_NUM_TEXTURES; ++i)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, m_textures[i]);
    }
    glActiveTexture(GL_TEXTURE0 + GBUFFER_NUM_TEXTURES);
    glBindTexture(GL_TEXTURE_2D, m_shininessTexture);
}

void GBuffer::UnbindForRender()
{
    for (GLuint i = 0; i < GBUFFER_NUM_TEXTURES; ++i)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    glActiveTexture(GL_TEXTURE0 + GBUFFER_NUM_TEXTURES);
    glBindTexture(GL_TEXTURE_2D, 0);
}

