package fr.isen.ewine

import android.content.Context
import android.util.Log
import android.view.LayoutInflater
import android.view.ViewGroup
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import fr.isen.ewine.databinding.ItemRowsBinding

class RowsAdapter(val context: Context, val cellarHeight: Int, val cellarWidth: Int, val _tab_cellar: Array<Array<String>>) : RecyclerView.Adapter<RowsAdapter.RowsViewHolder>() {
    class RowsViewHolder(val binding: ItemRowsBinding) : RecyclerView.ViewHolder(binding.root)

    var y : Int = 0
    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): RowsViewHolder {
        val binding = ItemRowsBinding.inflate(LayoutInflater.from(parent.context), parent, false)
        binding.columnsRecyclerView.layoutManager = LinearLayoutManager(parent.context, LinearLayoutManager.HORIZONTAL, false)

        binding.columnsRecyclerView.adapter = ColumnsAdapter(context,cellarHeight,cellarWidth,_tab_cellar,y)
        return RowsViewHolder(binding)
    }

    override fun onBindViewHolder(holder: RowsViewHolder, position: Int) {
        y = position % cellarHeight
    }

    override fun getItemCount(): Int = cellarHeight
}