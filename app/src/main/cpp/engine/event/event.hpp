#pragma once

#include <future>
#include <vector>
#include <any>
#include <string_view>
#include <initializer_list>
#include <queue>

namespace engine::event {
    struct Message {
        int type = 0;
        explicit constexpr Message() = default;
        explicit constexpr Message(int type) : type(type) {}
    };

	struct Event {
		Event();
		~Event();

		enum : int { Quit };
		void push(int type);
		bool poll(Message& message);
		void clear();

	private:
		std::mutex mutex;
		std::queue<Message> queue{};
	};

	extern void push(int type);
	extern void pump();
    extern bool poll(Message& message);
    extern void handle(Message& message);
    extern void quit();
    extern void clear();
}