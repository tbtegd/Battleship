#pragma once

#include "android/window.hpp"

namespace engine::window {
	struct Window {
		Window() : impl() {}

		inline void create() {
			impl.createDisplay();
			impl.createContext();
		}

		inline void makeCurrent() {
			impl.makeCurrent();
		}
		inline void swapBuffers() {
			impl.swapBuffers();
		}
		inline void createSurface(void* native) {
			impl.createSurface(native);
		}
		inline void destroySurface() {
			impl.destroySurface();
		}
		inline void destroy() {
			impl.destroyContext();
			impl.destroyDisplay();
		}
		inline void close() {
			impl.close();
		}
		inline bool is_open() {
			return impl.is_open();
		}
	private:
		internal::Window impl;
	};

	extern void close();
	extern bool is_open();
}