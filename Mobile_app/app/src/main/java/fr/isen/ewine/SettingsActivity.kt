package fr.isen.ewine

import android.content.SharedPreferences
import android.graphics.Color
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.util.Log
import android.widget.Toast
import com.google.gson.Gson
import fr.isen.ewine.databinding.ActivitySettingsBinding

class SettingsActivity : AppCompatActivity() {
    private lateinit var binding : ActivitySettingsBinding
    override fun onCreate(savedInstanceState: Bundle?) {

        val sharedPref: SharedPreferences = getSharedPreferences("settings", 0)
        var darkMode = sharedPref.getBoolean("dark_mode", false)
        var cellar_height = sharedPref.getInt("height",1)
        var cellar_width = sharedPref.getInt("width",1)
        val gson = Gson()
        var json: String
        val jsonFromPrefs = sharedPref.getString("tab_cellar", "")
        if(jsonFromPrefs == ""){
            var tab = Array(cellar_width) { Array(cellar_height) {""} }
            var json_empty = gson.toJson(tab)
            sharedPref.edit().putString("tab_cellar", json_empty).apply()
        }
        var tab_cellar = gson.fromJson(jsonFromPrefs, Array<Array<String>>::class.java)
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
                val tab_temp = tab_cellar
                tab_cellar = Array(cellar_width) { Array(cellar_height) {""} }
                for(x in 0..cellar_width-1) {
                    for(y in 0..cellar_height-1) {
                        tab_cellar[x][y] = tab_temp[x][y]
                    }
                }
                json = gson.toJson(tab_cellar)
                sharedPref.edit().putString("tab_cellar", json).apply()
            }
            else {
                Toast.makeText(baseContext, "Warning! You cannot have less than 1 columns in your cellar ! ",Toast.LENGTH_SHORT).show()
            }
        }
        binding.ButtonPlusColumns.setOnClickListener{
            cellar_width ++
            binding.numberColumns.text = "$cellar_width"
            sharedPref.edit().putInt("width", cellar_width).apply()
            val tab_temp = tab_cellar
            tab_cellar = Array(cellar_width) { Array(cellar_height) {""} }
            for(x in 0..cellar_width-2) {
                for(y in 0..cellar_height-1) {
                    tab_cellar[x][y] = tab_temp[x][y]
                }
            }
            json = gson.toJson(tab_cellar)
            sharedPref.edit().putString("tab_cellar", json).apply()
        }
        binding.ButtonMinusRows.setOnClickListener{
            if (cellar_height > 1){
                cellar_height --
                binding.numberRows.text = "$cellar_height"
                sharedPref.edit().putInt("height", cellar_height).apply()
                val tab_temp = tab_cellar
                tab_cellar = Array(cellar_width) { Array(cellar_height) {""} }
                for(x in 0..cellar_width-1) {
                    for(y in 0..cellar_height-1) {
                        tab_cellar[x][y] = tab_temp[x][y]
                    }
                }
                json = gson.toJson(tab_cellar)
                sharedPref.edit().putString("tab_cellar", json).apply()
            }
            else {
                Toast.makeText(baseContext, "Warning! You cannot have less than 1 rows in your cellar ! ",Toast.LENGTH_SHORT).show()
            }
        }
        binding.ButtonPlusRows.setOnClickListener{
            cellar_height ++
            binding.numberRows.text = "$cellar_height"
            sharedPref.edit().putInt("height", cellar_height).apply()
            val tab_temp = tab_cellar
            tab_cellar = Array(cellar_width) { Array(cellar_height) {""} }
            for(x in 0..cellar_width-1) {
                for(y in 0..cellar_height-2) {
                    tab_cellar[x][y] = tab_temp[x][y]
                }
            }
            json = gson.toJson(tab_cellar)
            sharedPref.edit().putString("tab_cellar", json).apply()
        }

        binding.buttonMode.setOnClickListener {
            darkMode = !darkMode
            mode(darkMode)
            sharedPref.edit().putBoolean("dark_mode", darkMode).apply()
        }
        binding.buttonAddBottle.setOnClickListener {
            val char_x = binding.NumberX.text.toString()
            val x : Int
            val y : Int
            if(char_x != "") {
                x = char_x.toInt() - 1
            } else {
                x = -1
            }
            val char_y = binding.NumberY.text.toString()
            if(char_y != "") {
                y = char_y.toInt() - 1
            } else {
                y = -1
            }
            val wine = binding.WineType.text.toString().toLowerCase()
            if(wine != "") {
                if (x >= 0 && x <= cellar_width) {
                    if (y >= 0 && y <= cellar_height) {
                        if (tab_cellar[x][y] == "") {
                            when (wine) {
                                "red" -> {
                                    tab_cellar[x][y] = "Red"
                                    json = gson.toJson(tab_cellar)
                                    sharedPref.edit().putString("tab_cellar", json).apply()
                                    Toast.makeText(
                                        baseContext,
                                        "Bottle of " + wine + " added at this position : " + char_x + ", " + char_y + " !",
                                        Toast.LENGTH_SHORT
                                    ).show()
                                }
                                "white" -> {
                                    Log.w(" Avant ", "$jsonFromPrefs")
                                    tab_cellar[x][y] = "White"
                                    json = gson.toJson(tab_cellar)
                                    Log.w(" Après ", "$json")
                                    sharedPref.edit().putString("tab_cellar", json).apply()
                                    Toast.makeText(
                                        baseContext,
                                        "Bottle of " + wine + " added at this position : " + char_x + ", " + char_y + " !",
                                        Toast.LENGTH_SHORT
                                    ).show()
                                }
                                "rose" -> {
                                    tab_cellar[x][y] = "Rose"
                                    json = gson.toJson(tab_cellar)
                                    sharedPref.edit().putString("tab_cellar", json).apply()
                                    Toast.makeText(
                                        baseContext,
                                        "Bottle of " + wine + " added at this position : " + char_x + ", " + char_y + " !",
                                        Toast.LENGTH_SHORT
                                    ).show()
                                }
                                else -> {
                                    Toast.makeText(
                                        baseContext,
                                        "Warning ! " + wine + " is not a type of wine : red, white, or rose",
                                        Toast.LENGTH_SHORT
                                    ).show()
                                }
                            }
                        } else {
                            Toast.makeText(
                                baseContext,
                                "This place has already been occupied ! Choose an other one ! ",
                                Toast.LENGTH_SHORT
                            ).show()
                        }
                    } else {
                        Toast.makeText(
                            baseContext,
                            "Y not valid ! Must be between 1 and " + cellar_height + " !",
                            Toast.LENGTH_SHORT
                        ).show()
                    }
                } else {
                    Toast.makeText(
                        baseContext,
                        "X not valid ! Must be between 1 and " + cellar_width + " !",
                        Toast.LENGTH_SHORT
                    ).show()
                }
            } else {
                Toast.makeText(
                    baseContext,
                    "Type of wine can't be empty !",
                    Toast.LENGTH_SHORT
                ).show()
            }
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