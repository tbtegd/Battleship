#pragma once

#include <atomic>

namespace engine {
    class Object {
    public:
        Object() : count(1) {}
        Object(const Object &other) = delete;
        Object& operator=(const Object &other) = delete;
        Object(Object &&other) = delete;
        Object& operator=(Object &&other) = delete;
        virtual ~Object() = default;

        int getReferenceCount() const {
            return count;
        }
        void retain() {
            count.fetch_add(1, std::memory_order_relaxed);
        }
        void release() {
            if (count.fetch_sub(1, std::memory_order_release) == 1) {
                std::atomic_thread_fence(std::memory_order_acquire);
                delete this;
            }
        }

    private:
        std::atomic<int> count;
    };

    enum class Acquire {
        RETAIN,
        NORETAIN,
    };

    template <typename T>
    class StrongRef {
    public:
        constexpr StrongRef() noexcept : object(nullptr) {}

        StrongRef(T *obj, Acquire acquire = Acquire::RETAIN) : object(obj) {
            if (object && (acquire == Acquire::RETAIN)) {
                object->retain();
            }
        }

        StrongRef(const StrongRef &other) : object(other.object) {
            if (object) {
                object->retain();
            }
        }

        StrongRef &operator =(const StrongRef &other) {
            set(other.object);
            return *this;
        }

        constexpr StrongRef(StrongRef &&other) noexcept : object(other.object) {
            other.object = nullptr;
        }
        constexpr StrongRef &operator =(StrongRef &&other) noexcept {
            object = other.object;
            other.object = nullptr;
            return *this;
        }

        ~StrongRef() {
            release();
        }

        inline constexpr T *operator->() const noexcept {
            return object;
        }

        inline constexpr operator T*() const noexcept {
            return object;
        }

        inline constexpr explicit operator bool() const noexcept {
            return object != nullptr;
        }

        inline constexpr T *get() const noexcept {
            return object;
        }

        inline void set(T *obj, Acquire acquire = Acquire::RETAIN) {
            if (obj && (acquire == Acquire::RETAIN)) {
                obj->retain();
            }
            release();
            object = obj;
        }
    private:
        inline void release() {
            if (object) {
                object->release();
            }
        }
        T *object;
    };
}