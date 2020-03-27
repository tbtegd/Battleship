#include "graphics.hpp"
#include "window/window.hpp"
#include "module.hpp"

namespace {
	engine::module::handle<engine::window::Window> window;
	engine::module::handle<engine::graphics::Graphics> graphics;
}

void engine::graphics::Graphics::set_active(bool enable) {
	active.store(enable);
}

bool engine::graphics::Graphics::is_active() {
	return active.load() && ::window->is_open();
}

void engine::graphics::Graphics::present() {
	::window->swapBuffers();
}

bool engine::graphics::is_active() {
	return ::graphics->is_active();
}

void engine::graphics::present() {
	::graphics->present();
}
