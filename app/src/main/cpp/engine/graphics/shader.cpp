#include "shader.hpp"

#include <cstdio>

namespace engine::graphics {
	GLuint ShaderSource::compile(GLenum type) {
		GLuint shader = glCreateShader(type);
		glShaderSource(shader, 1, &__data, &__size);
		glCompileShader(shader);

		GLint status = GL_FALSE;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
		if (status == GL_FALSE) {
			GLint length;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
			if (length > 0) {
				char* message = new char[length];
				glGetShaderInfoLog(shader, length, nullptr, message);
				fwrite(message, length, 1, stderr);
				delete[] message;
			}
			glDeleteShader(shader);
			return 0;
		}
		return shader;
	}

	Shader::Shader(ShaderSource vertex_source, ShaderSource fragment_source) : m_program(0) {
		GLuint vertex = vertex_source.compile(GL_VERTEX_SHADER);
		if (vertex == 0) {
			return;
		}
		GLuint fragment = fragment_source.compile(GL_FRAGMENT_SHADER);
		if (fragment == 0) {
			return;
		}

		GLuint program = glCreateProgram();
		glAttachShader(program, vertex);
		glAttachShader(program, fragment);
		glLinkProgram(program);

		glDeleteShader(vertex);
		glDeleteShader(fragment);

		GLint status = GL_FALSE;
		glGetProgramiv(program, GL_LINK_STATUS, &status);

		if (status == GL_FALSE) {
			GLint length;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
			if (length > 0) {
				char* message = new char[length];
				glGetProgramInfoLog(program, length, nullptr, message);
				fwrite(message, length, 1, stderr);
				delete[] message;
			}
			glDeleteProgram(program);
			return;
		}

		m_program = program;
	}
}