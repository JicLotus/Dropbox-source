package fiuba.cliente.activity;

import android.content.Intent;
import android.location.Location;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.EditText;
import android.view.View;
import android.widget.RelativeLayout;
import android.widget.TextView;
import android.widget.Toast;

import fiuba.cliente.R;
import fiuba.cliente.model.Request;
import fiuba.cliente.model.RequestHandler;
import fiuba.cliente.model.Response;
import fiuba.cliente.model.Utils;
import fiuba.cliente.model.gpsLocation;


public class MainActivity extends AppCompatActivity {
    private RelativeLayout layout;
    private EditText username;
    private EditText password;
    private TextView errorMessage;



    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        username = (EditText) findViewById(R.id.username);
        password = (EditText) findViewById(R.id.password);
        //layout = (RelativeLayout) findViewById(R.id.layout);
    }


    public void logIn(View view) {
        try {
            if (errorMessage != null) {
                layout.removeView(errorMessage);
            }
            if (username.getText().toString().matches("") || password.getText().toString().matches("")) {
                showErrorMessage("Invalid user or password");
                return;
            }

            Request request = new Request("GET",
                    "users/auth?identifier=" + username.getText().toString()
                    + "&md5=" + Utils.md5(password.getText().toString()));


            Response resp = new RequestHandler().sendRequest(request);

            if (resp.getStatus()) {
                String token = resp.get("token");

                Intent documentsActivity = new Intent(this, DocumentsActivity.class);
                documentsActivity.putExtra("token", token);
                documentsActivity.putExtra("username", username.getText().toString());
                startActivity(documentsActivity);
             } else {
                showErrorMessage(resp.get("message"));
                //Toast.makeText(getApplicationContext(), (String) resp.get("message"), Toast.LENGTH_LONG).show();
             }
            clearTextboxLogin();

        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private void showErrorMessage(String message) {
        /*errorMessage = new TextView(this);
        errorMessage.setId(1);
        errorMessage.setText(message);

        RelativeLayout.LayoutParams params1 = new RelativeLayout.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT, ViewGroup.LayoutParams.WRAP_CONTENT);
        params1.addRule(RelativeLayout.ABOVE, username.getId());
        params1.addRule(RelativeLayout.ALIGN_LEFT, username.getId());
        layout.addView(errorMessage, params1);*/
        Toast.makeText(this,message,Toast.LENGTH_LONG).show();
    }

    public void signUp(View view) {
        Intent signupActivity = new Intent(this, SignupActivity.class);
        startActivity(signupActivity);

    }

    public void clearTextboxLogin() {
        username.setText("");
        password.setText("");
    }

}