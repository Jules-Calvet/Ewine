package fr.isen.ewine

import android.content.Intent
import android.content.SharedPreferences
import android.graphics.Color
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.view.View
import com.google.gson.Gson
import com.squareup.picasso.Picasso
import fr.isen.ewine.databinding.ActivityBottleModifyBinding
import fr.isen.ewine.model.UserData

class BottleModifyActivity : AppCompatActivity() {
    private lateinit var binding : ActivityBottleModifyBinding

    private lateinit var bottleTypeOfWine : String
    private lateinit var bottleName : String
    private lateinit var bottleProducerName : String
    private lateinit var bottleYearOfProduction : String
    private lateinit var tabCellar : UserData

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        binding = ActivityBottleModifyBinding.inflate(layoutInflater)
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
        var json: String
        val sharedPref: SharedPreferences = getSharedPreferences("settings", 0)
        var cellarHeight = sharedPref.getInt("height", 3)
        var cellarWidth = sharedPref.getInt("width", 5)
        val darkMode = sharedPref.getBoolean("dark_mode", false)
        val jsonFromPrefs = sharedPref.getString("tab_cellar", null)
        if(jsonFromPrefs != null) {
            tabCellar = gson.fromJson(jsonFromPrefs, UserData::class.java)
            bottleTypeOfWine = tabCellar.cellarData[x+(y*sharedPref.getInt("width",0))].bottleTypeOfWine
            bottleName = tabCellar.cellarData[x+(y*sharedPref.getInt("width", 0))].bottleName
            bottleProducerName = tabCellar.cellarData[x+(y*sharedPref.getInt("width", 0))].bottleProducerName
            bottleYearOfProduction = tabCellar.cellarData[x+(y*sharedPref.getInt("width", 0))].bottleYearOfProduction
        } else {
            startActivity(Intent(this, CellarActivity::class.java))
            finish()
        }


        if(!bottleTypeOfWine.isNullOrEmpty()) {
            runOnUiThread {
                when (bottleTypeOfWine) {
                    "Red", "Rouge" -> {
                        binding.bottleColorRed.visibility = View.VISIBLE
                    }
                    "White", "Blanc" -> {
                        binding.bottleColorWhite.visibility = View.VISIBLE
                    }
                    "Rose", "Rosé" -> {
                        binding.bottleColorRose.visibility = View.VISIBLE
                    }
                }
            }
        }

        mode(darkMode)

        binding.bottleRed.setOnClickListener {
            runOnUiThread {
                binding.bottleColorRed.visibility = View.VISIBLE
                binding.bottleColorWhite.visibility = View.GONE
                binding.bottleColorRose.visibility = View.GONE
                bottleTypeOfWine = "Red"
                tabCellar.cellarData[x+(y*cellarWidth)].bottleTypeOfWine = bottleTypeOfWine
                json = gson.toJson(tabCellar)
                sharedPref.edit().putString("tab_cellar", json).apply()
            }
        }
        binding.bottleWhite.setOnClickListener {
            runOnUiThread {
                binding.bottleColorRed.visibility = View.GONE
                binding.bottleColorWhite.visibility = View.VISIBLE
                binding.bottleColorRose.visibility = View.GONE
                bottleTypeOfWine = "White"
                tabCellar.cellarData[x+(y*cellarWidth)].bottleTypeOfWine = bottleTypeOfWine
                json = gson.toJson(tabCellar)
                sharedPref.edit().putString("tab_cellar", json).apply()
            }
        }
        binding.bottleRose.setOnClickListener {
            runOnUiThread {
                binding.bottleColorRed.visibility = View.GONE
                binding.bottleColorWhite.visibility = View.GONE
                binding.bottleColorRose.visibility = View.VISIBLE
                bottleTypeOfWine = "Rose"
                tabCellar.cellarData[x+(y*cellarWidth)].bottleTypeOfWine = bottleTypeOfWine
                json = gson.toJson(tabCellar)
                sharedPref.edit().putString("tab_cellar", json).apply()
            }
        }
    }

    private fun mode(darkMode: Boolean) {
        if (darkMode) {
            binding.root.setBackgroundColor(Color.BLACK)
        } else {
            binding.root.setBackgroundColor(Color.WHITE)
        }
    }
}