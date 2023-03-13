package fr.isen.ewine

import android.annotation.SuppressLint
import android.content.Intent
import android.content.SharedPreferences
import android.graphics.Color
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.util.Log
import android.widget.Toast
import com.google.firebase.auth.FirebaseAuth
import fr.isen.ewine.databinding.ActivityLoginBinding

class LoginActivity : AppCompatActivity() {
    private lateinit var binding : ActivityLoginBinding
    override fun onCreate(savedInstanceState: Bundle?) {

        super.onCreate(savedInstanceState)

        binding = ActivityLoginBinding.inflate(layoutInflater)
        setContentView(binding.root)

        val auth = FirebaseAuth.getInstance()

        checkLoginStatus()

        //Click on buttons
        //Button login
        binding.buttonLogIn.setOnClickListener {
            val email = binding.editTextEmail.text.toString()
            val pwd = binding.editTextPassword.text.toString()
            if(email != "" && pwd != "") {
                auth.signInWithEmailAndPassword(email, pwd)
                    .addOnCompleteListener {
                        if (it.isSuccessful) {
                            val intent = Intent(this, CellarActivity::class.java)
                            startActivity(intent)
                        } else {
                            Log.w("Login", "signInWithEmail:failure", it.exception)
                            Toast.makeText(
                                baseContext, "Authentication failed.",
                                Toast.LENGTH_SHORT
                            ).show()
                        }
                    }
            } else {
                Toast.makeText(
                    baseContext, "Please enter email and password",
                    Toast.LENGTH_SHORT
                ).show()
            }
        }
        //Button to register
        binding.buttonToRegister.setOnClickListener {
            val intent = Intent(this, RegisterActivity::class.java)
            startActivity(intent)
        }
        //Button settings
        binding.buttonToSettings.setOnClickListener {
            val intent = Intent(this, SettingsActivity::class.java)
            startActivity(intent)
        }
        //Button forget password
        binding.buttonForgetPwd.setOnClickListener {
            val intent = Intent(this, BottleActivity::class.java)
            startActivity(intent)
        }

    }

    override fun onStart() {
        super.onStart()

        //Get data from shared preferences
        val sharedPref: SharedPreferences = getSharedPreferences("settings", 0)
        val darkMode = sharedPref.getBoolean("dark_mode", false)

        mode(darkMode)
    }
    private fun checkLoginStatus() {
        val currentUser = FirebaseAuth.getInstance().currentUser
        if (currentUser != null) {
            startActivity(Intent(this, CellarActivity::class.java))
        }
    }
    @SuppressLint("UseCompatLoadingForDrawables")
    private fun mode(darkMode : Boolean) {
        if (darkMode) { //dark mode enabled
            binding.root.setBackgroundColor(Color.BLACK)
            binding.buttonToSettings.setBackgroundResource(R.drawable.parameter_dark)
            binding.buttonToSettings.foreground = getDrawable(R.drawable.parameter_dark)
            binding.textInputLayoutEmail.setBoxBackgroundColorResource(com.google.android.material.R.color.material_grey_900)
            binding.editTextEmail.setTextColor(Color.WHITE)
            binding.textInputLayoutPwd.setBoxBackgroundColorResource(com.google.android.material.R.color.material_grey_900)
            binding.editTextPassword.setTextColor(Color.WHITE)
        } else { //dark mode disabled
            binding.root.setBackgroundColor(Color.WHITE)
            binding.buttonToSettings.setBackgroundResource(R.drawable.parameter)
            binding.buttonToSettings.foreground = getDrawable(R.drawable.parameter)
            binding.textInputLayoutEmail.setBoxBackgroundColorResource(com.google.android.material.R.color.material_grey_300)
            binding.editTextEmail.setTextColor(Color.BLACK)
            binding.textInputLayoutPwd.setBoxBackgroundColorResource(com.google.android.material.R.color.material_grey_300)
            binding.editTextPassword.setTextColor(Color.BLACK)
        }
    }
}