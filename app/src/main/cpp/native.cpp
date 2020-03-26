#include <jni.h>
#include <string>

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <android/native_window_jni.h>

#include <GLES3/gl3.h>

#include <thread>
#include <mutex>
#include <condition_variable>

#include <android/log.h>

#include <atomic>
#include <queue>
#include "engine.h"

#include <future>
#include <thread>

static_assert(std::atomic<bool>::is_always_lock_free);

std::unique_ptr<std::thread> m_thread;

void render_thread();

extern "C" {
    JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void*) {
        return JNI_VERSION_1_6;
    }

    JNIEXPORT void JNI_OnUnload(JavaVM* vm, void* reserved) {
    }

    JNIEXPORT void JNICALL Java_com_tbte_battleship_Engine_onCreate(JNIEnv *env, jclass) {
        engine_initialize();
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
        engine_dispatch_event(engine_event::quit, false);
        m_thread->join();
        m_thread.reset();

        engine_terminate();
    }

    JNIEXPORT void JNICALL Java_com_tbte_battleship_Engine_surfaceCreated(JNIEnv *env, jclass, jobject surface) {
        engine_init_surface(ANativeWindow_fromSurface(env, surface));

        if (m_thread == nullptr) {
            m_thread = std::make_unique<std::thread>(render_thread);
        }

        engine_dispatch_event(engine_event::create_surface, true);
    }

    JNIEXPORT void JNICALL Java_com_tbte_battleship_Engine_surfaceChanged(JNIEnv *env, jclass, jobject surface, jint format, jint width, jint height) {

    }

    JNIEXPORT void JNICALL Java_com_tbte_battleship_Engine_surfaceDestroyed(JNIEnv *env, jclass, jobject surface) {
        engine_dispatch_event(engine_event::destroy_surface, true);
    }
}

void render_thread() {
    engine_event_t event;

    while (true) {
        while (engine_pool_event(event)) {
            engine_handle_event(event);
        }
        if (engine_request_quit()) {
            break;
        }

        if (engine_render()) {
            glClearColor(1, 1, 1, 1);
            glClear(GL_COLOR_BUFFER_BIT);

            engine_swap_buffer();
        }
        std::this_thread::yield();
    }
}