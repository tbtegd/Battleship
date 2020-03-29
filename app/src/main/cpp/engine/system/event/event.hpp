#pragma once

namespace engine::system::event {
    enum class Event {
        Quit,
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
