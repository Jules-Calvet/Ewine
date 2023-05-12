package fr.isen.ewine

import android.content.Intent
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.view.View
import fr.isen.ewine.databinding.ActivityBottleModifyBinding

class BottleModifyActivity : AppCompatActivity() {
    private lateinit var binding : ActivityBottleModifyBinding

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        binding = ActivityBottleModifyBinding.inflate(layoutInflater)
        setContentView(binding.root)

        binding.buttonToSettings.setOnClickListener {
            val intent = Intent(this, SettingsActivity::class.java)
            startActivity(intent)
        }

        binding.bottleRed.setOnClickListener {
            runOnUiThread {
                binding.bottleColorRed.visibility = View.VISIBLE
                binding.bottleColorWhite.visibility = View.GONE
                binding.bottleColorRose.visibility = View.GONE
            }
        }
        binding.bottleWhite.setOnClickListener {
            runOnUiThread {
                binding.bottleColorRed.visibility = View.GONE
                binding.bottleColorWhite.visibility = View.VISIBLE
                binding.bottleColorRose.visibility = View.GONE
            }
        }
        binding.bottleRose.setOnClickListener {
            runOnUiThread {
                binding.bottleColorRed.visibility = View.GONE
                binding.bottleColorWhite.visibility = View.GONE
                binding.bottleColorRose.visibility = View.VISIBLE
            }
        }
    }

    override fun onStart() {
        super.onStart()
    }
}