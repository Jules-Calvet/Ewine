package fr.isen.ewine

import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.recyclerview.widget.RecyclerView
import android.content.res.Resources
import android.widget.ImageView

class ColumnsAdapter (val cellarWidth: Int) : RecyclerView.Adapter<ColumnsAdapter.ColumnsViewHolder>() {
    class ColumnsViewHolder(view : View) : RecyclerView.ViewHolder(view) {

    }

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): ColumnsViewHolder {
        val view = LayoutInflater.from(parent.context).inflate(R.layout.item_columns, parent, false)

        val displayMetrics = Resources.getSystem().displayMetrics
        val screenWidthPx = displayMetrics.widthPixels

        val BottleWidthPx = screenWidthPx / cellarWidth
        val bottleView = view.findViewById<ImageView>(R.id.CellarImageView)
        bottleView.layoutParams.width = BottleWidthPx
        bottleView.layoutParams.height = 2 * BottleWidthPx
        bottleView.requestLayout()

        return ColumnsViewHolder(view)
    }

    override fun onBindViewHolder(holder: ColumnsViewHolder, position: Int) {

    }
    override fun getItemCount(): Int = cellarWidth
}