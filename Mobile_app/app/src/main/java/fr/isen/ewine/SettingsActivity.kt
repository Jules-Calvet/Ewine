package fr.isen.ewine

import android.annotation.SuppressLint
import android.content.Intent
import android.content.SharedPreferences
import android.graphics.Color
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.util.Log
import android.widget.Toast
import androidx.appcompat.app.AppCompatDelegate
import com.google.gson.Gson
import fr.isen.ewine.databinding.ActivitySettingsBinding
import fr.isen.ewine.model.UserData

class SettingsActivity : AppCompatActivity() {
    private lateinit var binding : ActivitySettingsBinding
    override fun onCreate(savedInstanceState: Bundle?) {

        super.onCreate(savedInstanceState)

        binding = ActivitySettingsBinding.inflate(layoutInflater)
        setContentView(binding.root)
    }

    override fun onStart() {
        super.onStart()

        //get data from shared preferences
        val sharedPref: SharedPreferences = getSharedPreferences("settings", 0)
        var darkMode = sharedPref.getBoolean("dark_mode", false)

        mode(darkMode)

        var notifications = sharedPref.getBoolean("notifications", true)
        var cellarHeight = sharedPref.getInt("height",3)
        var cellarWidth = sharedPref.getInt("width",3)
        val gson = Gson()
        var json: String
        val jsonFromPrefs = sharedPref.getString("tab_cellar", "")
        if(jsonFromPrefs == ""){
            val tab = Array(cellarWidth) { Array(cellarHeight) {""} }
            val jsonEmpty = gson.toJson(tab)
            sharedPref.edit().putString("tab_cellar", jsonEmpty).apply()
        }
        var tabCellar = gson.fromJson(jsonFromPrefs, Array<Array<UserData.CellarData>>::class.java)

        //link to values for width and height
        binding.numberColumns.text = "$cellarWidth"
        binding.numberRows.text = "$cellarHeight"

        //click on minus width
        binding.buttonMinusColumns.setOnClickListener{
            if (cellarWidth > 1){
                cellarWidth --
                binding.numberColumns.text = "$cellarWidth"
                sharedPref.edit().putInt("width", cellarWidth).apply()
                val tabTemp = tabCellar
                tabCellar = Array(cellarWidth) { i ->
                    Array(cellarHeight) { j ->
                        UserData.CellarData("", "", "", "", "")
                    }
                }
                for(x in 0 until cellarWidth) {
                    for(y in 0 until cellarHeight) {
                        tabCellar[x][y] = tabTemp[x][y]
                    }
                }
                json = gson.toJson(tabCellar)
                sharedPref.edit().putString("tab_cellar", json).apply()
            }
            else {
                Toast.makeText(baseContext, "Warning! You cannot have less than 1 columns in your cellar ! ",Toast.LENGTH_SHORT).show()
            }
        }
        //click on plus width
        binding.buttonPlusColumns.setOnClickListener{
            cellarWidth ++
            binding.numberColumns.text = "$cellarWidth"
            sharedPref.edit().putInt("width", cellarWidth).apply()
            val tabTemp = tabCellar
            tabCellar = Array(cellarWidth) { i ->
                Array(cellarHeight) { j ->
                    UserData.CellarData("", "", "", "", "")
                }
            }
            for(x in 0..cellarWidth-2) {
                for(y in 0 until cellarHeight) {
                    tabCellar[x][y] = tabTemp[x][y]
                }
            }
            json = gson.toJson(tabCellar)
            sharedPref.edit().putString("tab_cellar", json).apply()
        }
        //click on minus height
        binding.buttonMinusRows.setOnClickListener{
            if (cellarHeight > 1){
                cellarHeight --
                binding.numberRows.text = "$cellarHeight"
                sharedPref.edit().putInt("height", cellarHeight).apply()
                val tabTemp = tabCellar
                tabCellar = Array(cellarWidth) { i ->
                    Array(cellarHeight) { j ->
                        UserData.CellarData("", "", "", "", "")
                    }
                }
                for(x in 0 until cellarWidth) {
                    for(y in 0 until cellarHeight) {
                        tabCellar[x][y] = tabTemp[x][y]
                    }
                }
                json = gson.toJson(tabCellar)
                sharedPref.edit().putString("tab_cellar", json).apply()
            }
            else {
                Toast.makeText(baseContext, "Warning! You cannot have less than 1 rows in your cellar ! ",Toast.LENGTH_SHORT).show()
            }
        }
        //click on plus height
        binding.buttonPlusRows.setOnClickListener{
            cellarHeight ++
            binding.numberRows.text = "$cellarHeight"
            sharedPref.edit().putInt("height", cellarHeight).apply()
            val tabTemp = tabCellar
            tabCellar = Array(cellarWidth) { i ->
                Array(cellarHeight) { j ->
                    UserData.CellarData("", "", "", "", "")
                }
            }
            for(x in 0 until cellarWidth) {
                for(y in 0 until cellarHeight-2) {
                    tabCellar[x][y] = tabTemp[x][y]
                }
            }
            json = gson.toJson(tabCellar)
            sharedPref.edit().putString("tab_cellar", json).apply()
        }

        //toggle darkmode
        binding.buttonMode.setOnClickListener {
            darkMode = !darkMode
            sharedPref.edit().putBoolean("dark_mode", darkMode).apply()
            mode(darkMode)
        }

        //adding bottle in the cellar
        binding.buttonAddBottle.setOnClickListener {
            val charX = binding.numberX.text.toString()
            val x : Int
            val y : Int
            if(charX.isNotEmpty()) {
                x = charX.toInt() - 1
            } else {
                x = -1
            }
            val charY = binding.numberY.text.toString()
            if(charY.isNotEmpty()) {
                y = charY.toInt() - 1
            } else {
                y = -1
            }
            val wine = binding.wineType.text.toString().lowercase()
            if(wine.isNotEmpty()) {
                if (x >= 0 && x <= cellarWidth) {
                    if (y >= 0 && y <= cellarHeight) {
                        if (tabCellar[x][y].bottle_TypeOfWine == "") {
                            when (wine) {
                                "red" -> {
                                    tabCellar[x][y].bottle_TypeOfWine = "Red"
                                    json = gson.toJson(tabCellar)
                                    sharedPref.edit().putString("tab_cellar", json).apply()
                                    Toast.makeText(
                                        baseContext,
                                        "Bottle of " + wine + " added at this position : " + charX + ", " + charY + " !",
                                        Toast.LENGTH_SHORT
                                    ).show()
                                }
                                "white" -> {
                                    Log.w(" Avant ", "$jsonFromPrefs")
                                    tabCellar[x][y].bottle_TypeOfWine = "White"
                                    json = gson.toJson(tabCellar)
                                    Log.w(" Après ", json)
                                    sharedPref.edit().putString("tab_cellar", json).apply()
                                    Toast.makeText(
                                        baseContext,
                                        "Bottle of " + wine + " added at this position : " + charX + ", " + charY + " !",
                                        Toast.LENGTH_SHORT
                                    ).show()
                                }
                                "rose" -> {
                                    tabCellar[x][y].bottle_TypeOfWine = "Rose"
                                    json = gson.toJson(tabCellar)
                                    sharedPref.edit().putString("tab_cellar", json).apply()
                                    Toast.makeText(
                                        baseContext,
                                        "Bottle of " + wine + " added at this position : " + charX + ", " + charY + " !",
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
                            "Y not valid ! Must be between 1 and " + cellarHeight + " !",
                            Toast.LENGTH_SHORT
                        ).show()
                    }
                } else {
                    Toast.makeText(
                        baseContext,
                        "X not valid ! Must be between 1 and " + cellarWidth + " !",
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

        //toggle notifications
        binding.toggleNotification.setOnClickListener {
            notifications = !notifications
            if(notifications) {
                binding.toggleNotification.text = "Notifications ON"
            }
            else {
                binding.toggleNotification.text = "Notifications OFF"
            }
            sharedPref.edit().putBoolean("notifications", notifications).apply()
        }

        if(notifications) {
            binding.toggleNotification.text = "Notifications ON"
            binding.toggleNotification.isChecked = true
        } else {
            binding.toggleNotification.text = "Notifications OFF"
            binding.toggleNotification.isChecked = false
        }
    }

    @SuppressLint("UseCompatLoadingForDrawables")
    private fun mode(darkMode : Boolean) {
        if (darkMode) { //dark mode enabled
            //AppCompatDelegate.setDefaultNightMode(AppCompatDelegate.MODE_NIGHT_YES)
            binding.root.setBackgroundColor(Color.BLACK)
            binding.buttonMode.setBackgroundResource(R.drawable.sun)
            binding.buttonMode.foreground = getDrawable(R.drawable.sun)
            binding.toggleNotification.setTextColor(Color.WHITE)
        } else { //dark mode disabled
            //AppCompatDelegate.setDefaultNightMode(AppCompatDelegate.MODE_NIGHT_NO)
            binding.root.setBackgroundColor(Color.WHITE)
            binding.buttonMode.setBackgroundResource(R.drawable.moon)
            binding.buttonMode.foreground = getDrawable(R.drawable.moon)
            binding.toggleNotification.setTextColor(Color.BLACK)
        }
    }
}