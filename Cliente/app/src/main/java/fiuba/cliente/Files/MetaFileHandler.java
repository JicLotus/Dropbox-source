package fiuba.cliente.Files;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.util.ArrayDeque;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;
import java.util.Stack;

import fiuba.cliente.model.Request;
import fiuba.cliente.model.RequestHandler;
import fiuba.cliente.model.Response;
import fiuba.cliente.model.Utils;

/**
 * Created by lotus on 15/09/15.
 */
public class MetaFileHandler {

    String token;
    public JSONObject userInfo;
    public JSONArray ownFiles;
    public JSONArray filesShowing;
    public JSONArray sharedFiles;
    public Stack<JSONArray> afterFolders;
    public ArrayDeque<Integer> idsPathToActualFolder;
    public ArrayDeque<String> namesPathToActualFolder;
    public String userName;

    int actualIdentifier=-1; //id de la carpeta abierta actualmente

    public MetaFileHandler(String Token,String UserName)
    {
        userName=UserName;
        token = Token;
        afterFolders = new Stack<JSONArray>();
        idsPathToActualFolder = new ArrayDeque<Integer>();
        namesPathToActualFolder = new ArrayDeque<String>();
    }

    // Se llama a esta funcion cuando se quiere abrir una carpeta, que esta en la posicion
    // index de la lista de archivos que se muestran
    public void updateIdentifier(int index)
    {
        // Item que se agrega al principio

        try {
            int newIdentifier = (int) filesShowing.getJSONObject(index).get("identifier");
            String newFolderName = filesShowing.getJSONObject(index).getString("name");
            if (!isAfterFolder(index)) {
                idsPathToActualFolder.offerLast(newIdentifier);
                namesPathToActualFolder.offerLast(newFolderName);
                JSONObject afterFolder_ = this.buildAfterJsonFolder();
                filesShowing = filesShowing.getJSONObject(index).getJSONArray("content");
                if (!haveAfterFolder(filesShowing)) {
                    filesShowing = Utils.addJsonObjectAsFirstElement(filesShowing, afterFolder_);
                }
            }
            else {
                idsPathToActualFolder.pollLast();
                namesPathToActualFolder.pollLast();
                filesShowing = afterFolders.pop();
            }
            actualIdentifier = newIdentifier;
        } catch (Exception e) {

        }
    }

    public void setSharedFilesVisible() {
        try {
            updateVisibleFiles(new JSONArray(sharedFiles.toString()));
        } catch (Exception e) {}
    }

    public void setDeletedFilesVisible() {
        try {
            JSONArray ownDeletedFiles = getDeletedFiles(new JSONArray(), ownFiles);
            JSONArray sharedDeletedFiles = getDeletedFiles(new JSONArray(), sharedFiles);
            JSONArray deletedFiles = Utils.concatArray(ownDeletedFiles, sharedDeletedFiles);
            updateVisibleFiles(new JSONArray(deletedFiles.toString()));
        } catch (Exception e) {}
    }

    public JSONArray getDeletedFiles(JSONArray deletedFiles,JSONArray files) {
        for(int i = 0; i < files.length(); i++) {
            try {
                JSONObject obj = (JSONObject) files.get(i);
                if (obj.getInt("type") == 2)
                    deletedFiles = getDeletedFiles(deletedFiles, obj.getJSONArray("content"));
                else if (obj.getBoolean("delete"))
                    deletedFiles.put(obj);
            } catch (Exception e) {}
        }
        return deletedFiles;

    }

    public String getPathActualFolder() {
        String path = "/";
        ArrayDeque<String> namesPath = getNamesPathToActualFolder();
        while (!namesPath.isEmpty()) {
            path += namesPath.poll() + "/";
        }
        return path;
    }

    public JSONArray getOwnFiles() {
        return ownFiles;
    }

    public JSONArray getSharedFiles() { return sharedFiles; }

    public ArrayDeque<Integer> getIdsPathToActualFolder() {
        return idsPathToActualFolder.clone();
    }

    public ArrayDeque<String> getNamesPathToActualFolder() {
        return namesPathToActualFolder.clone();
    }

    public void moveThroughPath(ArrayDeque<Integer> foldersIds) {
        while (!foldersIds.isEmpty()) {
            int indexNextFolder = getMetafileIndex(foldersIds.poll());
            updateIdentifier(indexNextFolder);
        }
    }

    public int getMetafileIndex(int metafileIdentifier) {
        for (int i = 0; i < filesShowing.length(); i++) {
            try {
                JSONObject fileOrFolder = filesShowing.getJSONObject(i);
                if ((int)fileOrFolder.get("identifier") == metafileIdentifier)
                    return i;
            } catch (Exception e) {}
        }
        return -1;
    }

    public void updateVisibleFiles(JSONArray files) {
        try {
            filesShowing = new JSONArray(files.toString());
        } catch (JSONException e) {
            e.printStackTrace();
        }
    }

    public boolean haveAfterFolder(JSONArray jsonArray)
    {
        try {
            for (int i=0;i<jsonArray.length();i++)
            {
                if(((JSONObject)jsonArray.get(i)).get("name")=="../")return true;
            }
        }
        catch(Exception e)
        {
            return false;
        }
        return false;
    }

    // Devuelve true si se eligio el item de la lista de archivos que no es una carpeta ni un
    // archivo, sino la opcion para volver a la carpeta anterior
    public boolean isAfterFolder(int index)
    {
        try {
            return filesShowing.getJSONObject(index).get("name").toString() == "../";
        }
        catch(Exception e)
        {
            return false;
        }
    }

    public JSONObject buildAfterJsonFolder()
    {
        try{
            JSONObject afterFile = new JSONObject();
            afterFile.put("name", "../");
            afterFile.put("type", 2);
            afterFile.put("extension", "");
            afterFile.put("identifier",actualIdentifier);
            afterFolders.push(filesShowing);
            return afterFile;
        }
        catch (Exception e)
        {
            return null;
        }
    }

    public boolean isMainRoot()
    {
        return actualIdentifier==-1;
    }

    public String requestMetaFiles()
    {
        try {

            Request request = new Request("GET","users/"+userName+"?token="+token);
            Response resp = new RequestHandler().sendRequest(request);

            ownFiles = resp.getJsonObject().getJSONArray("ownFiles");
            filesShowing = new JSONArray(resp.getJsonObject().getJSONArray("ownFiles").toString());
            sharedFiles = new JSONArray(resp.getJsonObject().getJSONArray("sharedFiles").toString());
            userInfo = resp.getJsonObject();
            afterFolders.clear();
            idsPathToActualFolder.clear();
            namesPathToActualFolder.clear();
            actualIdentifier = -1;

            return "Logueado";
        }
        catch (Exception e){ return e.getMessage();}

    }

    public int requestQuotaMB() {
        try {

            Request request = new Request("GET","users/"+userName+"?token="+token);
            Response resp = new RequestHandler().sendRequest(request);

            return resp.getJsonObject().getInt("quotaMB");
        }
        catch (Exception e){ return -1;}
    }

    public void updateUserInfo() {
        Request request = new Request("GET","users/"+userName+"?token="+token);
        Response resp = new RequestHandler().sendRequest(request);
        userInfo = resp.getJsonObject();
    }


    public String[] getEmpytArrayString()
    {
        return new String[filesShowing.length()];
    }

    public String getMetaFileName(int index)
    {
        try {
            return filesShowing.getJSONObject(index).get("name").toString();
        }
        catch (Exception e){ return "";}
    }

    public String getMetaFileExtension(int index)
    {
        try {
            return filesShowing.getJSONObject(index).get("extension").toString();
        }
        catch (Exception e){ return "";}
    }

    public int getMetaFileIdentifier(int index)
    {
        try {
            return (int)filesShowing.getJSONObject(index).get("identifier");
        }
        catch (Exception e){ return -1;}
    }

    public int getMetafileSize(int index) {
        try {
            return filesShowing.getJSONObject(index).getInt("sizeMB");
        }
        catch (Exception e){ return -1;}
    }

    public boolean getMetafileIsDeleted(int index) {
        try {
            return filesShowing.getJSONObject(index).getBoolean("delete");
        }
        catch (Exception e){ return false;}
    }

    public String getMetafileOwner(int index) {
        try {
            return filesShowing.getJSONObject(index).getString("owner");
        }catch (Exception e) {
            return null;
        }
    }

    public int getMetafileFolder(int idMetafile) {
        if (folderContainsMetafile(idMetafile, ownFiles)) {
            return -1;
        }
        return getMetafileFolder(idMetafile, ownFiles, -1);
    }

    public int getMetafileFolder(int idMetafile, JSONArray actualFolder, int idActualFolder) {
        try {

            if (!folderContainsMetafile(idMetafile, actualFolder)) {
                ArrayList<Integer> r = new ArrayList<Integer>();
                for (int i = 0; i < actualFolder.length(); i++) {
                    JSONObject fileOrFolder = actualFolder.getJSONObject(i);
                    if (fileOrFolder.getInt("type") == 2) {
                        int nextFolderIdentifier = (int)fileOrFolder.get("identifier");
                        r.add(getMetafileFolder(idMetafile, fileOrFolder.getJSONArray("content"), nextFolderIdentifier));
                    }
                }
                if (r.size() == 0) return -2;
                Collections.sort(r);
                return r.get(r.size()-1);
            } else {
                return idActualFolder;
            }
        } catch (Exception e) {return -2;}
    }

    public boolean folderContainsMetafile(int idMetafile, JSONArray folder) {
        try {
            for (int i = 0; i < folder.length(); i++) {
                JSONObject fileOrFolder = folder.getJSONObject(i);
                if (fileOrFolder.getInt("type") == 1) {
                    if ((int)fileOrFolder.get("identifier") == idMetafile) {
                        return true;
                    }
                }
            }
            return false;
        } catch (Exception e) {return false;}
    }

    public boolean isFolder(int index)
    {
        try {
            return (int)(filesShowing.getJSONObject(index).get("type"))==2;
        }
        catch (Exception e){ return false;}
    }

    public CharSequence[] getVersions(int index)
    {
        try{
            JSONArray versiones =filesShowing.getJSONObject(index).getJSONArray("versions");
            int len = versiones.length();
            CharSequence optionFiles[];
            optionFiles = new CharSequence[len];

            for (int i=0;i<len;i++){
                optionFiles[i] = ((JSONObject)versiones.get(i)).get("version").toString();
            }

            return optionFiles;
        }
        catch(Exception e)
        {
            return null;
        }
    }

    public CharSequence[] getVersionsDate(int index)
    {
        try{
            JSONArray versiones =filesShowing.getJSONObject(index).getJSONArray("versions");
            int len = versiones.length();
            CharSequence optionFiles[];
            optionFiles = new CharSequence[len];

            for (int i=0;i<len;i++){
                optionFiles[i] = ((JSONObject)versiones.get(i)).get("version").toString() + " (" + Utils.timestampToDate((int)((JSONObject)versiones.get(i)).get("date")) + ")";
            }

            return optionFiles;
        }
        catch(Exception e)
        {
            return null;
        }
    }

    public int getActualIdentifier()
    {
        return actualIdentifier;
    }

    public void setActualIdentifier(int ActualIdentifier){
        actualIdentifier = ActualIdentifier;
    }

    public String getUserDetails() {
        return userInfo.toString();
    }

    public boolean setMetafileDeleted(int index) {
        int idMetafile = getMetaFileIdentifier(index);
        try {

            Request request = new Request("DELETE", "metafiles/" + idMetafile + "?token=" + token);
            Response resp = new RequestHandler().sendRequest(request);

            return resp.getStatus();
        } catch (Exception e) {
            return false;
        }
    }

    public CharSequence[] fileDetails(int index)
    {
        CharSequence filedetails[];
        filedetails = new CharSequence[9];

        try {
            filedetails[0]= "Nombre: " + filesShowing.getJSONObject(index).getString("name");
            filedetails[1]= "Extension: " + filesShowing.getJSONObject(index).getString("extension");
            filedetails[2]= "Ultima modificacion: " + Utils.timestampToDate(filesShowing.getJSONObject(index).getInt("lastModifiedOn"));
            filedetails[3]= "Creado por: " + filesShowing.getJSONObject(index).getString("owner");
            filedetails[4]= "Modificado por: " + filesShowing.getJSONObject(index).getString("lastModifiedBy");
            filedetails[5]= "Size: " + filesShowing.getJSONObject(index).getString("sizeMB");
            filedetails[6]= "Modificar nombre";
            filedetails[7]= "Agregar Tag";
            filedetails[8]= "Ver Tags";

        } catch (Exception e) {}

        return filedetails;
    }

    public boolean shareFile(int index, String usersStr) {
        try {
            int idMetafile = getMetaFileIdentifier(index);
            JSONObject data = new JSONObject();
            JSONArray users = new JSONArray(Arrays.asList(usersStr.split("\\s*,\\s*")));
            data.put("users", users);

            Request request = new Request("POST", "metafiles/share/" + idMetafile + "?token=" + token, data);
            Response resp = new RequestHandler().sendRequest(request);

            return resp.getStatus();
        } catch (Exception e) {
            return false;
        }
    }

    public boolean unshareFile(int index, String user) {
        try {
            int idMetafile = getMetaFileIdentifier(index);

            Request request = new Request("DELETE", "metafiles/share/" + idMetafile + "?user=" + user);
            Response resp = new RequestHandler().sendRequest(request);

            return resp.getStatus();
        } catch (Exception e) {
            return false;
        }
    }

    public String addNewTag(String tag, int idMetafile) {
        try {
            JSONObject data_metafile = new JSONObject();
            data_metafile.put("id", idMetafile);
            JSONArray currentTags = getMetafileTags(getMetafileIndex(idMetafile));
            currentTags.put(tag);
            data_metafile.put("tags", currentTags);
            Request request = new Request("PUT","metafiles/" + idMetafile, data_metafile);
            new RequestHandler().sendRequest(request);

            return "Tag agregado correctamente";

        } catch (Exception e){
            return e.getMessage();
        }
    }


    public String deleteTag(int idTag, int idMetafile)
    {
        try {
            JSONObject data_metafile = new JSONObject();
            data_metafile.put("id", idMetafile);
            JSONArray currentTags = getMetafileTags(getMetafileIndex(idMetafile));

            currentTags = Utils.removeElementInJsonArray(currentTags, idTag);

            data_metafile.put("tags", currentTags);
            Request request = new Request("PUT","metafiles/" + idMetafile, data_metafile);
            new RequestHandler().sendRequest(request);

            return "Tag eliminado correctamente";

        } catch (Exception e) {
            return e.getMessage();
        }

    }


    public String setLongitudeLatitude(double longitude,double latitude) {
        try {
            JSONObject data_metafile = new JSONObject();
            data_metafile.put("identifier", userName);
            data_metafile.put("lastLatitude", "" + latitude);
            data_metafile.put("lastLongitude", ""+longitude);
            data_metafile.put("token", token);

            Request request = new Request("PUT","users/" + userName + "?token="+ token, data_metafile);
            new RequestHandler().sendRequest(request);

            return "";

        } catch (Exception e) {
            return e.getMessage();
        }
    }


    public String recoverFile(int idMetafile) {
        try {
            JSONObject data_metafile = new JSONObject();
            data_metafile.put("id", idMetafile);

            data_metafile.put("delete", 0);
            Request requestMetafile = new Request("PUT","metafiles/" + idMetafile, data_metafile);
            new RequestHandler().sendRequest(requestMetafile);

            JSONObject data_user = new JSONObject();
            data_user.put("identifier", userName);
            data_user.put("token", token);

            int newQuota = requestQuotaMB() + getMetafileSize(getMetafileIndex(idMetafile));
            data_user.put("quotaMB", newQuota);

            Request requestUser = new Request("PUT","users/" + userName, data_user);
            new RequestHandler().sendRequest(requestUser);

            return "Archivo restaurado correctamente";

        } catch (Exception e) {
            return e.getMessage();
        }
    }


    public String changeFileName(String fileName,int idMetafile)
    {
        try {
            JSONObject data_metafile = new JSONObject();
            data_metafile.put("id", idMetafile);
            data_metafile.put("name", (String)fileName);
            Request request = new Request("PUT","metafiles/" + idMetafile, data_metafile);
            new RequestHandler().sendRequest(request);

            return "Nombre cambiado correctamente";

        } catch (Exception e){
            return e.getMessage();
        }
    }

    public JSONArray getMetafileTags(int index) {
        try {
            return new JSONArray(filesShowing.getJSONObject(index).getJSONArray("tags").toString());
        } catch (Exception e) {
            return null;
        }
    }

    public CharSequence[] getTags(int index) {
        try {
            JSONArray tags = getMetafileTags(index);
            int len = tags.length();
            CharSequence optionTags[];
            optionTags = new CharSequence[len];

            for (int i = 0; i < len; i++) {
                optionTags[i] = tags.getString(i);
            }

            return optionTags;
        } catch (Exception e) {
            return null;
        }
    }

}
