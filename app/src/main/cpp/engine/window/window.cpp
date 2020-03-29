#include "window.hpp"

#include <EGL/egl.h>
#include <android/native_window.h>

namespace {
    constexpr EGLint surface_attribs[] = {
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

    constexpr EGLint context_attribs[] = {
            EGL_CONTEXT_CLIENT_VERSION, 3,
            EGL_NONE
    };

    NativeWindowType window;
    EGLDisplay display;
    EGLContext context;
    EGLSurface surface;
    EGLConfig config;
}

extern "C" NativeWindowType ANativeWindow_getInstance();

namespace engine::window {
	int init() {
		auto win = ANativeWindow_getInstance();
		ANativeWindow_acquire(win);

		display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
		if (display == EGL_NO_DISPLAY) {
			return -1;
		}
		if (eglInitialize(display, nullptr, nullptr) != EGL_TRUE) {
			return -1;
		}

		EGLint numConfigs;
		if (eglChooseConfig(display, surface_attribs, &config, 1, &numConfigs) != EGL_TRUE) {
			return -1;
		}

		EGLint format;
		if (eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format) != EGL_TRUE) {
			return -1;
		}

		context = eglCreateContext(display, config, nullptr, context_attribs);
		if (context == EGL_NO_CONTEXT) {
			return -1;
		}
		surface = eglCreateWindowSurface(display, config, win, nullptr);
		if (surface == EGL_NO_SURFACE) {
			return -1;
		}

		EGLint width, height;
		eglQuerySurface(display, surface, EGL_WIDTH, &width);
		eglQuerySurface(display, surface, EGL_HEIGHT, &height);

		if (ANativeWindow_setBuffersGeometry(win, width, height, format) < 0) {
			return -1;
		}

		if (eglMakeCurrent(display, surface, surface, context) != EGL_TRUE) {
			return -1;
		}

		return 0;
	}

	void quit() {
		eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
		eglDestroySurface(display, surface);
		eglDestroyContext(display, context);
		eglTerminate(display);
		ANativeWindow_release(ANativeWindow_getInstance());
	}

	void swapBuffers() {
		eglSwapBuffers(display, surface);
	}
}