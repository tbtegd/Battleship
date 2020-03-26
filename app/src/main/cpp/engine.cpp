#include <android/native_window.h>
#include <mutex>
#include <queue>
#include <optional>
#include <linux/fcntl.h>
#include <unistd.h>

#include "engine.h"

namespace {
    inline constexpr EGLint surface_attribs[] = {
            EGL_RENDERABLE_TYPE,
            EGL_OPENGL_ES2_BIT,
            EGL_SURFACE_TYPE,
            EGL_WINDOW_BIT,
            EGL_BLUE_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_RED_SIZE, 8,
            EGL_DEPTH_SIZE, 16,
            EGL_NONE
    };

    inline constexpr EGLint context_attribs[] = {
            EGL_CONTEXT_CLIENT_VERSION, 2,
            EGL_NONE
    };

    EGLDisplay display = EGL_NO_DISPLAY;
    EGLContext context = EGL_NO_CONTEXT;
    EGLSurface surface = EGL_NO_SURFACE;
    EGLConfig config = EGL_NO_CONFIG_KHR;
    EGLint format = 0;

    std::mutex mutex;
    std::queue<engine_event_t> m_events{};

    std::atomic<bool> rendering;
    std::atomic<bool> quit = false;

//    int fd[2];
}

bool engine_request_quit() {
    return quit.load();
}

void engine_initialize() {
    //    if (pipe(fd) < 0) {
//        perror("pipe ");
//        exit(1);
//    }
//    fcntl(fd[0], F_SETFL, O_NONBLOCK);

    quit.store(false);

    EGLint numConfigs;

    display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    eglInitialize(display, nullptr, nullptr);
    eglChooseConfig(display, surface_attribs, &config, 1, &numConfigs);

    context = eglCreateContext(display, config, nullptr, context_attribs);

    eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);
}

void engine_terminate() {
    eglDestroyContext(display, context);
    eglTerminate(display);

//    close(fd[0]);
//    close(fd[1]);
}

void engine_init_surface(EGLNativeWindowType window) {
    surface = eglCreateWindowSurface(display, config, window, nullptr);

    EGLint width, height;
    eglQuerySurface(display, surface, EGL_WIDTH, &width);
    eglQuerySurface(display, surface, EGL_HEIGHT, &height);

    ANativeWindow_setBuffersGeometry(window, 0, 0, format);
}

void engine_destroy_surface() {
    eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    eglDestroySurface(display, surface);
    rendering = false;
}

void engine_make_current() {
    eglMakeCurrent(display, surface, surface, context);
    rendering = true;
}

void engine_swap_buffer() {
    eglSwapBuffers(display, surface);
}

void engine_dispatch_event(engine_event event, bool wait) {
    std::promise<void> promise;
    auto future = promise.get_future();
    if (std::unique_lock lock(mutex); lock) {
        m_events.emplace(std::move(promise), event);
    } else {
        return;
    }
    if (wait) {
        future.get();
    }
}

bool engine_pool_event(engine_event_t& event) {
    if (std::unique_lock lock(mutex, std::try_to_lock); lock) {
        if (!m_events.empty()) {
            event = std::move(m_events.front());
            m_events.pop();
            return true;
        }
    }
    return false;
}

void engine_handle_event(engine_event_t& event) {
    switch (event.event) {
        case engine_event::quit:
            quit = true;
            break;
        case engine_event::create_surface:
            engine_make_current();
            break;
        case engine_event::destroy_surface:
            engine_destroy_surface();
            break;
    }
    event.promise.set_value();
}

bool engine_render() {
    return rendering;
}