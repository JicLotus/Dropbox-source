package fiuba.cliente.model;

import org.json.JSONObject;

/**
 * Created by kevin on 29/08/15.
 */
public class Response {
    private boolean status;
    private JSONObject jsonResponse;

    public Response(JSONObject response) {
        this.jsonResponse = response;
        try {
            this.status = response.getString("result").equals("OK");
        } catch (Exception e) {
            this.status = false;
        }
    }

    public boolean getStatus() {
        return status;
    }

    public String get(String key) {
        try {
            return (String) jsonResponse.get(key);
        } catch (Exception e) {
            return null;
        }
    }

    public JSONObject getJsonObject(){return jsonResponse;}

}
