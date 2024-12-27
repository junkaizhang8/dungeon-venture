#include "Texture.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

#include "utils/debug.h"

Texture::Texture(const std::string& filepath)
    : id(0),
      filepath(filepath),
      localBuffer(nullptr),
      width(0),
      height(0),
      bpp(0)
{
    // Flip the image vertically since OpenGL expects the origin to be at the
    // bottom-left corner
    stbi_set_flip_vertically_on_load(1);

    // Load the image data
    localBuffer = stbi_load(filepath.c_str(), &width, &height, &bpp, 4);

    // Generate the texture
    glCall(glGenTextures(1, &id));
    glCall(glBindTexture(GL_TEXTURE_2D, id));

    // Set the texture parameters
    glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    // Upload the image data to the texture
    glCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA,
                        GL_UNSIGNED_BYTE, localBuffer));

    // Unbind the texture
    glCall(glBindTexture(GL_TEXTURE_2D, 0));

    if (localBuffer)
        // Free the image data
        stbi_image_free(localBuffer);
}

Texture::~Texture()
{
    // Delete the texture
    glCall(glDeleteTextures(1, &id));
}

void Texture::bind(unsigned int slot) const
{
    // Activate the texture slot
    glCall(glActiveTexture(GL_TEXTURE0 + slot));

    // Bind the texture
    glCall(glBindTexture(GL_TEXTURE_2D, id));
}

void Texture::unbind() const { glCall(glBindTexture(GL_TEXTURE_2D, 0)); }