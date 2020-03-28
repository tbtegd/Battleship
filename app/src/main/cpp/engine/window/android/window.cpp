#include <android/native_window.h>
#include "window.hpp"

namespace {
	inline constexpr EGLint surface_attribs[] = {
            EGL_RENDERABLE_TYPE,
            EGL_OPENGL_ES3_BIT,
            EGL_SURFACE_TYPE,
            EGL_WINDOW_BIT,
            EGL_BLUE_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_RED_SIZE, 8,
            EGL_DEPTH_SIZE, 16,
            EGL_NONE
    };

    inline constexpr EGLint context_attribs[] = {
            EGL_CONTEXT_CLIENT_VERSION, 3,
            EGL_NONE
    };
}

void engine::window::internal::Window::createDisplay() {
	display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
	eglInitialize(display, nullptr, nullptr);
}

void engine::window::internal::Window::createContext() {
	EGLint numConfigs;
	eglChooseConfig(display, surface_attribs, &config, 1, &numConfigs);
	eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);

	context = eglCreateContext(display, config, nullptr, context_attribs);
}

void engine::window::internal::Window::createSurface(void *native) {
	window = reinterpret_cast<NativeWindowType>(native);
	surface = eglCreateWindowSurface(display, config, window, nullptr);

	eglQuerySurface(display, surface, EGL_WIDTH, &width);
	eglQuerySurface(display, surface, EGL_HEIGHT, &height);

	ANativeWindow_acquire(window);
}

void engine::window::internal::Window::updateSurface() {
	ANativeWindow_setBuffersGeometry(window, width, height, format);
}

void engine::window::internal::Window::destroySurface() {
	eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
	eglDestroySurface(display, surface);
	surface = EGL_NO_SURFACE;

	ANativeWindow_release(window);
}

void engine::window::internal::Window::destroyContext() {
	eglDestroyContext(display, context);
	context = EGL_NO_CONTEXT;
}

void engine::window::internal::Window::destroyDisplay() {
	eglTerminate(display);
	display = EGL_NO_DISPLAY;
}

void engine::window::internal::Window::makeCurrent() {
	eglMakeCurrent(display, surface, surface, context);
}

void engine::window::internal::Window::swapBuffers() {
    eglSwapBuffers(display, surface);
}