package fr.isen.ewine

import android.annotation.SuppressLint
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
        val darkMode = sharedPref.getBoolean("dark_mode", false)
        val cellarHeight = sharedPref.getInt("height",3)
        val cellarWidth = sharedPref.getInt("width",3)
        val jsonFromPrefs = sharedPref.getString("tab_cellar", "")
        binding.rowsRecyclerView.layoutManager = LinearLayoutManager(this)
        if(jsonFromPrefs != null){
            val tabCellar = gson.fromJson(jsonFromPrefs, Array<Array<String>>::class.java)
            binding.rowsRecyclerView.adapter = RowsAdapter(cellarHeight,cellarWidth,tabCellar)
            var bottleCount = 0
            for (x in 0 until cellarWidth) {
                for(y in 0 until cellarHeight){
                    if(tabCellar[x][y] != ""){
                        bottleCount ++
                    }
                }
            }
            val totalNumber = cellarHeight*cellarWidth
            binding.bottleNumber.text = "Bottles : $bottleCount / $totalNumber"
        }
        mode(darkMode)
    }

    @SuppressLint("UseCompatLoadingForDrawables")
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