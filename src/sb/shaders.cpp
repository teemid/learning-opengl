#include <memory>

#include "sb/shaders.h"


GLuint compile_shaders(
    const GLchar * const vertex_shader_source,
    const GLchar * const fragment_shader_source
)
{
    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
    glCompileShader(vertex_shader);
   
    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
    glCompileShader(fragment_shader);

    GLuint program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return program;
}

GLuint compile_shaders(const std::vector<ShaderInfo> & shaders)
{
    std::vector<GLuint> shaderIds;

    for (auto & info : shaders)
    {
        GLuint shaderId = glCreateShader(info.type);
        glShaderSource(shaderId, 1, &info.source, NULL);
        glCompileShader(shaderId);

        GLsizei length;
        glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &length);

        GLsizei actual;
        auto log = std::make_unique<char[]>(length);
        glGetShaderInfoLog(shaderId, length, &actual, log.get());

        shaderIds.push_back(shaderId);
    }

    GLuint program = glCreateProgram();
    for (auto & id : shaderIds)
    {
        glAttachShader(program, id);
    }

    glLinkProgram(program);

    for (auto & id : shaderIds)
    {
        glDeleteShader(id);
    }

    return program;
}
