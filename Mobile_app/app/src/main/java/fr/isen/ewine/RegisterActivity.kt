package fr.isen.ewine

import android.content.SharedPreferences
import android.graphics.Color
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import fr.isen.ewine.databinding.ActivityRegisterBinding

class RegisterActivity : AppCompatActivity() {
    lateinit var binding : ActivityRegisterBinding

    override fun onCreate(savedInstanceState: Bundle?) {

        val sharedPref: SharedPreferences = getSharedPreferences("settings", 0)
        var darkMode = sharedPref.getBoolean("dark_mode", false)

        super.onCreate(savedInstanceState)

        binding = ActivityRegisterBinding.inflate(layoutInflater)
        setContentView(binding.root)

        mode(darkMode)
    }

    private fun mode(darkMode : Boolean) {
        if (darkMode) {
            binding.root.setBackgroundColor(Color.BLACK)
        } else {
            binding.root.setBackgroundColor(Color.WHITE)
        }
    }
}