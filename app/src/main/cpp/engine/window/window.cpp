#include "window.hpp"

#include "../module.hpp"

namespace {
    engine::module::handle<engine::window::Window> window;
}

void engine::window::close() {
    ::window->close();
}

bool engine::window::is_open() {
    return ::window->is_open();
}
