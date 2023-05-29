package fr.isen.ewine

import  android.annotation.SuppressLint
import android.content.Context
import android.content.Intent
import android.content.SharedPreferences
import android.graphics.Color
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.os.Handler
import android.text.TextUtils
import android.util.Log
import android.view.Menu
import android.view.MenuInflater
import android.view.View
import android.widget.PopupMenu
import androidx.recyclerview.widget.LinearLayoutManager
import com.google.firebase.auth.FirebaseAuth
import com.google.gson.Gson
import com.hivemq.client.mqtt.MqttClient
import com.hivemq.client.mqtt.MqttGlobalPublishFilter
import com.hivemq.client.mqtt.mqtt3.Mqtt3AsyncClient
import com.hivemq.client.mqtt.mqtt3.message.connect.connack.Mqtt3ConnAck
import com.hivemq.client.mqtt.mqtt3.message.publish.Mqtt3Publish
import com.hivemq.client.mqtt.mqtt3.message.subscribe.suback.Mqtt3SubAck
import fr.isen.ewine.databinding.ActivityCellarBinding
import fr.isen.ewine.model.CellarData
import fr.isen.ewine.model.UserData
import io.netty.util.AsciiString
import java.util.*
import java.util.concurrent.CompletableFuture
import kotlin.collections.ArrayList

class CellarActivity : AppCompatActivity() {
    private lateinit var binding: ActivityCellarBinding
    private lateinit var client : Mqtt3AsyncClient
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
        val sharedPrefs: SharedPreferences = getSharedPreferences("settings", Context.MODE_PRIVATE)
        val jsonFromPref = sharedPrefs.getString("temperature", "20")
        binding.tempValue.text = jsonFromPref
        val topicToF = "ewine/ToF"
        val topicTemp = "ewine/Temp"

        clientBuild()
        connectClient().thenAccept {
            if(it) {
                subscribe(topicToF)
                subscribe(topicTemp)
                getMessage()
            }
        }

        mode(darkMode)
        binding.menu.setOnClickListener {
            showPopup(it)
        }
    }

    private fun clientBuild(){
        val clientBuilder = MqttClient.builder()
            .identifier(UUID.randomUUID().toString())
            .serverHost("broker.mqttdashboard.com")

        client = clientBuilder.useMqttVersion3().buildAsync()
    }
    private fun connectClient(): CompletableFuture<Boolean> {
        var success = CompletableFuture<Boolean>()
        client.connectWith()
            .send()
            .whenComplete { connAck: Mqtt3ConnAck?, throwable: Throwable? ->
                if (throwable != null) {
                    Log.d("connect", "failure")
                    success.complete(false)
                } else {
                    Log.d("connect", "success")
                    success.complete(true)
                }
            }
        Log.d("success value", "$success")
        return success
    }
    private fun subscribe(topic : String){
        client.subscribeWith()
            .topicFilter(topic)
            .send()
            .whenComplete { subAck: Mqtt3SubAck?, throwable: Throwable? ->
                if (throwable != null) {
                    Log.d("subscribe", "failure")
                } else {
                    Log.d("subscribe", "success")
                }
            }
    }

    private fun getMessage() {
        client.toAsync().publishes(MqttGlobalPublishFilter.ALL) { publish: Mqtt3Publish ->
            val message = publish.payloadAsBytes.toString(Charsets.UTF_8)
            if (publish.topic.toString().contains("ToF")) {
                Log.d("Received message: {} -> {}, ", "${publish.topic}, $message")
                if (AsciiString.contains(message, "tof_left\":1")) {
                    runOnUiThread {
                        PlaceAutoBottle(0,1)
                    }
                }
                if (AsciiString.contains(message, "tof_left\":0")) {
                    runOnUiThread {
                        PlaceAutoBottle(0,0)

                    }
                }
                if (AsciiString.contains(message, "tof_center\":1")) {
                    runOnUiThread {
                        PlaceAutoBottle(1,1)
                    }
                }
                if (AsciiString.contains(message, "tof_center\":0")) {
                    runOnUiThread {
                        PlaceAutoBottle(1,0)

                    }
                }
                if (AsciiString.contains(message, "tof_right\":1")) {
                    runOnUiThread {
                        PlaceAutoBottle(2,1)
                    }
                }
                if (AsciiString.contains(message, "tof_right\":0")) {
                    runOnUiThread {
                        PlaceAutoBottle(2,0)
                    }
                }
            }
            if(publish.topic.toString().contains("Temp")) {
                Log.d("temp Received message: {} -> {}, ", "${publish.topic}, $message")
                val temp = TextUtils.substring(message, message.indexOf(':') + 1, message.indexOf('}'))
                runOnUiThread {
                    val sharedPref: SharedPreferences = getSharedPreferences("settings", Context.MODE_PRIVATE)
                    val jsonFromPrefs = sharedPref.getString("temperature", "20")
                    if (jsonFromPrefs != null) {
                        sharedPref.edit().putString("temperature", temp).apply()
                        binding.tempValue.text = temp
                    }
                }
            }
        }
    }

    private fun PlaceAutoBottle(place: Int, state: Int){
        val x = place
        val y = 0
        val gson = Gson()
        var json: String
        val sharedPref: SharedPreferences = getSharedPreferences("settings", 0)
        var cellarWidth = sharedPref.getInt("width", 5)
        val jsonFromPrefs = sharedPref.getString("tab_cellar", null)
        val tabCellar = gson.fromJson(jsonFromPrefs, UserData::class.java)
        if(state == 1) tabCellar.cellarData[x + (y * cellarWidth)].bottleTypeOfWine = "Grey"
        else tabCellar.cellarData[x + (y * cellarWidth)].bottleTypeOfWine = ""
        tabCellar.cellarData[x + (y * cellarWidth)].bottleName = ""
        tabCellar.cellarData[x + (y * cellarWidth)].bottleProducerName = ""
        tabCellar.cellarData[x + (y * cellarWidth)].bottleYearOfProduction = ""
        json = gson.toJson(tabCellar)
        sharedPref.edit().putString("tab_cellar", json).apply()
        val intent = Intent(this, CellarActivity::class.java)
        startActivity(intent)
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