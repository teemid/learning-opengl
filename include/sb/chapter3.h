#ifndef OPENGL_CHAPTER_3_H
#define OPENGL_CHAPTER_3_H

#include "sb/application.h"


class Chapter3_1 final : public Application
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

        GLfloat attrib[] = { (float)sin(currentTime) * 0.5f,
                             (float)cos(currentTime) * 0.6f,
                             0.0f, 0.0f };

        glVertexAttrib4fv(0, attrib);

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

        layout (location = 0) in vec4 offset;

        void main()
        {
            // Declare a hard-coded array of positions
            const vec4 vertices[3] = vec4[3](vec4( 0.25, -0.25, 0.5, 1.0),
                                             vec4(-0.25, -0.25, 0.5, 1.0),
                                             vec4( 0.25,  0.25, 0.5, 1.0));

            gl_Position = vertices[gl_VertexID] + offset;
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


class Chapter3_2 final : public Application
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

        GLfloat attrib[] = { (float)sin(currentTime) * 0.5f,
                             (float)cos(currentTime) * 0.6f,
                             0.0f, 0.0f };

        glVertexAttrib4fv(0, attrib);

        GLfloat colorAttrib[] = { (float)sin(currentTime) * 0.5f + 0.5f, (float)cos(currentTime) * 0.6f + 0.5f, 0.2f, 1.0f };
        glVertexAttrib4fv(1, colorAttrib);

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

        layout (location = 0) in vec4 offset;
        layout (location = 1) in vec4 color;

        out vec4 vs_color;

        void main()
        {
            // Declare a hard-coded array of positions
            const vec4 vertices[3] = vec4[3](vec4( 0.25, -0.25, 0.5, 1.0),
                                             vec4(-0.25, -0.25, 0.5, 1.0),
                                             vec4( 0.25,  0.25, 0.5, 1.0));

            gl_Position = vertices[gl_VertexID] + offset;

            vs_color = color;
        }
    )";

    const char * m_fragment = R"(
        #version 450 core

        in vec4 vs_color;

        out vec4 color;

        void main(void)
        {
            color = vs_color;
        }
    )";
};


class Chapter3_3 final : public Application
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

        GLfloat attrib[] = { (float)sin(currentTime) * 0.5f,
                             (float)cos(currentTime) * 0.6f,
                             0.0f, 0.0f };

        glVertexAttrib4fv(0, attrib);

        GLfloat colorAttrib[] = { (float)sin(currentTime) * 0.5f + 0.5f, (float)cos(currentTime) * 0.6f + 0.5f, 0.2f, 1.0f };
        glVertexAttrib4fv(1, colorAttrib);

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

        layout (location = 0) in vec4 offset;
        layout (location = 1) in vec4 color;

        out VS_OUT
        {
            vec4 color;
        } vs_out;

        void main()
        {
            // Declare a hard-coded array of positions
            const vec4 vertices[3] = vec4[3](vec4( 0.25, -0.25, 0.5, 1.0),
                                             vec4(-0.25, -0.25, 0.5, 1.0),
                                             vec4( 0.25,  0.25, 0.5, 1.0));

            gl_Position = vertices[gl_VertexID] + offset;

            vs_out.color = color;
        }
    )";

    const char * m_fragment = R"(
        #version 450 core

        in VS_OUT
        {
            vec4 color;
        } fs_in;

        out vec4 color;

        void main(void)
        {
            color = fs_in.color;
        }
    )";
};


class Chapter3_4 final : public Application
{
public:
    void startup() override
    {
        m_program = compile_shaders({
            { GL_VERTEX_SHADER, m_vertex },
            { GL_TESS_CONTROL_SHADER, m_tess_control_shader },
            { GL_TESS_EVALUATION_SHADER, m_tess_evaluation_shader },
            { GL_FRAGMENT_SHADER, m_fragment },
        });
        glCreateVertexArrays(1, &m_vao);
        glBindVertexArray(m_vao);

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    void render(double currentTime) override
    {
        const GLfloat color[] = { 0.0f, 0.2f, 0.0f, 1.0f };
        glClearBufferfv(GL_COLOR, 0, color);

        glUseProgram(m_program);

        glDrawArrays(GL_PATCHES, 0, 3);
    }

    void shutdown() override
    {
        glDeleteProgram(m_program);
        glDeleteVertexArrays(1, &m_vao);
    }

private:
    GLuint m_program;
    GLuint m_vao;

    const char * m_tess_control_shader = R"(
        #version 450 core

        layout (vertices = 3) out;

        void main()
        {
            if (gl_InvocationID == 0)
            {
                gl_TessLevelInner[0] = 5.0;
                gl_TessLevelOuter[0] = 5.0;
                gl_TessLevelOuter[1] = 5.0;
                gl_TessLevelOuter[2] = 5.0;
            }

            gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
        }
    )";

    const char * m_tess_evaluation_shader = R"(
        #version 450 core

        layout (triangles, equal_spacing, cw) in;

        void main()
        {
            gl_Position = (gl_TessCoord.x * gl_in[0].gl_Position) +
                          (gl_TessCoord.y * gl_in[1].gl_Position) +
                          (gl_TessCoord.z * gl_in[2].gl_Position);
        }
    )";

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


class Chapter3_5 final : public Application
{
public:
    void startup() override
    {
        m_program = compile_shaders({
            { GL_VERTEX_SHADER, m_vertex },
            { GL_TESS_CONTROL_SHADER, m_tess_control_shader },
            { GL_TESS_EVALUATION_SHADER, m_tess_evaluation_shader },
            { GL_GEOMETRY_SHADER, m_geometry },
            { GL_FRAGMENT_SHADER, m_fragment },
        });
        glCreateVertexArrays(1, &m_vao);
        glBindVertexArray(m_vao);

        glPointSize(5.0);
    }

    void render(double currentTime) override
    {
        const GLfloat color[] = { 0.0f, 0.2f, 0.0f, 1.0f };
        glClearBufferfv(GL_COLOR, 0, color);

        glUseProgram(m_program);

        glDrawArrays(GL_PATCHES, 0, 3);
    }

    void shutdown() override
    {
        glDeleteProgram(m_program);
        glDeleteVertexArrays(1, &m_vao);
    }

private:
    GLuint m_program;
    GLuint m_vao;

    const char * m_tess_control_shader = R"(
        #version 450 core

        layout (vertices = 3) out;

        void main()
        {
            if (gl_InvocationID == 0)
            {
                gl_TessLevelInner[0] = 5.0;
                gl_TessLevelOuter[0] = 5.0;
                gl_TessLevelOuter[1] = 5.0;
                gl_TessLevelOuter[2] = 5.0;
            }

            gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
        }
    )";

    const char * m_tess_evaluation_shader = R"(
        #version 450 core

        layout (triangles, equal_spacing, cw) in;

        void main()
        {
            gl_Position = (gl_TessCoord.x * gl_in[0].gl_Position) +
                          (gl_TessCoord.y * gl_in[1].gl_Position) +
                          (gl_TessCoord.z * gl_in[2].gl_Position);
        }
    )";

    const char * m_geometry = R"(
        #version 450 core

        layout (triangles) in;
        layout (points, max_vertices = 3) out;

        void main()
        {
            int i;

            for (i = 0; i < gl_in.length(); i++)
            {
                gl_Position = gl_in[i].gl_Position;
                EmitVertex();
            }
        }
    )";

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

class Chapter3_6 final : public Application
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
            color = vec4(sin(gl_FragCoord.x * 0.25) * 0.5 + 0.5,
                         cos(gl_FragCoord.y * 0.25) * 0.5 + 0.5,
                         sin(gl_FragCoord.x * 0.15) * cos(gl_FragCoord.y * 0.15),
                         1.0);
        }
    )";
};


/*
 * Illustrating fragment shader input color blending across triangle.
 */
class Chapter3_7 final : public Application
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

        out vec4 vs_color;

        void main()
        {
            // Declare a hard-coded array of positions
            const vec4 vertices[3] = vec4[3](vec4( 0.25, -0.25, 0.5, 1.0),
                                             vec4(-0.25, -0.25, 0.5, 1.0),
                                             vec4( 0.25,  0.25, 0.5, 1.0));

            const vec4 colors[] = vec4[3](vec4(1.0, 0.0, 0.0, 1.0),
                                          vec4(0.0, 1.0, 0.0, 1.0),
                                          vec4(0.0, 0.0, 1.0, 1.0));

            gl_Position = vertices[gl_VertexID];

            vs_color = colors[gl_VertexID];
        }
    )";

    const char * m_fragment = R"(
        #version 450 core

        in vec4 vs_color;

        out vec4 color;

        void main(void)
        {
            color = vs_color;
        }
    )";
};

#endif
