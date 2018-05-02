#ifndef SB_CHAPTER_1_H
#define SB_CHAPTER_1_H

#include "sb/application.h"
#include "sb/shaders.h"
#include "gl.h"

#include <cmath>


class Chapter2_1 final : public Application
{
public:
    void render(double currentTime) override
    {
        static const GLfloat red[] = { 1.0f, 0.0f, 0.0f, 1.0f };

        glClearBufferfv(GL_COLOR, 0, red);
    }
};

class Chapter2_2 final : public Application
{
public:
    void render(double currentTime) override
    {
        const GLfloat color[] = { (float)sin(currentTime) * 0.5f + 0.5f,
                                  (float)cos(currentTime) * 0.5f + 0.5f,
                                  0.0f, 1.0f };
        glClearBufferfv(GL_COLOR, 0, color);
    }
};

class Chapter2_3 final : public Application
{
public:
    void startup() override
    {
        m_program = compile_shaders(m_vertex, m_fragment);
        glCreateVertexArrays(1, &m_vao);
        glBindVertexArray(m_vao);
    }

    void render(double currentTime) override
    {
        const GLfloat color[] = { (float)sin(currentTime) * 0.5f + 0.5f,
                                  (float)cos(currentTime) * 0.5f + 0.5f,
                                  0.0f, 1.0f };
        glClearBufferfv(GL_COLOR, 0, color);

        glUseProgram(m_program);

        glDrawArrays(GL_POINTS, 0, 1);
    }

    void shutdown() override
    {
        glDeleteProgram(m_program);
        glDeleteVertexArrays(1, &m_vao);
    }

private:
    GLuint m_program;
    GLuint m_vao;

    const char * m_vertex = R"(
        #version 450 core

        void main(void)
        {
            gl_Position = vec4(0.0, 0.0, 0.5, 1.0);
        }
    )";

    const char * m_fragment = R"(
        #version 450 core

        out vec4 color;

        void main(void)
        {
            color = vec4(0.0, 0.8, 1.0, 1.0);
        }
    )";
};

class Chapter2_4 final : public Application
{
public:
    void startup() override
    {
        m_program = compile_shaders(m_vertex, m_fragment);
        glCreateVertexArrays(1, &m_vao);
        glBindVertexArray(m_vao);
    }

    void render(double currentTime) override
    {
        const GLfloat color[] = { 0.0f, 0.2f, 0.0f, 1.0f };
        glClearBufferfv(GL_COLOR, 0, color);

        glUseProgram(m_program);

        glDrawArrays(GL_TRIANGLES, 0, 3);
    }

    void shutdown() override
    {
        glDeleteProgram(m_program);
        glDeleteVertexArrays(1, &m_vao);
    }

private:
    GLuint m_program;
    GLuint m_vao;

    const char * m_vertex = R"(
        #version 450 core

        void main()
        {
            // Declare a hard-coded array of positions
            const vec4 vertices[3] = vec4[3](vec4( 0.25, -0.25, 0.5, 1.0),
                                              vec4(-0.25, -0.25, 0.5, 1.0),
                                              vec4( 0.25,  0.25, 0.5, 1.0));

            gl_Position = vertices[gl_VertexID];
        }
    )";

    const char * m_fragment = R"(
        #version 450 core

        out vec4 color;

        void main(void)
        {
            color = vec4(0.0, 0.8, 1.0, 1.0);
        }
    )";
};

#endif