package fr.isen.ewine

import android.annotation.SuppressLint
import android.content.SharedPreferences
import android.graphics.Color
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.util.Log
import android.widget.Toast
import com.google.firebase.database.FirebaseDatabase
import com.google.gson.Gson
import fr.isen.ewine.databinding.ActivitySettingsBinding
import fr.isen.ewine.model.CellarData
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
        var cellarHeight = sharedPref.getInt("height", 3)
        var cellarWidth = sharedPref.getInt("width", 5)
        val gson = Gson()
        var json: String
        val jsonFromPrefs = sharedPref.getString("tab_cellar", "")

        var tabCellar : UserData
        Log.d("json", jsonFromPrefs.toString())
        if (jsonFromPrefs == "") {
            tabCellar = UserData("1", ArrayList(cellarWidth+(cellarHeight*cellarWidth)))
            for(x in 0 until cellarWidth) {
                for(y in 0 until cellarHeight) {
                    tabCellar.cellarData.add(CellarData("","","","",""))
                }
            }
            val jsonEmpty = gson.toJson(tabCellar)
            sharedPref.edit().putString("tab_cellar", jsonEmpty).apply()
        } else {
            tabCellar = gson.fromJson(jsonFromPrefs, UserData::class.java)
        }

        //link to values for width and height
        binding.numberColumns.text = "$cellarWidth"
        binding.numberRows.text = "$cellarHeight"

        //click on minus width
        binding.buttonMinusColumns.setOnClickListener {
            if (cellarWidth > 1) {
                cellarWidth--
                binding.numberColumns.text = "$cellarWidth"
                sharedPref.edit().putInt("width", cellarWidth).apply()
                for(x in 1 .. cellarHeight) {
                    tabCellar.cellarData.removeAt(x*cellarWidth)
                }
                json = gson.toJson(tabCellar)
                sharedPref.edit().putString("tab_cellar", json).apply()
            } else {
                Toast.makeText(
                    baseContext,
                    "Warning! You cannot have less than 1 columns in your cellar ! ",
                    Toast.LENGTH_SHORT
                ).show()
            }
        }
        //click on plus width
        binding.buttonPlusColumns.setOnClickListener {
            cellarWidth++
            binding.numberColumns.text = "$cellarWidth"
            sharedPref.edit().putInt("width", cellarWidth).apply()
            for(x in 0 until cellarHeight) {
                tabCellar.cellarData.add(cellarWidth+(x*(cellarWidth-1)) ,CellarData("", "", "", "", ""))
            }
            json = gson.toJson(tabCellar)
            sharedPref.edit().putString("tab_cellar", json).apply()
        }
        //click on minus height
        binding.buttonMinusRows.setOnClickListener {
            if (cellarHeight > 1) {
                cellarHeight--
                binding.numberRows.text = "$cellarHeight"
                sharedPref.edit().putInt("height", cellarHeight).apply()
                for(x in 0 until cellarWidth) {
                    tabCellar.cellarData.removeLast()
                }
                json = gson.toJson(tabCellar)
                sharedPref.edit().putString("tab_cellar", json).apply()
                } else {
                    Toast.makeText(
                        baseContext,
                        "Warning! You cannot have less than 1 rows in your cellar ! ",
                        Toast.LENGTH_SHORT
                    ).show()
                }
            }
            //click on plus height
            binding.buttonPlusRows.setOnClickListener {
                cellarHeight++
                binding.numberRows.text = "$cellarHeight"
                sharedPref.edit().putInt("height", cellarHeight).apply()
                for(x in 0 until cellarWidth) {
                    tabCellar.cellarData.add(CellarData("", "", "", "", ""))
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

            //toggle notifications
            binding.toggleNotification.setOnClickListener {
                notifications = !notifications
                if (notifications) {
                    binding.toggleNotification.text = "Notifications ON"
                } else {
                    binding.toggleNotification.text = "Notifications OFF"
                }
                sharedPref.edit().putBoolean("notifications", notifications).apply()
            }

            if (notifications) {
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