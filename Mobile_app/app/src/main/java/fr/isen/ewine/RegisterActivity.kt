package fr.isen.ewine

import android.annotation.SuppressLint
import android.content.Intent
import android.content.SharedPreferences
import android.graphics.Color
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import fr.isen.ewine.databinding.ActivityRegisterBinding

class RegisterActivity : AppCompatActivity() {
    lateinit var binding : ActivityRegisterBinding

    override fun onCreate(savedInstanceState: Bundle?) {

        super.onCreate(savedInstanceState)

        binding = ActivityRegisterBinding.inflate(layoutInflater)
        setContentView(binding.root)

        binding.buttonRegister.setOnClickListener {
            val intent = Intent(this, CellarActivity::class.java)
            startActivity(intent)
        }

        binding.buttonToLogin.setOnClickListener {
            val intent = Intent(this, LoginActivity::class.java)
            startActivity(intent)
        }

        binding.buttonToSettings.setOnClickListener {
            val intent = Intent(this, SettingsActivity::class.java)
            startActivity(intent)
        }
    }

    override fun onStart() {
        super.onStart()

        val sharedPref: SharedPreferences = getSharedPreferences("settings", 0)
        val darkMode = sharedPref.getBoolean("dark_mode", false)

        mode(darkMode)
    }

    @SuppressLint("UseCompatLoadingForDrawables")
    private fun mode(darkMode : Boolean) {
        if (darkMode) {
            binding.root.setBackgroundColor(Color.BLACK)
            binding.buttonToSettings.setBackgroundResource(R.drawable.parameter_dark)
            binding.buttonToSettings.foreground = getDrawable(R.drawable.parameter_dark)
            binding.textInputLayoutEmail.setBoxBackgroundColorResource(com.google.android.material.R.color.material_grey_900)
            binding.editTextEmail.setTextColor(Color.WHITE)
            binding.textInputLayoutPwd.setBoxBackgroundColorResource(com.google.android.material.R.color.material_grey_900)
            binding.editTextPassword.setTextColor(Color.WHITE)
            binding.textInputLayoutPwdConfirm.setBoxBackgroundColorResource(com.google.android.material.R.color.material_grey_900)
            binding.editTextPasswordConfirm.setTextColor(Color.WHITE)
        } else {
            binding.root.setBackgroundColor(Color.WHITE)
            binding.buttonToSettings.setBackgroundResource(R.drawable.parameter)
            binding.buttonToSettings.foreground = getDrawable(R.drawable.parameter)
            binding.textInputLayoutEmail.setBoxBackgroundColorResource(com.google.android.material.R.color.material_grey_300)
            binding.editTextEmail.setTextColor(Color.BLACK)
            binding.textInputLayoutPwd.setBoxBackgroundColorResource(com.google.android.material.R.color.material_grey_300)
            binding.editTextPassword.setTextColor(Color.BLACK)
            binding.textInputLayoutPwdConfirm.setBoxBackgroundColorResource(com.google.android.material.R.color.material_grey_300)
            binding.editTextPasswordConfirm.setTextColor(Color.BLACK)
        }
    }
}