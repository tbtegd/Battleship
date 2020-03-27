#include <android/native_window_jni.h>
#include <android/native_window.h>
#include <mutex>
#include <queue>
#include <optional>
#include <thread>

#include <iostream>

#include "module.hpp"

#include "event/event.hpp"
#include "window/window.hpp"

namespace {
	engine::module::handle<engine::window::Window> window;

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
        return JNI_VERSION_1_6;
    }

    JNIEXPORT void JNI_OnUnload(JavaVM* vm, void* reserved) {
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
        engine::event::push(engine::event::Event::quit, false);

        m_thread->join();
        m_thread.reset();
    }

    JNIEXPORT void JNICALL Java_com_tbte_battleship_Engine_surfaceCreated(JNIEnv *env, jclass, jobject surface) {
        window->createSurface(ANativeWindow_fromSurface(env, surface));

        if (m_thread == nullptr) {
            m_thread = std::make_unique<std::thread>(engine_main);
        } else {
            engine::event::push(engine::event::Event::create_surface, true);
        }
    }

    JNIEXPORT void JNICALL Java_com_tbte_battleship_Engine_surfaceChanged(JNIEnv *env, jclass, jobject surface, jint format, jint width, jint height) {
    }

    JNIEXPORT void JNICALL Java_com_tbte_battleship_Engine_surfaceDestroyed(JNIEnv *env, jclass, jobject surface) {
        engine::event::push(engine::event::Event::destroy_surface, true);
    }
}