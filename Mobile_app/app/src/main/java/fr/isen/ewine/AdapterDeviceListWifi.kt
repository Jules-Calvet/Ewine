package fr.isen.ewine

import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.TextView
import androidx.recyclerview.widget.RecyclerView

class AdapterDevicesListWifi(var devices: ProfileActivity.WifiDevices, val onItemClickListener: (ProfileActivity.WifiDevices, Int) -> Unit) : RecyclerView.Adapter<AdapterDevicesListWifi.DevicesListViewHolder>() {

    class DevicesListViewHolder(view: View) : RecyclerView.ViewHolder(view) {
        val deviceName: TextView = view.findViewById(R.id.name)
    }

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): DevicesListViewHolder {
        val view = LayoutInflater.from(parent.context)
            .inflate(R.layout.list_devices, parent, false)

        return DevicesListViewHolder(view)
    }

    override fun onBindViewHolder(holder: DevicesListViewHolder, position: Int) {
        val name = devices.ssid[position]
        holder.deviceName.text = name.toString()

        holder.itemView.setOnClickListener {
            onItemClickListener(devices, position)
        }
    }

    override fun getItemCount(): Int = devices.size
}
