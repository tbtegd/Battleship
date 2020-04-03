#include <string_view>
#include <thread>

#include <system/event/event.hpp>
#include <window/window.hpp>
#include <graphics/graphics.hpp>
#include <engine/graphics/texture.hpp>

using std::string_view_literals::operator""sv;

constexpr engine::graphics::ShaderSource vertex_src = R"(
    #version 300 es

    layout(location = 0) in vec3 _Position;
    layout(location = 1) in vec2 _FragCoord;

    out vec2 fragCoord;

    void main() {
        fragCoord = _FragCoord;
        gl_Position = vec4(_Position, 1.0);
    }
)"sv;

constexpr engine::graphics::ShaderSource fragment_src = R"(
    #version 300 es

    in vec2 fragCoord;
    out vec4 fragColor;

    uniform sampler2D img;

    void main(){
        fragColor = texture(img, fragCoord);
    }
)"sv;

void android_main() {
    if (engine::window::init() < 0) {
        std::__throw_runtime_error("Failed to init window!");
    }

    /*Game Loop*/ {
        auto shader(engine::graphics::newShader(vertex_src, fragment_src));
        glUseProgram(shader->operator GLuint());

        auto field(engine::graphics::newImage("field.png"));
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, field->operator GLuint());

        auto s = float(field->getWidth()) / float(field->getHeight()) * 0.5f;

        const GLfloat g_vertex_buffer_data[] = {
                -1.0f, -s, 0.0f,
                1.0f, -s, 0.0f,
                -1.0f, s, 0.0f,

                -1.0f, s, 0.0f,
                1.0f, -s, 0.0f,
                1.0f, s, 0.0f,
        };

        const GLfloat g_uv_buffer_data[] = {
                0, 1,
                1, 1,
                0, 0,

                0, 0,
                1, 1,
                1, 0
        };

        GLuint m_vao, m_vbo, m_ubo;
        glGenVertexArrays(1, &m_vao);
        glBindVertexArray(m_vao);

        glGenBuffers(1, &m_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        glGenBuffers(1, &m_ubo);
        glBindBuffer(GL_ARRAY_BUFFER, m_ubo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(g_uv_buffer_data), g_uv_buffer_data, GL_STATIC_DRAW);

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, m_ubo);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

        glClearColor(1, 1, 1, 1);

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

            if (engine::graphics::isActive()) {
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                glBindVertexArray(m_vao);
                glDrawArrays(GL_TRIANGLES, 0, 6);
                glBindVertexArray(0);

                engine::window::swapBuffers();
            }
            std::this_thread::yield();
        }
    }

    engine::window::quit();
}