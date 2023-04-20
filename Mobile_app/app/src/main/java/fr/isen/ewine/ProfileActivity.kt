package fr.isen.ewine

import android.content.Intent
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.util.Log
import android.view.View
import android.widget.Toast
import com.google.firebase.auth.EmailAuthProvider
import com.google.firebase.auth.FirebaseAuth
import fr.isen.ewine.databinding.ActivityProfileBinding

class ProfileActivity : AppCompatActivity() {
    private lateinit var binding: ActivityProfileBinding

    private var switchModifyChecked = false
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        checkLoginStatus()

        val auth = FirebaseAuth.getInstance()

        binding = ActivityProfileBinding.inflate(layoutInflater)
        setContentView(binding.root)

        binding.switchModify.setOnClickListener {
            if(!switchModifyChecked) {
                binding.switchModify.text = "Password"
                binding.textInputLayoutEmailModify.visibility = View.GONE
                binding.textInputLayoutPasswordModify.visibility = View.VISIBLE
            } else {
                binding.switchModify.text = "Email"
                binding.textInputLayoutPasswordModify.visibility = View.GONE
                binding.textInputLayoutEmailModify.visibility = View.VISIBLE
            }
            switchModifyChecked = !switchModifyChecked
        }

        binding.modify.setOnClickListener {
            auth.signInWithEmailAndPassword(auth.currentUser!!.email!!, binding.editTextPwd.text.toString()).addOnCompleteListener {
                if(it.isSuccessful) {
                    Log.d("login test", "OK")
                    if (!switchModifyChecked) {
                        auth.currentUser!!.updateEmail(binding.editTextEmailModify.text.toString())
                        Log.d("Update", "EMAIL")
                    } else {
                        auth.currentUser!!.updatePassword(binding.editTextPwdModify.text.toString())
                        Log.d("Update", "PASSWORD")
                    }
                } else {
                    Toast.makeText(
                        baseContext, "Wrong Password !",
                        Toast.LENGTH_SHORT
                    ).show()
                }
            }
            }
    }

    private fun checkLoginStatus() {
        val currentUser = FirebaseAuth.getInstance().currentUser
        if (currentUser == null) {
            startActivity(Intent(this, LoginActivity::class.java))
            finish()
        }
    }
}