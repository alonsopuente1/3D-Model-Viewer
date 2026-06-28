#include "Texture.h"

#include <glad/glad.h>

#include "error.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

void Texture::Create()
{
    if (mTexture != 0)
    {
        std::cerr << "Can't overwrite a texture that's already created!" << std::endl;
        return;
    }

    GLCall(glGenTextures(1, &mTexture));
}

void Texture::Bind(unsigned int texUnit)
{
    if (mTexture == 0)
    {
        std::cerr << "Can't bind uncreated texture!" << std::endl;
        return;
    }

    if (texUnit > 31)
    {
        std::cerr << "Can't bind to texture unit above 31!" << std::endl;
        return;
    }

    GLCall(glActiveTexture(GL_TEXTURE0 + texUnit));
    GLCall(glBindTexture(GL_TEXTURE_2D, mTexture));
}

bool Texture::LoadTexture(const char* texturePath)
{
    int width, height, numChannels;

    stbi_set_flip_vertically_on_load(1);

    unsigned char* img = stbi_load(texturePath, &width, &height, &numChannels, 0);

    if (!img)
    {
        std::cerr << "Failed to load texture! File path " << texturePath << std::endl;
        return false;
    }

    Delete();

    Create();

    Bind(0);

    std::cerr << "Num channels in texture: " << numChannels << std::endl;

    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT));

    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img));
    GLCall(glGenerateMipmap(GL_TEXTURE_2D));

    stbi_image_free(img);

    return true;
}

bool Texture::IsCreated() const
{
    return mTexture != 0;
}

void Texture::Delete()
{
    if (mTexture == 0)
        return;

    GLCall(glDeleteTextures(1, &mTexture));
    mTexture = 0;
}

void Texture::Unbind(unsigned int texUnit)
{
    GLCall(glActiveTexture(GL_TEXTURE0 + texUnit));
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}
