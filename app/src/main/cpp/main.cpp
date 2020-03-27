#include "event/event.hpp"
#include "graphics/graphics.hpp"
#include "window/window.hpp"

#include <GLES3/gl3.h>
#include <thread>
#include <vector>

inline namespace math {
	template <size_t N, typename T> //requires (std::is_integral_v<T> || std::is_floating_point_v<T>)
	struct vec;

	template <typename T>
	struct vec<2, T> {
		T x, y;

		explicit constexpr vec() noexcept : x(0), y(0) {}
		explicit constexpr vec(T scalar) noexcept : x(scalar), y(scalar) {}
		explicit constexpr vec(T x, T y, T z) noexcept : x(x), y(y) {}
	};

	template <typename T>
	struct vec<3, T> {
		T x, y, z;

		explicit constexpr vec() noexcept : x(0), y(0), z(0) {}
		explicit constexpr vec(T scalar) noexcept : x(scalar), y(scalar), z(scalar) {}
		explicit constexpr vec(T x, T y, T z) noexcept : x(x), y(y), z(z) {}
	};

	template <typename T>
	struct vec<4, T> {
		T x, y, z, w;

		explicit constexpr vec() noexcept : x(0), y(0), z(0), w(0) {}
		explicit constexpr vec(T scalar) noexcept : x(scalar), y(scalar), z(scalar), w(scalar) {}
		explicit constexpr vec(T x, T y, T z, T w) noexcept : x(x), y(y), z(z), w(w) {}
	};

    using vec2 = vec<2, float>;
    using vec3 = vec<3, float>;
    using vec4 = vec<4, float>;
    using ivec2 = vec<2, int>;
    using ivec3 = vec<3, int>;
    using ivec4 = vec<4, int>;
    using bvec2 = vec<2, bool>;
    using bvec3 = vec<3, bool>;
    using bvec4 = vec<4, bool>;
}

using Index = uint32_t;
using Vertex = vec3;
using Normal = vec3;
using Color = vec4;
using UV = vec2;

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

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <GLES/gl.h>
#include <GLES/glext.h>
#include <GLES3/gl3ext.h>

#include <fstream>
#include <sstream>
#include <string_view>

using std::string_literals::operator""s;
using std::string_view_literals::operator""sv;

const std::string vertex_src = R"(
    #version 300 es

    layout(location = 0) in vec3 vertexPosition;
    layout(location = 1) in vec3 vertexColor;

    out vec3 _Color;

    void main() {
        _Color = vertexColor;
        gl_Position = vec4(vertexPosition, 1.0);
    }
)"s;

const std::string fragment_src = R"(
    #version 300 es
    out vec3 color;

    in vec3 _Color;

    void main(){
        color = _Color;
    }
)"s;

GLuint LoadShaders(std::string const& vsource, std::string const& fsource) {
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    GLint Result = GL_FALSE;
    int InfoLogLength;

    // Компилируем Вершинный шейдер
    auto VertexSourcePointer = vsource.c_str();
//    auto VertexSourceLength = GLint(vsource.size());
    glShaderSource(VertexShaderID, 1, &VertexSourcePointer , nullptr);
    glCompileShader(VertexShaderID);

    // Выполняем проверку Вершинного шейдера
    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
        std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
        glGetShaderInfoLog(VertexShaderID, InfoLogLength, nullptr, &VertexShaderErrorMessage[0]);
        fprintf(stdout, "%sn", &VertexShaderErrorMessage[0]);
    }

    // Компилируем Фрагментный шейдер
    auto FragmentSourcePointer = fsource.c_str();
//    auto FragmentSourceLength = GLint(fsource.size());
    glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, nullptr);
    glCompileShader(FragmentShaderID);

    // Проверяем Фрагментный шейдер
    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
        std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
        glGetShaderInfoLog(FragmentShaderID, InfoLogLength, nullptr, &FragmentShaderErrorMessage[0]);
        fprintf(stdout, "%s\n", &FragmentShaderErrorMessage[0]);
    }

    // Создаем шейдерную программу и привязываем шейдеры к ней
    fprintf(stdout, "Создаем шейдерную программу и привязываем шейдеры к нейn");
    GLuint ProgramID = glCreateProgram();
    glAttachShader(ProgramID, VertexShaderID);
    glAttachShader(ProgramID, FragmentShaderID);
    glLinkProgram(ProgramID);

    // Проверяем шейдерную программу
    glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
    glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
        std::vector<char> ProgramErrorMessage(InfoLogLength+1);
        glGetProgramInfoLog(ProgramID, InfoLogLength, nullptr, &ProgramErrorMessage[0]);
        fprintf(stdout, "%s\n", &ProgramErrorMessage[0]);
    }

    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);

    return ProgramID;
}

int main() {
    GLuint m_vao, m_vbo, m_cbo;

    GLuint programID = LoadShaders(vertex_src, fragment_src);

    glUseProgram(programID);

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
    glVertexAttribPointer(0, 3,  GL_FLOAT, GL_FALSE, 0, nullptr);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, m_cbo);
    glVertexAttribPointer(1, 3,  GL_FLOAT, GL_FALSE, 0, nullptr);
    glClearColor(1, 1, 1, 1);

    engine::event::Event event;
    while (engine::window::is_open()) {
        engine::event::pump();
        while (engine::event::poll(event)) {
            engine::event::handle(event);
        }

        if (engine::graphics::is_active()) {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glBindVertexArray(m_vao);
            glDrawArrays(GL_TRIANGLES, 0, 3);
            glBindVertexArray(0);

            engine::graphics::present();
        }
        std::this_thread::yield();
    }

	return 0;
}