package fr.isen.ewine

import android.content.Intent
import android.content.SharedPreferences
import android.graphics.Color
import android.graphics.drawable.Drawable
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.widget.Toast
import androidx.annotation.DrawableRes
import fr.isen.ewine.databinding.ActivitySettingsBinding

class SettingsActivity : AppCompatActivity() {
    private lateinit var binding : ActivitySettingsBinding
    override fun onCreate(savedInstanceState: Bundle?) {

        val sharedPref: SharedPreferences = getSharedPreferences("settings", 0)
        var darkMode = sharedPref.getBoolean("dark_mode", false)
        var cellar_height = sharedPref.getInt("height",1)
        var cellar_width = sharedPref.getInt("width",1)

        super.onCreate(savedInstanceState)

        binding = ActivitySettingsBinding.inflate(layoutInflater)
        setContentView(binding.root)

        mode(darkMode)

        binding.numberColumns.text = "$cellar_width"
        binding.numberRows.text = "$cellar_height"
        binding.ButtonMinusColumns.setOnClickListener{
            if (cellar_width > 1){
                cellar_width --
                binding.numberColumns.text = "$cellar_width"
                sharedPref.edit().putInt("width", cellar_width).apply()
            }
            else {
                Toast.makeText(baseContext, "Warning! You cannot have less than 1 columns in your cellar ! ",Toast.LENGTH_SHORT).show()
            }
        }
        binding.ButtonPlusColumns.setOnClickListener{
            cellar_width ++
            binding.numberColumns.text = "$cellar_width"
            sharedPref.edit().putInt("width", cellar_width).apply()
        }
        binding.ButtonMinusRows.setOnClickListener{
            if (cellar_height > 1){
                cellar_height --
                binding.numberRows.text = "$cellar_height"
                sharedPref.edit().putInt("height", cellar_height).apply()
            }
            else {
                Toast.makeText(baseContext, "Warning! You cannot have less than 1 rows in your cellar ! ",Toast.LENGTH_SHORT).show()
            }
        }
        binding.ButtonPlusRows.setOnClickListener{
            cellar_height ++
            binding.numberRows.text = "$cellar_height"
            sharedPref.edit().putInt("height", cellar_height).apply()
        }




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