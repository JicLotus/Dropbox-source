package fiuba.cliente.model;

import org.apache.http.HttpEntity;
import org.json.JSONObject;

import java.io.File;

/**
 * Created by kevin on 29/08/15.
 */
public class Request {
    private String path;
    private String method;
    private JSONObject args;
    private HttpEntity reqEntity = null;

    public Request(String method, String path, JSONObject args) {
        this.method = method;
        this.path = path;
        this.args = args;
    }

    public Request(String method, String path) {
        this.method = method;
        this.path = path;
        this.args = null;
    }

    public String getPath() {
        return path;
    }

    public JSONObject getArgs() {
        return args;
    }

    public String getMethod() {
        return method;
    }

    public void setFile(File file){

        try{
            //reqEntity = MultipartEntityBuilder.create().addBinaryBody("file", file, ContentType.create("image/jpeg"), file.getName()).build();
        }
        catch (Exception e) {
            reqEntity= null;
        }
    }

    public boolean haveFile()
    {
        return reqEntity!=null;
    }

    public HttpEntity getReqEntity()
    {
        return reqEntity;
    }

}
