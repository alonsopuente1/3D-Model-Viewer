#pragma once

class Texture
{

public:
    Texture() {}

    void Create();

    void Bind(unsigned int texUnit);

    bool LoadTexture(const char* texturePath);

    bool IsCreated() const;

    void Delete();

    static void Unbind(unsigned int texUnit);

private:

    unsigned int mTexture = 0;

};

