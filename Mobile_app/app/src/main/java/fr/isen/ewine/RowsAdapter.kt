package fr.isen.ewine

import android.content.Context
import android.content.SharedPreferences
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import fr.isen.ewine.databinding.ItemRowsBinding

class RowsAdapter(val cellarHeight: Int,val cellarWidth: Int) : RecyclerView.Adapter<RowsAdapter.RowsViewHolder>() {
    class RowsViewHolder(val binding: ItemRowsBinding) : RecyclerView.ViewHolder(binding.root)

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): RowsViewHolder {
        val binding = ItemRowsBinding.inflate(LayoutInflater.from(parent.context), parent, false)

        binding.columnsRecyclerView.layoutManager = LinearLayoutManager(parent.context, LinearLayoutManager.HORIZONTAL, false)
        binding.columnsRecyclerView.adapter = ColumnsAdapter(cellarWidth)
        return RowsViewHolder(binding)
    }

    override fun onBindViewHolder(holder: RowsViewHolder, position: Int) {
        //val rows = cellarHeight
    }

    override fun getItemCount(): Int = cellarHeight
}