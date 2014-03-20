
#ifndef GBUFFER_H
#define GBUFFER_H

#include "Util.h"

class GBuffer
{
public:
    enum GBUFFER_TEXTURE_TYPE
    {
        GBUFFER_TEXTURE_TYPE_POSITION,
        GBUFFER_TEXTURE_TYPE_DIFFUSE,
        GBUFFER_TEXTURE_TYPE_NORMAL,
        GBUFFER_TEXTURE_TYPE_TEXCOORD,
        GBUFFER_TEXTURE_TYPE_DIFFUSE_COLOR,
        GBUFFER_TEXTURE_TYPE_SPECULAR_COLOR,
        GBUFFER_NUM_TEXTURES
    };

    GBuffer();
    virtual ~GBuffer();

    bool Init(unsigned int WindowWidth, unsigned int WindowHeight);
    void BindForWriting();
    void UnbindForWriting();
    void BindForReading();
    void UnbindForReading();
    void SetReadBuffer(GBUFFER_TEXTURE_TYPE TextureType);

    void BindForRender();
    void UnbindForRender();

private:
    GLuint m_fbo;
    GLuint m_textures[GBUFFER_NUM_TEXTURES];
    GLuint m_shininessTexture;
    GLuint m_depthTexture;
};

#endif
