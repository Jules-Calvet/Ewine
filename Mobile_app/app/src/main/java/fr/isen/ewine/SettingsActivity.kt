package fr.isen.ewine

import android.content.Intent
import android.content.SharedPreferences
import android.graphics.Color
import android.graphics.drawable.Drawable
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import androidx.annotation.DrawableRes
import fr.isen.ewine.databinding.ActivitySettingsBinding

class SettingsActivity : AppCompatActivity() {
    private lateinit var binding : ActivitySettingsBinding
    override fun onCreate(savedInstanceState: Bundle?) {

        val sharedPref: SharedPreferences = getSharedPreferences("settings", 0)
        var darkMode = sharedPref.getBoolean("dark_mode", false)

        super.onCreate(savedInstanceState)

        binding = ActivitySettingsBinding.inflate(layoutInflater)
        setContentView(binding.root)

        mode(darkMode)

        binding.buttonMode.setOnClickListener {
            darkMode = !darkMode
            mode(darkMode)
            sharedPref.edit().putBoolean("dark_mode", darkMode).apply()
        }
    }

    private fun mode(darkMode : Boolean) {
        if (darkMode) {
            binding.root.setBackgroundColor(Color.BLACK)
            binding.buttonMode.setBackgroundResource(R.drawable.sun)
            binding.buttonMode.foreground = getDrawable(R.drawable.sun)
        } else {
            binding.root.setBackgroundColor(Color.WHITE)
            binding.buttonMode.setBackgroundResource(R.drawable.moon)
            binding.buttonMode.foreground = getDrawable(R.drawable.moon)
        }
    }
}