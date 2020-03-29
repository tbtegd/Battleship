#include <string_view>
#include <thread>

#include <system/event/event.hpp>
#include <window/window.hpp>
#include <graphics/graphics.hpp>

static const GLfloat g_vertex_buffer_data[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        0.0f,  1.0f, 0.0f,
};

static const GLfloat g_color_buffer_data[] = {
        0.583f,  0.771f,  0.014f,
        0.609f,  0.115f,  0.436f,
        0.327f,  0.483f,  0.844f
};

using std::string_view_literals::operator""sv;

constexpr engine::graphics::ShaderSource vertex_src = R"(
    #version 300 es

    layout(location = 0) in vec3 vertexPosition;
    layout(location = 1) in vec3 vertexColor;

    out vec3 _Color;

    void main() {
        _Color = vertexColor;
        gl_Position = vec4(vertexPosition, 1.0);
    }
)"sv;

constexpr engine::graphics::ShaderSource fragment_src = R"(
    #version 300 es
    out vec3 color;

    in vec3 _Color;

    void main(){
        color = _Color;
    }
)"sv;

void android_main() {
    if (engine::window::init() < 0) {
        std::__throw_runtime_error("Failed to init window!");
    }

    /*Game Loop*/ {
        auto shader(engine::graphics::newShader(vertex_src, fragment_src));
        glUseProgram(shader);

        GLuint m_vao, m_vbo, m_cbo;
        glGenVertexArrays(1, &m_vao);
        glBindVertexArray(m_vao);

        glGenBuffers(1, &m_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

        glGenBuffers(1, &m_cbo);
        glBindBuffer(GL_ARRAY_BUFFER, m_cbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, m_cbo);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        bool running = true;
        engine::system::event::Message message{};
        while (running) {
            while (engine::system::event::poll(message)) {
                switch (message.type) {
                    case engine::system::event::Event::Quit:
                        running = false;
                        break;
                }
            }

//            if (engine::graphics::isActive()) {
            glClearColor(1, 1, 1, 1);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glBindVertexArray(m_vao);
            glDrawArrays(GL_TRIANGLES, 0, 3);
            glBindVertexArray(0);

            engine::window::swapBuffers();
//            }
            std::this_thread::yield();
        }
    }

    engine::window::quit();
}