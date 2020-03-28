#pragma once

#include <type_traits>
#include <new>

namespace engine::module {
	template <typename T>
	struct handle {
		inline T* operator ->() const noexcept {
			return m_handle.operator->();
		}

	private:
		struct storage {
			storage() = default;
			storage(const storage &) = delete;
			storage &operator=(const storage &) = delete;
			storage(storage &&) = delete;
			storage &operator=(storage &&) = delete;

			~storage() {
				if (m_constructed) {
					m_constructed = false;

					reinterpret_cast<T *>(&m_storage)->~T();
				}
			}

			inline T *operator->() noexcept {
				if (!m_constructed) {
					m_constructed = true;
					new (&m_storage) T();
				}
				return reinterpret_cast<T *>(&m_storage);
			}

		private:
			bool m_constructed = false;
			std::aligned_storage_t<sizeof(T), alignof(T)> m_storage;
		};

    	inline static storage m_handle;
	};
}

