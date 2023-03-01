package fr.isen.ewine

import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.TextView
import androidx.recyclerview.widget.RecyclerView

class RowsAdapter (val cellarHeight: Int) : RecyclerView.Adapter<RowsAdapter.ColumnsViewHolder>() {
    class ColumnsViewHolder(view : View) : RecyclerView.ViewHolder(view) {
    }

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): ColumnsViewHolder {
        val view = LayoutInflater.from(parent.context).inflate(R.layout.item_columns, parent, false)
        return ColumnsViewHolder(view)
    }

    override fun onBindViewHolder(holder: ColumnsViewHolder, position: Int) {
        val column = cellarHeight
    }
    override fun getItemCount(): Int = cellarHeight
}