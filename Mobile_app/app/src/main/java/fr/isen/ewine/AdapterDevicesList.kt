package fr.isen.ewine

import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.TextView
import androidx.recyclerview.widget.RecyclerView

class AdapterDevicesList(var devices: ProfileActivity.Devices, val onItemClickListener: (ProfileActivity.Devices, Int) -> Unit) : RecyclerView.Adapter<AdapterDevicesList.DevicesListViewHolder>() {
    class DevicesListViewHolder(view: View) : RecyclerView.ViewHolder(view) {
        val deviceName: TextView = view.findViewById(R.id.name)
    }

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): DevicesListViewHolder {
        val view = LayoutInflater.from(parent.context)
            .inflate(R.layout.list_devices, parent, false)

        return DevicesListViewHolder(view)
    }

    override fun onBindViewHolder(holder: DevicesListViewHolder, position: Int) {
        val name = devices.device_name[position]
        holder.deviceName.text = name

        holder.itemView.setOnClickListener {
            onItemClickListener(devices, position)
        }
    }

    override fun getItemCount(): Int = devices.size
}
