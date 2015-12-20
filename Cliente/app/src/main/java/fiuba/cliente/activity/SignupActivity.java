package fiuba.cliente.activity;

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.view.ViewGroup;
import android.widget.EditText;
import android.widget.RelativeLayout;
import android.widget.TextView;
import android.widget.Toast;

import org.json.JSONObject;

import fiuba.cliente.R;
import fiuba.cliente.model.Request;
import fiuba.cliente.model.RequestHandler;
import fiuba.cliente.model.Response;
import fiuba.cliente.model.Utils;

public class SignupActivity extends AppCompatActivity {
    private RelativeLayout layout;
    private EditText username;
    private EditText name;
    private EditText lastName;
    private EditText email;
    private EditText password;
    private TextView errorMessage;

    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_signup);

        username = (EditText) findViewById(R.id.usernameForm);
        name = (EditText) findViewById(R.id.nameForm);
        lastName = (EditText) findViewById(R.id.lastNameForm);
        email = (EditText) findViewById(R.id.emailForm);
        password = (EditText) findViewById(R.id.passwordForm);
        //layout = (RelativeLayout) findViewById(R.id.layout);
    }

    public void cancel(View view) {
        finish();
    }

    public void signUp(View view) {
        JSONObject data = new JSONObject();
        String messageInvalidFields = checkInvalidFields();
        if (messageInvalidFields.isEmpty()) {
            try {

                data.put("identifier", username.getText().toString());
                data.put("name", name.getText().toString());
                data.put("lastName", lastName.getText().toString());
                data.put("email", email.getText().toString());
                data.put("md5", Utils.md5(password.getText().toString()));
            } catch (Exception e) {
                return;
            }

            Request request = new Request("POST", "users", data);
            Response resp = new RequestHandler().sendRequest(request);

            if (resp.getStatus()) {
                automaticSignIn();
            } else {
                Toast.makeText(getApplicationContext(), (String) resp.get("message"), Toast.LENGTH_LONG).show();
            }

            clearTextboxSignUp();
        }
        else
            Toast.makeText(getApplicationContext(), messageInvalidFields, Toast.LENGTH_LONG).show();
    }

    public String checkInvalidFields() {
        if (username.getText().toString().isEmpty())
            return "Invalid user";

        if (name.getText().toString().isEmpty())
            return "Invalid name";

        if (lastName.getText().toString().isEmpty())
            return "Invalid lastName";

        if (email.getText().toString().isEmpty())
            return "Invalid email";

        if (password.getText().toString().isEmpty())
            return "Invalid password";

        if (!Utils.isEmailValid(email.getText().toString()))
            return "Invalid email";

        return "";

    }

    public void automaticSignIn() {
        Request request = new Request("GET",
                "users/auth?identifier=" + username.getText().toString()
                + "&md5=" + Utils.md5(password.getText().toString()));


        Response resp = new RequestHandler().sendRequest(request);


        if (resp.getStatus()) {
            String token = resp.get("token");
            Intent documentsActivity = new Intent(this, DocumentsActivity.class);
            documentsActivity.putExtra("username", username.getText().toString());
            documentsActivity.putExtra("token", token);
            startActivity(documentsActivity);
        } else {
            showErrorMessage(resp.get("message"));
            Toast.makeText(getApplicationContext(), (String) resp.get("message"), Toast.LENGTH_LONG).show();
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

    public void clearTextboxSignUp() {
        username.setText("");
        name.setText("");
        lastName.setText("");
        email.setText("");
        password.setText("");
    }
}
