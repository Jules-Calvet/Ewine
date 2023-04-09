package fr.isen.ewine

import android.content.Intent
import android.content.SharedPreferences
import android.graphics.Color
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import com.google.gson.Gson
import fr.isen.ewine.databinding.ActivityBottleBinding
import fr.isen.ewine.model.CellarData
import fr.isen.ewine.model.UserData

class BottleActivity : AppCompatActivity() {
    private lateinit var binding : ActivityBottleBinding
    override fun onCreate(savedInstanceState: Bundle?) {

        super.onCreate(savedInstanceState)

        binding = ActivityBottleBinding.inflate(layoutInflater)
        setContentView(binding.root)

        binding.buttonToSettings.setOnClickListener {
            val intent = Intent(this, SettingsActivity::class.java)
            startActivity(intent)
        }
    }
    override fun onStart() {
        super.onStart()

        val x = intent.getIntExtra("x", 0)
        val y = intent.getIntExtra("y", 0)
        val gson = Gson()
        val sharedPref: SharedPreferences = getSharedPreferences("settings", 0)
        val darkMode = sharedPref.getBoolean("dark_mode", false)
        val jsonFromPrefs = sharedPref.getString("tab_cellar", null)
        if(jsonFromPrefs != null) {
            val tabCellar = gson.fromJson(jsonFromPrefs, /*Array<Array<*/UserData/*>>*/::class.java)
            binding.BottleTypeOfWine.text = tabCellar/*[x][y]*/.cellarData[x+(y*sharedPref.getInt("width",0))].bottleTypeOfWine
        }
        mode(darkMode)
    }
    private fun mode(darkMode: Boolean) {
        if (darkMode) {
            binding.root.setBackgroundColor(Color.BLACK)
            binding.BottleTypeOfWine.setTextColor(Color.WHITE)
        } else {
            binding.root.setBackgroundColor(Color.WHITE)
            binding.BottleTypeOfWine.setTextColor(Color.BLACK)
        }
    }
}