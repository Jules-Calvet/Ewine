package fr.isen.ewine

import android.content.Intent
import android.content.SharedPreferences
import android.graphics.Color
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.view.View
import com.google.gson.Gson
import com.squareup.picasso.Picasso
import fr.isen.ewine.databinding.ActivityBottleBinding
import fr.isen.ewine.model.CellarData
import fr.isen.ewine.model.UserData

class BottleActivity : AppCompatActivity() {
    private lateinit var binding : ActivityBottleBinding

    private lateinit var bottleTypeOfWine : String
    private lateinit var bottleName : String
    private lateinit var bottleProducerName : String
    private lateinit var bottleYearOfProduction : String
    private lateinit var tabCellar : UserData
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
        var json : String
        val sharedPref: SharedPreferences = getSharedPreferences("settings", 0)
        var cellarHeight = sharedPref.getInt("height", 3)
        var cellarWidth = sharedPref.getInt("width", 5)
        val darkMode = sharedPref.getBoolean("dark_mode", false)
        val jsonFromPrefs = sharedPref.getString("tab_cellar", null)
        if(jsonFromPrefs != null) {
            tabCellar = gson.fromJson(jsonFromPrefs, UserData::class.java)
            bottleTypeOfWine = tabCellar.cellarData[x+(y*cellarWidth)].bottleTypeOfWine
            bottleName = tabCellar.cellarData[x+(y*cellarWidth)].bottleName
            bottleProducerName = tabCellar.cellarData[x+(y*cellarWidth)].bottleProducerName
            bottleYearOfProduction = tabCellar.cellarData[x+(y*cellarWidth)].bottleYearOfProduction
        }
        mode(darkMode)

        binding.buttonDelete.setOnClickListener {
            tabCellar.cellarData[x+(y*cellarWidth)].bottleTypeOfWine = ""
            tabCellar.cellarData[x+(y*cellarWidth)].bottleName = ""
            tabCellar.cellarData[x+(y*cellarWidth)].bottleProducerName = ""
            tabCellar.cellarData[x+(y*cellarWidth)].bottleYearOfProduction = ""
            json = gson.toJson(tabCellar)
            sharedPref.edit().putString("tab_cellar", json).apply()
            val intent = Intent(this, CellarActivity::class.java)
            startActivity(intent)
            finish()
        }

        displayManagement()
    }

    private fun displayManagement() {
        if(bottleName.isNullOrEmpty()) {
            runOnUiThread {
                binding.BottleName.visibility = View.GONE
            }
        } else {
            runOnUiThread {
                binding.BottleName.text = bottleName
            }
        }
        if(bottleTypeOfWine.isNullOrEmpty()) {
            runOnUiThread {
                binding.BottleTypeOfWine.visibility = View.GONE
                binding.bottleColorImg.visibility = View.GONE
            }
        } else {
            runOnUiThread {
                binding.BottleTypeOfWine.text = bottleTypeOfWine
                when (bottleTypeOfWine) {
                    "Red", "Rouge" -> {
                        Picasso.get().load(R.drawable.red).into(binding.bottleColorImg)
                    }
                    "White", "Blanc" -> {
                        Picasso.get().load(R.drawable.white).into(binding.bottleColorImg)
                    }
                    "Rose", "Rosé" -> {
                        Picasso.get().load(R.drawable.rose).into(binding.bottleColorImg)
                    }
                }
            }
        }
        if(bottleProducerName.isNullOrEmpty()) {
            runOnUiThread {
                binding.BottleProducerName.visibility = View.GONE
            }
        } else {
            runOnUiThread {
                binding.BottleProducerName.text = bottleProducerName
            }
        }
        if(bottleYearOfProduction.isNullOrEmpty()) {
            runOnUiThread {
                binding.BottleYearOfProduction.visibility = View.GONE
            }
        } else {
            runOnUiThread {
                binding.BottleYearOfProduction.text = bottleYearOfProduction
            }
        }
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