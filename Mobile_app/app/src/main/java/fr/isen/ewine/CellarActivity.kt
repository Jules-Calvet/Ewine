package fr.isen.ewine

import android.content.Context
import android.content.Intent
import android.content.SharedPreferences
import android.graphics.Color
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import androidx.recyclerview.widget.LinearLayoutManager
import com.google.gson.Gson
import fr.isen.ewine.databinding.ActivityCellarBinding

class CellarActivity : AppCompatActivity() {
    private lateinit var binding: ActivityCellarBinding
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivityCellarBinding.inflate(layoutInflater)
        setContentView(binding.root)

        binding.buttonToSettings.setOnClickListener {
            val intent = Intent(this, SettingsActivity::class.java)
            startActivity(intent)
        }
    }

    override fun onStart() {
        super.onStart()
        val gson = Gson()
        val sharedPref: SharedPreferences = getSharedPreferences("settings", 0)
        var darkMode = sharedPref.getBoolean("dark_mode", false)
        var cellar_height = sharedPref.getInt("height",1)
        var cellar_width = sharedPref.getInt("width",1)
        val jsonFromPrefs = sharedPref.getString("tab_cellar", "zero")
        binding.rowsRecyclerView.layoutManager = LinearLayoutManager(this)

        if(jsonFromPrefs != null){
            val tab_cellar = gson.fromJson(jsonFromPrefs, Array<Array<String>>::class.java)
            binding.rowsRecyclerView.adapter = RowsAdapter(this,cellar_height,cellar_width,tab_cellar)
        }
        mode(darkMode)
    }

    private fun mode(darkMode: Boolean) {
        if (darkMode) {
            binding.root.setBackgroundColor(Color.BLACK)
            binding.buttonToSettings.setBackgroundResource(R.drawable.parameter_dark)
            binding.buttonToSettings.foreground = getDrawable(R.drawable.parameter_dark)
        } else {
            binding.root.setBackgroundColor(Color.WHITE)
            binding.buttonToSettings.setBackgroundResource(R.drawable.parameter)
            binding.buttonToSettings.foreground = getDrawable(R.drawable.parameter)
        }
    }
}