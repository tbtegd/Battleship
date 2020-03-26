package com.tbte.battleship

import android.app.Activity
import android.os.Bundle
import android.view.SurfaceHolder
import android.view.SurfaceView

class MainActivity : Activity(), SurfaceHolder.Callback {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        val surface = SurfaceView(this)
        surface.holder.addCallback(this)
        setContentView(surface)

        System.loadLibrary("battleship")

        Engine.onCreate()
    }

    override fun onStart() {
        super.onStart()
        Engine.onStart();
    }

    override fun onResume() {
        super.onResume()
        Engine.onResume();
    }

    override fun onPause() {
        Engine.onPause()
        super.onPause()
    }

    override fun onStop() {
        Engine.onStop()
        super.onStop()
    }

    override fun onDestroy() {
        super.onDestroy()
        Engine.onDestroy()
    }

    override fun surfaceCreated(holder: SurfaceHolder?) {
        Engine.surfaceCreated(holder!!.surface)
    }

    override fun surfaceChanged(holder: SurfaceHolder?, format: Int, width: Int, height: Int) {
        Engine.surfaceChanged(holder!!.surface, format, width, height);
    }

    override fun surfaceDestroyed(holder: SurfaceHolder?) {
        Engine.surfaceDestroyed(holder!!.surface)
    }
}