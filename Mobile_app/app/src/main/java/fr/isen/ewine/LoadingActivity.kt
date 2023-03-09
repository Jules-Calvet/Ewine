package fr.isen.ewine

import android.content.Intent
import android.content.SharedPreferences
import android.graphics.Color
import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import fr.isen.ewine.databinding.ActivityLoadingBinding
import java.util.*
import kotlin.concurrent.schedule


class LoadingActivity : AppCompatActivity() {
    private lateinit var binding : ActivityLoadingBinding
    override fun onCreate(savedInstanceState: Bundle?) {

        val sharedPref: SharedPreferences = getSharedPreferences("settings", 0)
        val darkMode = sharedPref.getBoolean("dark_mode", false)

        super.onCreate(savedInstanceState)

        binding = ActivityLoadingBinding.inflate(layoutInflater)
        setContentView(binding.root)

        mode(darkMode)

        Timer().schedule(4000) {
            val intent = Intent(this@LoadingActivity, LoginActivity::class.java)
            startActivity(intent)
            finish()
        }
    }

    private fun mode(darkMode : Boolean) {
        if (darkMode) {
            binding.root.setBackgroundColor(Color.BLACK)
            binding.imageLogo.setImageResource(R.drawable.logo_dark)
        } else {
            binding.root.setBackgroundColor(Color.WHITE)
            binding.imageLogo.setImageResource(R.drawable.logo)
        }
    }
}