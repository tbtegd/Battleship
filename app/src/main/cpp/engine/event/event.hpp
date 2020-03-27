#pragma once

#include <future>

namespace engine::event {
    struct Event {
    	using Promise = std::promise<void>;
        enum Type {
            quit,
            create_surface,
            destroy_surface,
        };

        Promise promise;
        Type type;

        explicit Event() = default;
        Event(Promise&& promise, Type type) : promise(std::forward<Promise>(promise)), type(type) {}
    };

	extern void push(Event::Type type, bool wait);
	extern void pump();
    extern bool poll(Event& event);
    extern void handle(Event& event);
    extern void quit();
}