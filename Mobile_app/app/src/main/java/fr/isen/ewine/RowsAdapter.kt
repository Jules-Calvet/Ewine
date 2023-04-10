package fr.isen.ewine

import android.view.LayoutInflater
import android.view.ViewGroup
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import fr.isen.ewine.databinding.ItemRowsBinding
import fr.isen.ewine.model.UserData

class RowsAdapter(val cellarHeight: Int, val cellarWidth: Int, val _tab_cellar: UserData) : RecyclerView.Adapter<RowsAdapter.RowsViewHolder>() {
    class RowsViewHolder(binding: ItemRowsBinding) : RecyclerView.ViewHolder(binding.root){
        val RV : RecyclerView = binding.columnsRecyclerView
    }
    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): RowsViewHolder {
        val binding = ItemRowsBinding.inflate(LayoutInflater.from(parent.context), parent, false)
        return RowsViewHolder(binding)
    }

    override fun onBindViewHolder(holder: RowsViewHolder, position: Int) {
        val LayoutManager = LinearLayoutManager(holder.RV.context, RecyclerView.HORIZONTAL, false)
        holder.RV.apply {
            layoutManager = LayoutManager
            adapter = ColumnsAdapter(cellarWidth,_tab_cellar,position)
        }
    }

    override fun getItemCount(): Int = cellarHeight
}