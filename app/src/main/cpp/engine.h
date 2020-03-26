#pragma once

#include <EGL/egl.h>
#include <EGL/eglext.h>

#include <future>

enum class engine_event {
    quit,
    create_surface,
    destroy_surface,
};

struct engine_event_t {
    std::promise<void> promise;
    engine_event event;

    explicit engine_event_t() = default;
    engine_event_t(std::promise<void>&& promise, engine_event event)
        : promise(std::forward<std::promise<void>>(promise))
        , event(event) {}
};

extern bool engine_request_quit();
extern void engine_initialize();
extern void engine_terminate();
extern void engine_init_surface(EGLNativeWindowType window);
extern void engine_destroy_surface();
extern void engine_make_current();
extern void engine_swap_buffer();
extern void engine_dispatch_event(engine_event event, bool wait);
extern bool engine_pool_event(engine_event_t& event);
extern void engine_handle_event(engine_event_t& event);
extern bool engine_render();