package fr.isen.ewine

import android.view.LayoutInflater
import android.view.ViewGroup
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import fr.isen.ewine.databinding.ItemRowsBinding

class RowsAdapter(val cellarHeight: Int, val cellarWidth: Int, val _tab_cellar: Array<Array<String>>/*, val onItemLongClickListener:()->Unit*/) : RecyclerView.Adapter<RowsAdapter.RowsViewHolder>() {
    private var y:Int = 0
    class RowsViewHolder(binding: ItemRowsBinding) : RecyclerView.ViewHolder(binding.root)
    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): RowsViewHolder {
        val binding = ItemRowsBinding.inflate(LayoutInflater.from(parent.context), parent, false)
        binding.columnsRecyclerView.layoutManager = LinearLayoutManager(parent.context, LinearLayoutManager.HORIZONTAL, false)

        binding.columnsRecyclerView.adapter = ColumnsAdapter(cellarWidth,_tab_cellar,y)
        return RowsViewHolder(binding)
    }

    override fun onBindViewHolder(holder: RowsViewHolder, position: Int) {
        y = position+1
    }

    override fun getItemCount(): Int = cellarHeight
}