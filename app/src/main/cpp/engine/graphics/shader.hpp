#pragma once

#include <GLES3/gl3.h>
#include <utility>
#include <string_view>

namespace engine::graphics {
	struct Shader;
	struct Graphics;

	struct ShaderSource {
		friend Shader;
		constexpr ShaderSource(std::string_view source) noexcept : __data(source.data()), __size(source.size()) {}
	private:
		GLuint compile(GLenum type);

		const GLchar* __data;
		GLint      __size;
	};

	struct Shader {
		friend Graphics;

		Shader(const Shader& shader) = delete;
		Shader& operator=(const Shader& shader) = delete;
		Shader(Shader&& shader) : m_program(std::exchange(shader.m_program, 0)) {}
		Shader& operator=(Shader&& shader)  {
			m_program = std::exchange(shader.m_program, 0);
			return *this;
		}
		inline void release() {
			if (m_program != 0) {
				glDeleteProgram(m_program);
				m_program = 0;
			}
		}
		inline operator GLuint() const {
			return m_program;
		}
	private:
		Shader(ShaderSource vertex_source, ShaderSource fragment_source);

		GLuint m_program;
	};
}