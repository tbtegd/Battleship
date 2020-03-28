#include "event.hpp"

#include <mutex>
#include <queue>

#include "common/module.hpp"
#include "window/window.hpp"
#include "graphics/graphics.hpp"

#include <string_view>
#include <unistd.h>
#include <iostream>

#include "system/system.hpp"

using namespace std::string_view_literals;

namespace {
    engine::module::handle<engine::event::Event> event;
    engine::module::handle<engine::window::Window> window;
    engine::module::handle<engine::graphics::Graphics> graphics;
}

engine::event::Event::Event() {
//    if (engine::system::event::init() < 0) {
//        std::__throw_runtime_error(/*"Could not initialize events subsystem (%s)",*/strerror(errno));
//    }
}

engine::event::Event::~Event() {
//    engine::system::event::quit();
}

void engine::event::Event::push(int type) {
    std::unique_lock lock(mutex);
    queue.emplace(type);
}

bool engine::event::Event::poll(Message& message) {
    std::unique_lock lock(mutex);
    if (queue.empty()) {
        return false;
    }
    message = queue.front();
    queue.pop();
    return true;
}

void engine::event::Event::clear() {
    std::unique_lock lock(mutex);
    while (!queue.empty()) {
        queue.pop();
    }
}

void engine::event::push(int type/*, std::vector<std::any> &&args*/) {
    ::event->push(type/*, std::forward<std::vector<std::any>>(args)*/);
}

void engine::event::pump() {
    engine::system::event::Message message{};
    while (engine::system::event::poll(message)) {
        switch (message.type) {
            case system::event::Event::SURFACE_CREATED:
                ::window->makeCurrent();
                ::graphics->setActive(true);
                reinterpret_cast<std::promise<void>*>(message.data)->set_value();
                break;
            case system::event::Event::SURFACE_DESTROYED:
                ::graphics->setActive(false);
                ::window->destroySurface();
                reinterpret_cast<std::promise<void>*>(message.data)->set_value();
                break;
        }
    }
}

//handle events
bool engine::event::poll(Message& message) {
    return ::event->poll(message);
}

void engine::event::handle(Message& message) {
    switch (message.type) {
        case Event::Quit:
            ::window->close();
            break;
    }
}

void engine::event::quit() {
    ::event->push(Event::Quit);
}

void engine::event::clear() {
    ::event->clear();
}
