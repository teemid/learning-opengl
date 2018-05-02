#ifndef SB_SHADERS_H
#define SB_SHADERS_H

#include <vector>
#include "gl.h"

struct ShaderInfo
{
    GLenum type;
    const GLchar * const source;
};

GLuint compile_shaders(
    const GLchar * const vertex_shader_source,
    const GLchar * const fragment_shader_source
);

GLuint compile_shaders(const std::vector<ShaderInfo> & shaders);

#endif