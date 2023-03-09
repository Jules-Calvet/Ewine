package fr.isen.ewine

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import fr.isen.ewine.databinding.ActivityBottleBinding

class BottleActivity : AppCompatActivity() {
    private lateinit var binding : ActivityBottleBinding
    override fun onCreate(savedInstanceState: Bundle?) {

        super.onCreate(savedInstanceState)

        binding = ActivityBottleBinding.inflate(layoutInflater)
        setContentView(binding.root)

    }
}