#pragma once

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <atomic>

namespace engine::window::internal {
	struct Window {
		void createDisplay();
		void createContext();
		void createSurface(void* native);

		void destroySurface();
		void destroyContext();
		void destroyDisplay();

		void makeCurrent();
		void swapBuffers();

		inline void close() {
			opened.store(false);
		}
		inline bool is_open() {
			return opened.load();
		}
	private:
		EGLDisplay display = EGL_NO_DISPLAY;
		EGLContext context = EGL_NO_CONTEXT;
		EGLSurface surface = EGL_NO_SURFACE;
		EGLConfig config = EGL_NO_CONFIG_KHR;
		EGLint format = 0;
		std::atomic_bool opened{true};
	};
}