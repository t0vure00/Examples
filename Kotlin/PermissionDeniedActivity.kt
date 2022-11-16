package com.example.viittomatulkki

import android.content.Intent
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.widget.Button

class PermissionDeniedActivity : AppCompatActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_permission_denied)
        oClickButtonListener()
    }


    fun oClickButtonListener() {
        val button_back = findViewById<Button>(R.id.button_back)
        button_back!!.setOnClickListener {
            this.finish()
        }
    }

}