#include <android/native_window_jni.h>
#include <android/native_window.h>
#include <mutex>
#include <queue>
#include <optional>
#include <thread>

#include <iostream>

#include <graphics/graphics.hpp>
#include <common/module.hpp>
#include <event/event.hpp>
#include <window/window.hpp>
#include <unistd.h>
#include <engine/system/system.hpp>
#include <android/log.h>

namespace {
    engine::module::handle<engine::event::Event> event;
    engine::module::handle<engine::window::Window> window;
    engine::module::handle<engine::graphics::Graphics> graphics;

	std::unique_ptr<std::thread> m_thread;

	extern "C" int main();

    inline void engine_main() {
        window->makeCurrent();
        main();
        window->destroy();
    }
}

extern "C" {
    JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void*) {
        if (engine::system::event::init() < 0) {
            return JNI_ERR;
        }

        return JNI_VERSION_1_6;
    }

    JNIEXPORT void JNI_OnUnload(JavaVM* vm, void* reserved) {
        engine::system::event::quit();
    }

    JNIEXPORT void JNICALL Java_com_tbte_battleship_Engine_onCreate(JNIEnv *env, jclass) {
        window->create();
    }

    JNIEXPORT void JNICALL Java_com_tbte_battleship_Engine_onStart(JNIEnv *env, jclass) {
    }

    JNIEXPORT void JNICALL Java_com_tbte_battleship_Engine_onResume(JNIEnv *env, jclass) {
    }

    JNIEXPORT void JNICALL Java_com_tbte_battleship_Engine_onPause(JNIEnv *env, jclass) {
    }

    JNIEXPORT void JNICALL Java_com_tbte_battleship_Engine_onStop(JNIEnv *env, jclass) {
    }

    JNIEXPORT void JNICALL Java_com_tbte_battleship_Engine_onDestroy(JNIEnv *env, jclass) {
        event->clear();
        window->close();

        m_thread->join();
        m_thread.reset();
    }

    JNIEXPORT void JNICALL Java_com_tbte_battleship_Engine_surfaceCreated(JNIEnv *env, jclass, jobject surface) {
        window->createSurface(ANativeWindow_fromSurface(env, surface));

        if (m_thread == nullptr) {
            m_thread = std::make_unique<std::thread>(engine_main);
        } else {
            std::promise<void> promise{};
            engine::system::event::Message message{
                    .type = engine::system::event::Event::SURFACE_CREATED,
                    .data = &promise
            };
            engine::system::event::push(message);
            promise.get_future().get();
        }
    }

    JNIEXPORT void JNICALL Java_com_tbte_battleship_Engine_surfaceChanged(JNIEnv *env, jclass, jobject surface, jint /*format*/, jint width, jint height) {
        window->updateSurface();
    }

    JNIEXPORT void JNICALL Java_com_tbte_battleship_Engine_surfaceDestroyed(JNIEnv *env, jclass, jobject surface) {
        std::promise<void> promise{};
        engine::system::event::Message message{
                .type = engine::system::event::Event::SURFACE_DESTROYED,
                .data = &promise
        };
        engine::system::event::push(message);
        promise.get_future().get();
    }
}