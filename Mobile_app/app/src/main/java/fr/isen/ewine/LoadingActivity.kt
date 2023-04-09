package fr.isen.ewine

import android.content.Intent
import android.content.SharedPreferences
import android.graphics.Color
import android.net.Uri
import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import androidx.appcompat.app.AppCompatDelegate
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

        /*binding.videoLoading.setVideoURI(Uri.parse("android.resource://res/" + R.raw.loadinglight02))
        binding.videoLoading.start()*/
        mode(darkMode)

        Timer().schedule(4000) {
            val intent = Intent(this@LoadingActivity, LoginActivity::class.java)
            startActivity(intent)
            finish()
        }
    }

    private fun mode(darkMode : Boolean) {
        if (darkMode) {
            AppCompatDelegate.setDefaultNightMode(AppCompatDelegate.MODE_NIGHT_YES)
            binding.root.setBackgroundColor(Color.BLACK)
            binding.imageLogo.setImageResource(R.drawable.logo_dark)
        } else {
            AppCompatDelegate.setDefaultNightMode(AppCompatDelegate.MODE_NIGHT_NO)
            binding.root.setBackgroundColor(Color.WHITE)
            binding.imageLogo.setImageResource(R.drawable.logo)
        }
    }
}