package fiuba.cliente.activity;

import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.Uri;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Base64;
import android.view.View;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.RelativeLayout;
import android.widget.TextView;
import android.widget.Toast;

import org.json.JSONObject;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;

import fiuba.cliente.R;
import fiuba.cliente.model.Request;
import fiuba.cliente.model.RequestHandler;
import fiuba.cliente.model.Response;
import fiuba.cliente.model.Utils;

public class UserDetails extends AppCompatActivity {
    private JSONObject details;
    private String userName;
    private EditText name;
    private EditText lastName;
    private EditText email;
    private TextView quota;
    private TextView lastLatitude;
    private TextView lastLongitude;
    private String avatar;
    String token;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_user_details);


        try {
            details = new JSONObject(getIntent().getStringExtra("details"));
            token = getIntent().getStringExtra("token");
            userName = details.getString("identifier");
            name = (EditText) findViewById(R.id.nameForm);
            name.setText(details.getString("name"));
            lastName = (EditText) findViewById(R.id.lastNameForm);
            lastName.setText(details.getString("lastName"));
            email = (EditText) findViewById(R.id.emailForm);
            email.setText(details.getString("email"));

            lastLatitude = (TextView) findViewById(R.id.latForm);
            lastLatitude.setText(details.getString("lastLatitude"));

            lastLongitude = (TextView) findViewById(R.id.longForm);
            lastLongitude.setText(details.getString("lastLongitude"));

            quota = (TextView) findViewById(R.id.quotaForm);
            int Currentquota = details.getInt("quotaMB");
            int quotaLimit = details.getInt("quotaLimitMB");
            quota.setText(Integer.toString(Currentquota) + " / " + Integer.toString(quotaLimit));

            String profilePicture = details.getString("avatar");
            if (!profilePicture.equals("")) {
                changePicture(profilePicture);
            }
        } catch (Exception e) {
            finish();
        }

    }

    public void changePicture(String pic) {
        ImageView img = (ImageView) findViewById(R.id.avatar);
        byte[] bytes = Base64.decode(pic, Base64.DEFAULT);
        Bitmap bm = BitmapFactory.decodeByteArray(bytes, 0 , bytes.length);
        img.setImageBitmap(bm);
    }

    public void cancel(View view) {
        finish();
    }

    public void save(View view) {
        String messageInvalidFields = checkInvalidFields();
        if (messageInvalidFields.isEmpty()) {
            JSONObject dataUser = buildJSONUser();

            Request request = new Request("PUT", "users/" + userName, dataUser);
            Response resp = new RequestHandler().sendRequest(request);

            if (resp.getStatus()) {
                finish();
            } else {
                Toast.makeText(getApplicationContext(), (String) resp.get("message"), Toast.LENGTH_LONG).show();
            }
        }
    }


    public JSONObject buildJSONUser() {
        try {
            JSONObject data = new JSONObject();
            data.put("name", name.getText().toString());
            data.put("lastName", lastName.getText().toString());
            data.put("email", email.getText().toString());
            data.put("token", token);
            data.put("avatar", avatar);
            return  data;
        } catch (Exception e) {
            return  null;
        }
    }

    public String checkInvalidFields() {
        if (name.getText().toString().isEmpty())
            return "Invalid name";

        if (lastName.getText().toString().isEmpty())
            return "Invalid lastName";

        if (email.getText().toString().isEmpty())
            return "Invalid email";

        if (!Utils.isEmailValid(email.getText().toString()))
            return "Invalid email";

        return "";

    }

    public void changeAvatar(View v) {
        Intent intent = new Intent(Intent.ACTION_GET_CONTENT);
        intent.setType("image/*");
        intent.addCategory(Intent.CATEGORY_OPENABLE);

        try {
            startActivityForResult(
                    Intent.createChooser(intent, "Select a File to Upload"),
                    1);
        } catch (android.content.ActivityNotFoundException ex) {
            // Potentially direct the user to the Market with a Dialog
            Toast.makeText(this, "Please install a File Manager.",
                    Toast.LENGTH_SHORT).show();
        }
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {

        switch (requestCode) {
            case 1:
                if (resultCode == RESULT_OK) {

                    try {
                        Uri uri = data.getData();
                        String path = Utils.getPath(this, uri);
                        File file = new File(path);
                        avatar = encondeBase64(file);
                        changePicture(avatar);

                    }
                    catch(Exception e){
                        Toast.makeText(getApplicationContext(), "No se pudo seleccionar el directorio", Toast.LENGTH_LONG).show();
                    }
                }
                break;
        }
        super.onActivityResult(requestCode, resultCode, data);
    }

    private String encondeBase64(File file) throws IOException {
        byte[] bytes = new byte[(int)file.length()];
        FileInputStream asd = new FileInputStream(file);
        asd.read(bytes);
        asd.close();
        String texto = new String(Base64.encode(bytes, Base64.DEFAULT));
        return texto;
    }
}
