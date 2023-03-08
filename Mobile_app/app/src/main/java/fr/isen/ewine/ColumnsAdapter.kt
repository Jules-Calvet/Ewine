package fr.isen.ewine

import android.content.Context
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.recyclerview.widget.RecyclerView
import android.content.res.Resources
import android.util.Log
import android.widget.ImageView
import androidx.appcompat.content.res.AppCompatResources.getDrawable
import com.squareup.picasso.Picasso
import fr.isen.ewine.R.drawable

class ColumnsAdapter (val context: Context, val cellarHeight: Int, val cellarWidth: Int, val _tab_cellar: Array<Array<String>>, val y: Int) : RecyclerView.Adapter<ColumnsAdapter.ColumnsViewHolder>() {
    class ColumnsViewHolder(view : View) : RecyclerView.ViewHolder(view) {
        var image : ImageView = view.findViewById<ImageView>(R.id.CellarImageView)
    }

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): ColumnsViewHolder {
        val view = LayoutInflater.from(parent.context).inflate(R.layout.item_columns, parent, false)

        val displayMetrics = Resources.getSystem().displayMetrics
        val screenWidthPx = displayMetrics.widthPixels
        val bottleWidthPx = screenWidthPx / cellarWidth
        val bottleView = view.findViewById<ImageView>(R.id.CellarImageView)
        bottleView.layoutParams.width = bottleWidthPx
        bottleView.layoutParams.height = 2 * bottleWidthPx
        bottleView.requestLayout()
        return ColumnsViewHolder(view)
    }
    override fun onBindViewHolder(holder: ColumnsViewHolder, position: Int) {
        val x = position % cellarWidth

        when (_tab_cellar[x][y]) {
            "Red" -> Picasso.get().load(drawable.bottle_red).into(holder.image)
            "White" -> Picasso.get().load(drawable.bottle_white).into(holder.image)
            "Rose" -> Picasso.get().load(drawable.bottle_rose).into(holder.image)
            else -> Picasso.get().load(drawable.bottle_none).into(holder.image)
        }
    }
    override fun getItemCount(): Int = cellarWidth
}