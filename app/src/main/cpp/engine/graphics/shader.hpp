#pragma once

#include <GLES3/gl3.h>
#include <string_view>

namespace engine::graphics {
	struct Shader;

	struct ShaderSource {
		friend Shader;
		constexpr ShaderSource(std::string_view source) noexcept : __data(source.data()), __size(source.size()) {}
	private:
		GLuint compile(GLenum type);

		const GLchar* __data;
		GLint      __size;
	};

	struct Shader {
	    friend Shader newShader(ShaderSource vertex_src, ShaderSource fragment_src);

		Shader(const Shader& shader) = delete;
		Shader& operator=(const Shader& shader) = delete;
		Shader(Shader&& shader) noexcept : m_program(shader.m_program) {
			shader.m_program = 0;
		}
		Shader& operator=(Shader&& shader) noexcept {
			m_program = shader.m_program;
			shader.m_program = 0;
			return *this;
		}
		inline void release() {
			if (m_program != 0) {
				glDeleteProgram(m_program);
				m_program = 0;
			}
		}
		inline operator GLuint() const noexcept {
			return m_program;
		}
	private:
        Shader(ShaderSource vertex_source, ShaderSource fragment_source);

        GLuint m_program;
	};
}