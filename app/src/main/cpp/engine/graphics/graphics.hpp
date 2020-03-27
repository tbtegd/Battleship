#pragma once

#include <atomic>

namespace engine::graphics {
	struct Graphics {
		void set_active(bool enable);
		bool is_active();
		void present();

	private:
		std::atomic_bool active{true};
	};

	extern bool is_active();
	extern void present();
}