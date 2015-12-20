package fiuba.cliente.model;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.ImageView;
import android.widget.TextView;

import fiuba.cliente.Files.MetaFileHandler;
import fiuba.cliente.R;
import fiuba.cliente.activity.DocumentsActivity;

/**
 * Created by lotus on 16/09/15.
 */

public class IconArrayAdapter extends ArrayAdapter<String> {

    private final Context context;
    private MetaFileHandler metaFileHandler;


    public IconArrayAdapter(Context context, String[] values) {
        super(context, R.layout.activity_documents, values);
        this.context = context;
    }

    public void setMetaFiles(MetaFileHandler MetaFileHandlerParam)
    {
        metaFileHandler = MetaFileHandlerParam;
    }


    @Override
    public View getView(int position, View convertView, ViewGroup parent) {
        LayoutInflater inflater = (LayoutInflater) context
                .getSystemService(Context.LAYOUT_INFLATER_SERVICE);

        View rowView = inflater.inflate(R.layout.activity_documents, parent, false);
        TextView textView = (TextView) rowView.findViewById(R.id.label);
        ImageView imageView = (ImageView) rowView.findViewById(R.id.logo);

        if (((DocumentsActivity)context).isDeletedFilesMode() || !metaFileHandler.getMetafileIsDeleted(position)) {
            String nameToShow = metaFileHandler.getMetaFileName(position);

            if (!metaFileHandler.isAfterFolder(position) && !metaFileHandler.isFolder(position))
                nameToShow += " - " + metaFileHandler.getMetafileSize(position) + " KB";

            textView.setText(nameToShow);

            boolean isFolder = metaFileHandler.isFolder(position);
            String extension = metaFileHandler.getMetaFileExtension(position);

            if (isFolder) {
                imageView.setImageResource(R.drawable.folder);
            } else {
                if (extension.toUpperCase().equals("JPG")) {
                    imageView.setImageResource(R.drawable.jpg);
                } else if (extension.toUpperCase().equals("GIF")) {
                    imageView.setImageResource(R.drawable.gif);
                } else if (extension.toUpperCase().equals("PDF")) {
                    imageView.setImageResource(R.drawable.pdf);
                } else if (extension.toUpperCase().equals("TXT")) {
                    imageView.setImageResource(R.drawable.txt);
                } else {
                    imageView.setImageResource(R.drawable.txt);
                }
            }
        } else {
            return inflater.inflate(R.layout.null_item, null);
        }

        return rowView;
    }

    @Override
    public String getItem(int position)
    {
        return "";
    }

}
