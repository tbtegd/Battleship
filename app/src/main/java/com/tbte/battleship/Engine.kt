package com.tbte.battleship

import android.view.Surface

object Engine {
    init {
        System.loadLibrary("battleship")
    }

    @JvmStatic external fun onCreate()
    @JvmStatic external fun onStart()
    @JvmStatic external fun onResume()
    @JvmStatic external fun onPause()
    @JvmStatic external fun onStop()
    @JvmStatic external fun onDestroy()
    @JvmStatic external fun onSurfaceTextureAvailable(surface: Surface?)
    @JvmStatic external fun onSurfaceTextureDestroyed()
}