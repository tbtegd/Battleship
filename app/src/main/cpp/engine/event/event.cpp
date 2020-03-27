#include "event.hpp"

#include <mutex>
#include <queue>

#include "module.hpp"
#include "window/window.hpp"
#include "graphics/graphics.hpp"

namespace {
	engine::module::handle<engine::window::Window> window;
	engine::module::handle<engine::graphics::Graphics> graphics;
	std::queue<engine::event::Event> events{};
	std::mutex mutex;
}

void engine::event::push(Event::Type type, bool wait) {
    Event::Promise promise;
	auto future = promise.get_future();
	if (std::unique_lock lock(mutex); lock) {
		events.emplace(std::move(promise), type);
	} else {
		return;
	}
	if (wait) {
		future.get();
	}
}

void engine::event::pump() {
}

//handle events
bool engine::event::poll(Event &event) {
	std::unique_lock lock(mutex, std::try_to_lock);
	if (lock && !events.empty()) {
		event = std::move(events.front());
		events.pop();
		return true;
	}
    return false;
}

//handle events
void engine::event::handle(Event& event) {
	switch (event.type) {
		case engine::event::Event::quit:
			::window->close();
			break;
		case engine::event::Event::create_surface:
			::window->makeCurrent();
			::graphics->set_active(true);
			break;
		case engine::event::Event::destroy_surface:
			::graphics->set_active(false);
			::window->destroySurface();
			break;
	}
	event.promise.set_value();
}