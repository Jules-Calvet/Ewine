package fr.isen.ewine

import android.content.Intent
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.recyclerview.widget.RecyclerView
import android.content.res.Resources
import android.widget.ImageView
import com.squareup.picasso.Picasso
import fr.isen.ewine.R.drawable
import fr.isen.ewine.model.UserData

class ColumnsAdapter (val cellarWidth: Int, val _tab_cellar: /*Array<Array<*/UserData/*>>*/, val y: Int/*, val onItemLongClickListener:(ColumnsViewHolder)->Unit*/) : RecyclerView.Adapter<ColumnsAdapter.ColumnsViewHolder>() {
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
        val x = position
        when (_tab_cellar/*[x][y]*/.cellarData[x+(y*cellarWidth)].bottleTypeOfWine) {
            "Red" -> {
                Picasso.get().load(drawable.bottle_red).into(holder.image)
            }
            "White" -> {
                Picasso.get().load(drawable.bottle_white).into(holder.image)
            }
            "Rose" -> {
                Picasso.get().load(drawable.bottle_rose).into(holder.image)
            }
            else -> Picasso.get().load(drawable.bottle_none).into(holder.image)
        }

        holder.image.setOnClickListener {
            // call the onItemLongClickListener callback with this holder
            val context = holder.itemView.context
            val intent = Intent(context, BottleActivity::class.java)
            intent.putExtra("x", x)
            intent.putExtra("y", y)
            context.startActivity(intent)
        }
    }
    override fun getItemCount(): Int = cellarWidth
}