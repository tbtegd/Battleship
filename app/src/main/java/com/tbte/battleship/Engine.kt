package com.tbte.battleship

import android.view.Surface

object Engine {
    @JvmStatic external fun onCreate()
    @JvmStatic external fun onStart()
    @JvmStatic external fun onResume()
    @JvmStatic external fun onPause()
    @JvmStatic external fun onStop()
    @JvmStatic external fun onDestroy()
    @JvmStatic external fun surfaceCreated(surface: Surface?)
    @JvmStatic external fun surfaceChanged(surface: Surface?, format: Int, width: Int, height: Int)
    @JvmStatic external fun surfaceDestroyed(surface: Surface?)
}