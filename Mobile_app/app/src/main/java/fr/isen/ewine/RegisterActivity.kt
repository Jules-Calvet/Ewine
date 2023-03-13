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
import fr.isen.ewine.databinding.ActivityRegisterBinding

class RegisterActivity : AppCompatActivity() {
    private lateinit var binding : ActivityRegisterBinding
    override fun onCreate(savedInstanceState: Bundle?) {

        super.onCreate(savedInstanceState)

        binding = ActivityRegisterBinding.inflate(layoutInflater)
        setContentView(binding.root)

        val auth = FirebaseAuth.getInstance()

        //Click on buttons
        //Button register
        binding.buttonRegister.setOnClickListener {
            val email = binding.editTextEmail.text.toString()
            val password = binding.editTextPassword.text.toString()
            val passwordConfirm = binding.editTextPasswordConfirm.text.toString()

            if (password.length >= 6) {
                if (email != "" && passwordConfirm != "") {
                    if (password == passwordConfirm) {
                        auth.createUserWithEmailAndPassword(email, password)
                            .addOnCompleteListener(this) { task ->
                                if (task.isSuccessful) {
                                    val intent = Intent(this, CellarActivity::class.java)
                                    startActivity(intent)
                                } else {
                                    // If sign in fails, display a message to the user.
                                    Log.w("Registration", "createUserWithEmail:failure", task.exception)
                                    Toast.makeText(baseContext, task.exception?.message,
                                        Toast.LENGTH_SHORT).show()
                                }
                            }
                    } else {
                        Toast.makeText(
                            baseContext, "Passwords are not the same ! ",
                            Toast.LENGTH_SHORT
                        ).show()
                    }
                } else {
                    Toast.makeText(
                        baseContext, "Please enter email and passwords",
                        Toast.LENGTH_SHORT
                    ).show()
                }
            }
        }
        //Button to login
        binding.buttonToLogin.setOnClickListener {
            val intent = Intent(this, LoginActivity::class.java)
            startActivity(intent)
        }
        //Button settings
        binding.buttonToSettings.setOnClickListener {
            val intent = Intent(this, SettingsActivity::class.java)
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
            binding.textInputLayoutPwdConfirm.setBoxBackgroundColorResource(com.google.android.material.R.color.material_grey_900)
            binding.editTextPasswordConfirm.setTextColor(Color.WHITE)
        } else { //dark mode disabled
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