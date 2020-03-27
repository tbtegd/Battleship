#pragma once

#include <atomic>
#include "shader.hpp"

namespace engine::graphics {
	struct Graphics {
		void setActive(bool enable);
		bool isActive();
		void present();
		static Shader newShader(ShaderSource vertex_source, ShaderSource fragment_source);
	private:
		std::atomic_bool active{true};
	};

	extern bool isActive();
	extern void present();
	extern Shader newShader(ShaderSource vertex_source, ShaderSource fragment_source);
}