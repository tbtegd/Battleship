#pragma once

#include <GLES3/gl3.h>
#include <string_view>

namespace engine::graphics {
	struct Shader;

	struct ShaderSource {
		constexpr ShaderSource(std::string_view source) noexcept : __data(source.data()), __size(source.size()) {}

		GLuint compile(GLenum type) const;

	private:
		const GLchar* __data;
		GLint      __size;
	};

	struct Shader {
	    friend std::shared_ptr<Shader> newShader(ShaderSource vertex_src, ShaderSource fragment_src);

	    constexpr Shader() noexcept : m_program(0) {}
		Shader(const Shader& other) = delete;
		Shader& operator=(const Shader& other) = delete;
		Shader(Shader&& other) noexcept : m_program(other.m_program) {
			other.m_program = 0;
		}
		Shader& operator=(Shader&& shader) noexcept {
			m_program = shader.m_program;
			shader.m_program = 0;
			return *this;
		}
		~Shader();
		inline operator GLuint() const noexcept {
			return m_program;
		}
	private:
		Shader(ShaderSource vertex_src, ShaderSource fragment_src);

        GLuint m_program;
	};
}