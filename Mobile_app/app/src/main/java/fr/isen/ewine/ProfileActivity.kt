package fr.isen.ewine

import android.Manifest
import android.annotation.SuppressLint
import android.app.AlertDialog
import android.bluetooth.*
import android.bluetooth.le.BluetoothLeScanner
import android.bluetooth.le.ScanCallback
import android.bluetooth.le.ScanResult
import android.content.*
import android.content.pm.PackageManager
import android.net.wifi.WifiManager
import android.net.wifi.WifiSsid
import android.os.Build
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.os.Handler
import android.text.InputType
import android.util.Log
import android.view.View
import android.widget.EditText
import android.widget.Toast
import androidx.core.app.ActivityCompat
import androidx.core.content.ContextCompat
import androidx.recyclerview.widget.LinearLayoutManager
import com.google.firebase.auth.FirebaseAuth
import fr.isen.ewine.databinding.ActivityProfileBinding
import java.util.*
import kotlin.collections.ArrayList

class ProfileActivity : AppCompatActivity() {
    private lateinit var binding: ActivityProfileBinding

    private var switchModifyChecked = false
    private var imageWifi = true
    private var bleConnected = false

    private val bluetoothAdapter: BluetoothAdapter? by lazy(LazyThreadSafetyMode.NONE) {
        val bluetoothManager = getSystemService(Context.BLUETOOTH_SERVICE) as BluetoothManager
        bluetoothManager.adapter
    }

    private var bluetoothGatt: BluetoothGatt? = null

    private val serviceUUID = UUID.fromString("0000feed-cc7a-482a-984a-7f2ed5b3e58f")
    private val characteristicSsidUUID = UUID.fromString("0000abcd-8e22-4541-9d4c-21edae82ed19")
    private val characteristicPasswordUUID = UUID.fromString("00001234-8e22-4541-9d4c-21edae82ed19")
    private val characteristicAckUUID = UUID.fromString("0000dead-8e22-4541-9d4c-21edae82ed19")
    private val configNotifications = UUID.fromString("00002902-0000-1000-8000-00805f9b34fb")

    private lateinit var bluetoothLeScanner : BluetoothLeScanner

    private lateinit var deviceName : String

    private val REQUIRED_PERMISSIONS_BLE = if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.S) {
        arrayOf(
            Manifest.permission.BLUETOOTH_CONNECT,
            Manifest.permission.BLUETOOTH_SCAN,
            Manifest.permission.ACCESS_FINE_LOCATION,
            Manifest.permission.ACCESS_COARSE_LOCATION,
            Manifest.permission.CHANGE_WIFI_STATE
        )
    } else {
        arrayOf(
            Manifest.permission.ACCESS_FINE_LOCATION,
            Manifest.permission.ACCESS_COARSE_LOCATION
        )
    }

    private lateinit var wifiManager : WifiManager

    private val wifiDeviceList = WifiDevices()

    private lateinit var ssidClicked : String

    private var ack = 0

    @SuppressLint("MissingPermission")
    private fun showPasswordDialog(ssid: String) {
        val builder = AlertDialog.Builder(this)
        builder.setTitle("Entrez le mot de passe pour $ssid")

        // Set up the input
        val input = EditText(this)
        input.inputType = InputType.TYPE_CLASS_TEXT or InputType.TYPE_TEXT_VARIATION_PASSWORD
        builder.setView(input)

        // Set up the buttons
        builder.setPositiveButton("OK") { dialog, which ->
            var ssidToSend = ssid.trim().trim('\"')
            val password = input.text.toString()
            connectWifi(ssidToSend, password)
            Log.d("ssid send", ssidToSend)
            Log.d("password send", password)
        }
        builder.setNegativeButton("Annuler") { dialog, which -> dialog.cancel() }

        builder.show()
    }

    @SuppressLint("MissingPermission")
    private fun scanSuccess() {
        val results = wifiManager.scanResults
        Log.d("wifi scan", results.toString())
        wifiDeviceList.addWifi(results)

        binding.wifiAvailable.adapter = AdapterDevicesListWifi(wifiDeviceList) {device, position ->
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.Q) {
                ssidClicked = device.ssid[position].toString()
                showPasswordDialog(device.ssid[position].toString())
            }
        }
        //use new scan results ...
    }

    @SuppressLint("MissingPermission")
    private fun scanFailure() {
        // handle failure: new scan did NOT succeed
        // consider using old scan results: these are the OLD results!
        val results = wifiManager.scanResults
        //potentially use older scan results ...
    }

    private var scanning = false
    private lateinit var sharedPref : SharedPreferences
    private lateinit var cellarConnected : String

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        checkLoginStatus()

        val auth = FirebaseAuth.getInstance()

        binding = ActivityProfileBinding.inflate(layoutInflater)
        setContentView(binding.root)

        sharedPref = getSharedPreferences("settings", 0)
        cellarConnected = sharedPref.getString("cellarBLE", null).toString()

        ssidClicked = ""

        bluetoothLeScanner = bluetoothAdapter!!.bluetoothLeScanner

        wifiManager = this.getSystemService(Context.WIFI_SERVICE) as WifiManager

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
            if(!binding.editTextPwd.text.isNullOrEmpty()) {
                auth.signInWithEmailAndPassword(
                    auth.currentUser!!.email!!,
                    binding.editTextPwd.text.toString()
                ).addOnCompleteListener {
                    if (it.isSuccessful) {
                        Log.d("login test", "OK")
                        if (!switchModifyChecked) {
                            if(!binding.editTextEmailModify.text.isNullOrEmpty()) {
                                auth.currentUser!!.updateEmail(binding.editTextEmailModify.text.toString())
                                Log.d("Update", "EMAIL")
                            }
                        } else {
                            if(!binding.editTextPwdModify.text.isNullOrEmpty()) {
                                auth.currentUser!!.updatePassword(binding.editTextPwdModify.text.toString())
                                Log.d("Update", "PASSWORD")
                            }
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

        binding.imageLogo.setOnClickListener {
            binding.wifiAvailable.layoutManager = LinearLayoutManager(this)
            if(imageWifi) {
                binding.imageLogo.setImageResource(R.drawable.logobluetooth)
                binding.groupWifiConnected.visibility = View.GONE
                Log.d("cellarConnected", cellarConnected)
                if(cellarConnected == "null") {
                    if (REQUIRED_PERMISSIONS_BLE.all {
                            ContextCompat.checkSelfPermission(
                                this,
                                it
                            ) != PackageManager.PERMISSION_GRANTED
                        }) {
                        if (REQUIRED_PERMISSIONS_BLE.all {
                                ActivityCompat.shouldShowRequestPermissionRationale(this, it)
                            }) {
                            // Expliquer à l'utilisateur pourquoi la permission est nécessaire
                            AlertDialog.Builder(this)
                                .setTitle("Autoriser la permission Bluetooth")
                                .setMessage("L'application a besoin de la permission Bluetooth pour détecter les appareils à proximité.")
                                .setPositiveButton("OK") { _, _ ->
                                    ActivityCompat.requestPermissions(this, REQUIRED_PERMISSIONS_BLE, 1)
                                }
                                .setNegativeButton("Annuler", null)
                                .show()
                        } else {
                            ActivityCompat.requestPermissions(this, REQUIRED_PERMISSIONS_BLE, 1)
                        }
                    } else {
                        if (bluetoothAdapter == null) {
                            Toast.makeText(
                                this,
                                "Le Bluetooth n'est pas disponible sur ce téléphone",
                                Toast.LENGTH_SHORT
                            ).show()
                        } else if (!bluetoothAdapter!!.isEnabled) {
                            Toast.makeText(this, "Le Bluetooth est désactivé", Toast.LENGTH_SHORT)
                                .show()
                        } else {
                            // Bluetooth est activé, continuer avec les interactions Bluetooth
                            scanLeDevice()
                        }
                    }
                } else {
                    if(!bleConnected) {
                        val device = bluetoothAdapter!!.getRemoteDevice(cellarConnected)
                        bluetoothGatt =
                            device.connectGatt(this@ProfileActivity, false, gattCallback)
                        deviceName = device.name
                    }
                }
            } else {
                binding.imageLogo.setImageResource(R.drawable.logowifi)
                if(scanning) {
                    scanLeDevice()
                }
                binding.groupConnected.visibility = View.GONE
                binding.wifiAvailable.visibility = View.VISIBLE

                if(ack == 1) {
                    binding.wifiAvailable.visibility = View.GONE
                    binding.groupWifiConnected.visibility = View.VISIBLE
                } else {

                    val wifiScanReceiver = object : BroadcastReceiver() {

                        override fun onReceive(context: Context, intent: Intent) {
                            val success =
                                intent.getBooleanExtra(WifiManager.EXTRA_RESULTS_UPDATED, false)
                            if (success) {
                                scanSuccess()
                            } else {
                                scanFailure()
                            }
                        }
                    }

                    if (REQUIRED_PERMISSIONS_BLE.all {
                            ContextCompat.checkSelfPermission(
                                this,
                                it
                            ) != PackageManager.PERMISSION_GRANTED
                        }) {
                        if (REQUIRED_PERMISSIONS_BLE.all {
                                ActivityCompat.shouldShowRequestPermissionRationale(this, it)
                            }) {
                            // Expliquer à l'utilisateur pourquoi la permission est nécessaire
                            AlertDialog.Builder(this)
                                .setTitle("Autoriser la permission Bluetooth")
                                .setMessage("L'application a besoin de la permission Bluetooth pour détecter les appareils à proximité.")
                                .setPositiveButton("OK") { _, _ ->
                                    ActivityCompat.requestPermissions(
                                        this,
                                        REQUIRED_PERMISSIONS_BLE,
                                        1
                                    )
                                }
                                .setNegativeButton("Annuler", null)
                                .show()
                        } else {
                            ActivityCompat.requestPermissions(this, REQUIRED_PERMISSIONS_BLE, 1)
                        }
                    } else {
                        val intentFilter = IntentFilter()
                        intentFilter.addAction(WifiManager.SCAN_RESULTS_AVAILABLE_ACTION)
                        this.registerReceiver(wifiScanReceiver, intentFilter)

                        val success = wifiManager.startScan()
                        if (!success) {
                            // scan failure handling
                            scanFailure()
                        }
                    }
                }
            }
            imageWifi = !imageWifi
        }

        binding.buttonScan.setOnClickListener {
            scanLeDevice()
        }

        binding.buttonDisconnectWifi.setOnClickListener {
            ssidClicked = ""
            binding.groupWifiConnected.visibility = View.GONE
            disconnectWifi()
        }
    }

    private fun checkLoginStatus() {
        val currentUser = FirebaseAuth.getInstance().currentUser
        if (currentUser == null) {
            startActivity(Intent(this, LoginActivity::class.java))
            finish()
        }
    }

    @Suppress("DEPRECATION")
    private val handler = Handler()
    // Stops scanning after 10 seconds.
    private val SCAN_PERIOD: Long = 15000

    @SuppressLint("MissingPermission")
    private fun scanLeDevice() {
        if (!scanning) { // Stops scanning after a pre-defined scan period.
            binding.buttonScan.visibility = View.GONE
            handler.postDelayed({
                scanning = false
                bluetoothLeScanner.stopScan(leScanCallback)
                runOnUiThread {
                    binding.buttonScan.visibility = View.VISIBLE
                }
            }, SCAN_PERIOD)
            scanning = true
            bluetoothLeScanner.startScan(leScanCallback)
        } else {
            scanning = false
            bluetoothLeScanner.stopScan(leScanCallback)
            handler.removeCallbacksAndMessages(null)
        }
    }

    private val leDeviceListAdapter = Devices()
    // Device scan callback.
    private val leScanCallback: ScanCallback = object : ScanCallback() {
        @SuppressLint("MissingPermission")
        override fun onScanResult(callbackType: Int, result: ScanResult) {
            super.onScanResult(callbackType, result)
            leDeviceListAdapter.addDevice(result.device)
            binding.wifiAvailable.adapter = AdapterDevicesList(leDeviceListAdapter) { device, position ->
                if(scanning) {
                    scanLeDevice()
                }
                val device = bluetoothAdapter!!.getRemoteDevice(device.MAC[position])
                bluetoothGatt = device.connectGatt(this@ProfileActivity, false, gattCallback)
                deviceName = device.name
            }
        }
    }

    class Devices {
        var device_name: ArrayList<String> = ArrayList()
        var MAC: ArrayList<String> = ArrayList()
        var size = 0

        @SuppressLint("MissingPermission")
        fun addDevice(device: BluetoothDevice) {
            //verify if there is a name and if we already scanned the device
            if (!device.name.isNullOrBlank()) {
                if(!MAC.contains(device.address)) {
                    device_name.add(device.name)
                    MAC.add(device.address)
                    size++
                }
            }
        }
    }

    class WifiDevices {
        var ssid: ArrayList<WifiSsid> = ArrayList()
        var size = 0

        fun addWifi(wifi: MutableList<android.net.wifi.ScanResult>) {
            if(Build.VERSION.SDK_INT >= Build.VERSION_CODES.TIRAMISU) {
                for(i in 0 until wifi.size) {
                    if (!wifi[i].wifiSsid.toString().isNullOrBlank()) {
                        if (!ssid.contains(wifi[i].wifiSsid)) {
                            ssid.add(wifi[i].wifiSsid!!)
                            size++
                        }
                    }
                }
            } else {
                TODO("VERSION.SDK_INT < TIRAMISU")
            }
        }
    }

    override fun onRequestPermissionsResult(
        requestCode: Int,
        permissions: Array<String>,
        grantResults: IntArray
    ) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults)
        if (requestCode == 1) {
            if (grantResults.isNotEmpty() && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
                // La permission a été accordée
                startActivity(Intent(this, ProfileActivity::class.java))
            } else {
                Toast.makeText(
                    this,
                    "Les permissions sont nécessaires pour utiliser la fonction de scan",
                    Toast.LENGTH_SHORT
                ).show()
                finish()
            }
        }
    }

    private val gattCallback = object : BluetoothGattCallback() {
        @SuppressLint("MissingPermission")
        override fun onConnectionStateChange(gatt: BluetoothGatt?, status: Int, newState: Int) {
            when (newState) {
                BluetoothProfile.STATE_CONNECTED -> {
                    // Le périphérique a été connecté avec succès.
                    Log.d("STATUS", "Connected to GATT server.")
                    bleConnected = true
                    // Découvrez les services GATT disponibles sur le périphérique.
                    bluetoothGatt?.discoverServices()
                    runOnUiThread {
                        binding.wifiAvailable.visibility = View.GONE
                        binding.groupConnected.visibility = View.VISIBLE
                        binding.connectedDevice.text =  getString(R.string.connectedTo) + " " + deviceName
                        sharedPref.edit().putString("cellarBLE", bluetoothGatt!!.device.address).apply()
                    }
                    binding.buttonDisconnectBLE.setOnClickListener {
                        bluetoothGatt?.disconnect()
                        sharedPref.edit().putString("cellarBLE", null).apply()
                        cellarConnected = null.toString()
                    }
                }
                BluetoothProfile.STATE_DISCONNECTED -> {
                    // Le périphérique a été déconnecté.
                    Log.d("STATUS", "Disconnected from GATT server.")
                    bleConnected = false
                    runOnUiThread {
                        binding.wifiAvailable.visibility = View.VISIBLE
                        binding.groupConnected.visibility = View.GONE
                    }
                }
                else -> {
                    Log.d("STATUS", "Connection state changed: $newState")
                }
            }
        }
        override fun onServicesDiscovered(gatt: BluetoothGatt?, status: Int) {
            when (status) {
                BluetoothGatt.GATT_SUCCESS -> {
                    // Les services GATT du périphérique ont été découverts avec succès.
                    Log.d("STATUS", "Services discovered successfully.")
                    val service = gatt?.getService(serviceUUID)
                    val characteristicButton = service?.getCharacteristic(characteristicAckUUID)
                    characteristicButton?.let { enableNotifications(it) }
                }
                else -> {
                    Log.d("STATUS", "Service discovery failed: $status")
                }
            }
        }
        @Deprecated("Deprecated in Java")
        override fun onCharacteristicChanged(gatt: BluetoothGatt?, characteristic: BluetoothGattCharacteristic?) {
            if(characteristic?.uuid == characteristicAckUUID) {
                val value = characteristic?.getIntValue(BluetoothGattCharacteristic.FORMAT_UINT8, 0)
                if(value == 1) {
                    ack = 1
                    if(imageWifi) {
                        runOnUiThread {
                            binding.groupConnectingWifi.visibility = View.GONE
                            binding.wifiAvailable.visibility = View.GONE
                            binding.groupWifiConnected.visibility = View.VISIBLE
                            binding.connectedWifi.text =
                                getString(R.string.connectedTo) + " " + ssidClicked
                        }
                    }
                } else {
                    ack = 0
                    runOnUiThread {
                        binding.wifiAvailable.visibility = View.VISIBLE
                        binding.groupConnectingWifi.visibility = View.GONE
                        binding.groupWifiConnected.visibility = View.GONE
                        if(!ssidClicked.isNullOrEmpty()) {
                            showPasswordDialog(ssidClicked)
                        }
                    }
                }

                Log.d("Bluetooth", "Received value: $value")
            }
        }
    }

    @SuppressLint("MissingPermission")
    fun enableNotifications(characteristic: BluetoothGattCharacteristic) {
        val descriptor = characteristic.getDescriptor(configNotifications)
        descriptor.value = BluetoothGattDescriptor.ENABLE_NOTIFICATION_VALUE
        bluetoothGatt?.writeDescriptor(descriptor)
        bluetoothGatt?.setCharacteristicNotification(characteristic, true)
    }
    @SuppressLint("MissingPermission")
    fun disableNotifications(characteristic: BluetoothGattCharacteristic) {
        bluetoothGatt?.setCharacteristicNotification(characteristic, false)
    }

    @SuppressLint("MissingPermission")
    fun connectWifi(ssid: String, password: String) {
        val service = bluetoothGatt?.getService(serviceUUID)
        val characteristicSsid = service?.getCharacteristic(characteristicSsidUUID)
        val characteristicPassword = service?.getCharacteristic(characteristicPasswordUUID)
        characteristicSsid?.setValue(ssid)
        characteristicPassword?.setValue(password)
        bluetoothGatt?.writeCharacteristic(characteristicSsid)
        Handler().postDelayed({
            bluetoothGatt?.writeCharacteristic(characteristicPassword)
        }, 1000)
        runOnUiThread {
            binding.wifiAvailable.visibility = View.GONE
            binding.groupConnectingWifi.visibility = View.VISIBLE
            binding.connectingWifi.text = "Connecting to $ssidClicked"
        }
    }

    @SuppressLint("MissingPermission")
    fun disconnectWifi() {
        val service = bluetoothGatt?.getService(serviceUUID)
        val characteristicSsid = service?.getCharacteristic(characteristicSsidUUID)
        val characteristicPassword = service?.getCharacteristic(characteristicPasswordUUID)
        characteristicSsid?.setValue("")
        characteristicPassword?.setValue("")
        bluetoothGatt?.writeCharacteristic(characteristicSsid)
        Handler().postDelayed({
            bluetoothGatt?.writeCharacteristic(characteristicPassword)
        }, 1000)
        runOnUiThread {
            binding.wifiAvailable.visibility = View.VISIBLE
            binding.groupConnectingWifi.visibility = View.GONE
            binding.connectingWifi.text = "Connecting to $ssidClicked"
        }
    }
}