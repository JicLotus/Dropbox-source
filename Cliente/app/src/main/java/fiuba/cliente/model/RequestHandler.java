package fiuba.cliente.model;

import org.apache.http.HttpEntity;
import org.apache.http.client.HttpClient;
import org.apache.http.HttpResponse;
import org.apache.http.client.methods.HttpDelete;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.client.methods.HttpPut;
import org.apache.http.entity.StringEntity;
import org.apache.http.impl.client.DefaultHttpClient;
import org.json.JSONException;
import org.json.JSONObject;
import org.json.JSONTokener;

import java.io.BufferedReader;
import java.io.InputStreamReader;

/**
 * Created by kevin on 26/08/15.
 */
public class RequestHandler {
    private Response response;

    private String ip = "http://192.168.0.20:8080/";

    public Response sendRequest(final Request request) {
        Thread thread = new Thread(new Runnable(){
            public void run() {
                try {
                    HttpClient httpclient = new DefaultHttpClient();
                    HttpResponse resp = null;
                    if (request.getMethod().equals("GET")) {
                        resp = httpclient.execute(new HttpGet(ip + request.getPath()));

                    } else {
                        if (request.getMethod().equals("DELETE")) {
                            resp = httpclient.execute(new HttpDelete(ip + request.getPath()));

                        }else if (request.getMethod().equals("POST")) {

                            HttpPost req = new HttpPost(ip + request.getPath());

                            JSONObject data = request.getArgs();
                            req.setEntity(new StringEntity(data.toString()));

                            resp = httpclient.execute(req);

                        }else if (request.getMethod().equals("PUT")) {

                            HttpPut req = new HttpPut(ip + request.getPath());

                            JSONObject data = request.getArgs();
                            req.setEntity(new StringEntity(data.toString()));

                            resp = httpclient.execute(req);
                        }

                    }

                    BufferedReader reader = new BufferedReader(new InputStreamReader(resp.getEntity().getContent(), "UTF-8"));

                    String json,line="";
                    StringBuilder builder = new StringBuilder();

                    while ((line= reader.readLine()) != null) {
                        builder.append(line);
                    }

                    json=builder.toString();

                    JSONTokener tokener = new JSONTokener(json);
                    JSONObject finalResult = new JSONObject(tokener);
                    response = new Response(finalResult);

                } catch (Exception e) {
                    try {
                        response =  new Response(new JSONObject("{\"result\":\"ERROR\", \"message\":\"Could not connect to server.\"}"));
                    } catch (JSONException exc) {
                        exc.printStackTrace();
                    }
                }
            }
        });
        try {
            thread.start();
            thread.join();
            return response;
        } catch (Exception e) {
            return null;
        }
    }

}