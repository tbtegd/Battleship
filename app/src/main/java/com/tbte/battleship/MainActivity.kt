package com.tbte.battleship

import android.app.Activity
import android.graphics.SurfaceTexture
import android.os.Bundle
import android.view.Surface
import android.view.TextureView
import kotlin.system.exitProcess

class MainActivity : Activity(), TextureView.SurfaceTextureListener {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        val view = TextureView(this)
        view.surfaceTextureListener = this
        setContentView(view)
        Engine.onCreate(assets)
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

        exitProcess(0)
    }

    override fun onSurfaceTextureAvailable(surface: SurfaceTexture?, width: Int, height: Int) {
        Engine.onSurfaceTextureAvailable(Surface(surface))
    }

    override fun onSurfaceTextureSizeChanged(surface: SurfaceTexture?, width: Int, height: Int) {
//        Engine.surfaceChanged(surface, width, height)
    }

    override fun onSurfaceTextureUpdated(surface: SurfaceTexture?) {
    }

    override fun onSurfaceTextureDestroyed(surface: SurfaceTexture?): Boolean {
        Engine.onSurfaceTextureDestroyed()
        return true;
    }
}