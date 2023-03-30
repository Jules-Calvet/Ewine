package fr.isen.ewine

import android.content.Intent
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import com.google.firebase.auth.FirebaseAuth
import fr.isen.ewine.databinding.ActivityProfileBinding
import kotlin.reflect.typeOf

class ProfileActivity : AppCompatActivity() {
    private lateinit var binding: ActivityProfileBinding
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        checkLoginStatus()

        binding = ActivityProfileBinding.inflate(layoutInflater)
        setContentView(binding.root)
    }

    private fun checkLoginStatus() {
        val currentUser = FirebaseAuth.getInstance().currentUser
        if (currentUser == null) {
            startActivity(Intent(this, LoginActivity::class.java))
        }
    }
}