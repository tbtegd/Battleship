#include "graphics.hpp"
#include "window/window.hpp"
#include "module.hpp"

namespace {
	engine::module::handle<engine::window::Window> window;
	engine::module::handle<engine::graphics::Graphics> graphics;
}

void engine::graphics::Graphics::setActive(bool enable) {
	active.store(enable);
}

bool engine::graphics::Graphics::isActive() {
	return active.load() && ::window->isOpen();
}

void engine::graphics::Graphics::present() {
	::window->swapBuffers();
}

engine::graphics::Shader engine::graphics::Graphics::newShader(ShaderSource vertex_source, ShaderSource fragment_source) {
	return Shader(vertex_source, fragment_source);
}

bool engine::graphics::isActive() {
	return ::graphics->isActive();
}

void engine::graphics::present() {
	::graphics->present();
}

engine::graphics::Shader engine::graphics::newShader(ShaderSource vertex_source, ShaderSource fragment_source) {
    return ::graphics->newShader(vertex_source, fragment_source);
}
