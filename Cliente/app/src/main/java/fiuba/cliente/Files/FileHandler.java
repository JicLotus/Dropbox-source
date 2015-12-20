package fiuba.cliente.Files;

import android.content.Context;
import android.os.Environment;
import android.util.Base64;
import android.widget.Toast;

import org.json.JSONArray;
import org.json.JSONObject;
import java.io.File;

import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;

import fiuba.cliente.model.Request;
import fiuba.cliente.model.RequestHandler;
import fiuba.cliente.model.Response;
import fiuba.cliente.model.Utils;

/**
 * Created by lotus on 6/09/15.
 */

public class FileHandler {


    private String userName;


    public FileHandler(String UserName)
    {
        userName=UserName;
    }

    public String createFolder(String name, int identifierFolder)
    {
        try {
            JSONObject data_file =buildJasonFile(name,"",2,identifierFolder);
            Request request = new Request("POST","files",data_file);
            Response resp = new RequestHandler().sendRequest(request);

            return "Carpeta creada correctamente.";
        }
        catch (Exception e){ return e.getMessage();}
    }

    public String uploadFile(String path,int identifierFolder)
    {
        File file = new File(path);
        try {
            JSONObject data_file =buildJasonFile(file.getName(),encondeBase64(file),1,identifierFolder);
            Request request = new Request("POST","files",data_file);
            Response resp = new RequestHandler().sendRequest(request);

            if (!resp.getStatus()) {
                return resp.get("message");
            }

            return file.getName() + " subido correctamente.";
        }
        catch (Exception e){ return e.getMessage();}
    }

    public boolean modifyFile(String path, int id, int identifierFolder, int version, boolean force)
    {
        File file = new File(path);
        try {
            JSONObject data_file =buildJasonFile(file.getName(),encondeBase64(file),1,identifierFolder);
            data_file.put("id", id);
            data_file.put("lastModifiedBy", userName);
            if (force) {
                data_file.put("forceCreation", true);
            }
            data_file.put("lastVersion", version);
            Request request = new Request("POST","files",data_file);
            Response resp = new RequestHandler().sendRequest(request);

            return resp.getStatus();
        } catch (Exception e){
            return false;
        }
    }

    public String downloadFile(String path,int indentifierFile, String token)
    {
        return downloadFile(path,indentifierFile,token,-1);
    }

    public String downloadFile(String path,int indentifierFile,String token,int version)
    {

        try {
            File file = new File(path);
            String textoRequest;
            if (version==-1)
                textoRequest="files/"+Integer.toString(indentifierFile)+"?token="+token;
            else
                textoRequest="files/" +Integer.toString(indentifierFile)+"/"+ Integer.toString(version) +"?token="+token;

            Request request = new Request("GET", String.format(textoRequest));
            Response resp = new RequestHandler().sendRequest(request);
            saveFile(file,resp.get("content"));
            return file.getName() + " descargado correctamente.";
        }
        catch(Exception e)
        {
            return e.getMessage();
        }
    }

    private String encondeBase64(File file) throws IOException {
        byte[] bytes = new byte[(int)file.length()];
        FileInputStream asd = new FileInputStream(file);
        asd.read(bytes);
        asd.close();
        String texto = new String(Base64.encode(bytes, Base64.DEFAULT));
        return texto;
    }

    private JSONObject buildJasonFile(String fileName,String textoBytesFile,int type,int identFolder)
    {
        try{
            JSONObject data_file = new JSONObject();
            data_file.put("name", fileName);
            data_file.put("extension", Utils.fileExtension(fileName));
            data_file.put("content", textoBytesFile);
            data_file.put("owner", userName);
            data_file.put("type",type);

            if (identFolder!=-1)
                data_file.put("father",identFolder);

            return data_file;
        }
        catch (Exception e)
        {
            return null;
        }
    }

    private void saveFile(File file,String textoBytes) throws IOException {
        byte[] bytes = Base64.decode(textoBytes, Base64.DEFAULT);
        FileOutputStream fop = new FileOutputStream(file);
        fop.write(bytes);
        fop.flush();
        fop.close();
    }

}
