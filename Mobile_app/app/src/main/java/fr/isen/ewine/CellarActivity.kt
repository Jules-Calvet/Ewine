package fr.isen.ewine

import  android.annotation.SuppressLint
import android.content.Intent
import android.content.SharedPreferences
import android.graphics.Color
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.util.Log
import android.view.Menu
import android.view.MenuInflater
import android.view.View
import android.widget.PopupMenu
import androidx.recyclerview.widget.LinearLayoutManager
import com.google.firebase.auth.FirebaseAuth
import com.google.gson.Gson
import fr.isen.ewine.databinding.ActivityCellarBinding
import fr.isen.ewine.model.CellarData
import fr.isen.ewine.model.UserData

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
        val cellarWidth = sharedPref.getInt("width",5)
        val jsonFromPrefs = sharedPref.getString("tab_cellar", null)
        binding.rowsRecyclerView.layoutManager = LinearLayoutManager(this)
        if(jsonFromPrefs != null){
            Log.d("json get", jsonFromPrefs)
            val tabCellar = gson.fromJson(jsonFromPrefs, UserData::class.java)
            binding.rowsRecyclerView.adapter = RowsAdapter(cellarHeight,cellarWidth,tabCellar)
            var bottleCount = 0
            for (x in 0 until cellarWidth) {
                for(y in 0 until cellarHeight){
                    if(tabCellar.cellarData[x+(y*cellarWidth)].bottleTypeOfWine != ""){
                        bottleCount ++
                    }
                }
            }
            val totalNumber = cellarHeight*cellarWidth
            binding.bottleNumber.text = "Bottles : $bottleCount / $totalNumber"
        } else {
            val tabCellar = UserData("1", ArrayList())
            for(x in 0 until cellarWidth) {
                for(y in 0 until cellarHeight) {
                    tabCellar.cellarData.add(CellarData("","","","",""))
                }
            }
            binding.rowsRecyclerView.adapter = RowsAdapter(cellarHeight,cellarWidth, tabCellar)
        }

        mode(darkMode)
        binding.menu.setOnClickListener {
            showPopup(it)
        }
    }

    private fun checkLoginStatus() {
        val currentUser = FirebaseAuth.getInstance().currentUser
        if (currentUser == null) {
            startActivity(Intent(this, LoginActivity::class.java))
        }
    }

    fun showPopup(v: View) {
        val popup = PopupMenu(this, v)
        val inflater: MenuInflater = popup.menuInflater
        inflater.inflate(R.menu.menu, popup.menu)
        popup.show()
        popup.menu.findItem(R.id.menuLogout).setOnMenuItemClickListener {
            FirebaseAuth.getInstance().signOut()
            checkLoginStatus()
            true
        }
        popup.menu.findItem(R.id.menuProfile).setOnMenuItemClickListener {
            startActivity(Intent(this, ProfileActivity::class.java))
            true
        }
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