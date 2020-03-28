#pragma once

namespace engine::system {
    namespace event {
        enum class Event {
            SURFACE_CREATED,
            SURFACE_DESTROYED,
        };

        struct Message {
            Event type;
            union {
                struct {
                } window;
            };
            void* data = nullptr;
        };

        extern int init();
        extern void quit();
        extern void push(Message message);
        extern bool poll(Message& message);
    }
}

