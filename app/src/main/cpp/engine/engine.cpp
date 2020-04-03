#include <system/event/event.hpp>

#include <android/native_window_jni.h>
#include <EGL/egl.h>
#include <pthread.h>
#include <cerrno>
#include <cstring>
#include <engine/graphics/graphics.hpp>

extern void android_main();

inline pthread_t m_thread;
inline NativeWindowType window;
inline jclass Class;

inline void* start_routine(void*) {
	android_main();
	return nullptr;
}

extern "C" {
    NativeWindowType ANativeWindow_getInstance() {
        return window;
    }

	JNIEXPORT void JNICALL Java_com_tbte_battleship_Engine_onCreate(JNIEnv *env, jclass clazz) {
        engine::graphics::setActive(true);
    }
    JNIEXPORT void JNICALL Java_com_tbte_battleship_Engine_onStart(JNIEnv *env, jclass clazz) {
        engine::graphics::setActive(true);
    }
    JNIEXPORT void JNICALL Java_com_tbte_battleship_Engine_onResume(JNIEnv *env, jclass clazz) {
        engine::graphics::setActive(true);
    }
    JNIEXPORT void JNICALL Java_com_tbte_battleship_Engine_onPause(JNIEnv *env, jclass clazz) {
        engine::graphics::setActive(false);
    }
    JNIEXPORT void JNICALL Java_com_tbte_battleship_Engine_onStop(JNIEnv *env, jclass clazz) {
        engine::graphics::setActive(false);
    }
    JNIEXPORT void JNICALL Java_com_tbte_battleship_Engine_onDestroy(JNIEnv *env, jclass clazz) {
        engine::graphics::setActive(false);
    }
    JNIEXPORT void JNICALL Java_com_tbte_battleship_Engine_onSurfaceTextureAvailable(JNIEnv *env, jclass, jobject surface) {
        if (engine::system::event::init() < 0) {
            env->ThrowNew(env->FindClass( "java/lang/RuntimeException"), strerror(errno));
            return;
        }

        window = ANativeWindow_fromSurface(env, surface);

        pthread_create(&m_thread, nullptr, start_routine, nullptr);
    }

    JNIEXPORT void JNICALL Java_com_tbte_battleship_Engine_onSurfaceTextureDestroyed(JNIEnv *env, jclass clazz) {
        engine::system::event::Message message{
                .type = engine::system::event::Event::Quit
        };
        engine::system::event::push(message);

        pthread_join(m_thread, nullptr);

        engine::system::event::quit();
    }

    JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void*) {
        return JNI_VERSION_1_6;
    }
    JNIEXPORT void JNI_OnUnload(JavaVM* vm, void*) {
    }
}