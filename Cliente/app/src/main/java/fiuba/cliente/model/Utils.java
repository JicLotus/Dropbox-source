package fiuba.cliente.model;

import android.content.Context;
import android.database.Cursor;
import android.net.Uri;

import org.json.JSONArray;
import org.json.JSONObject;

import java.net.URISyntaxException;
import java.security.MessageDigest;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * Created by kevin on 04/09/15.
 */
public class Utils {
    private static Utils ourInstance = new Utils();

    public static Utils getInstance() {
        return ourInstance;
    }

    private Utils() {
    }

    public static String md5(String s) {
        try {
            MessageDigest digest = java.security.MessageDigest.getInstance("MD5");
            digest.update(s.getBytes());
            byte messageDigest[] = digest.digest();

            StringBuffer hexString = new StringBuffer();
            for (int i=0; i<messageDigest.length; i++)
                hexString.append(Integer.toHexString(0xFF & messageDigest[i]));
            return hexString.toString();

        } catch (Exception e) {
            e.printStackTrace();
        }
        return "";
    }

    public static String getPath(Context context, Uri uri) throws URISyntaxException {
        if ("content".equalsIgnoreCase(uri.getScheme())) {
            String[] projection = { "_data" };
            Cursor cursor = null;

            try {
                cursor = context.getContentResolver().query(uri, projection, null, null, null);
                int column_index = cursor.getColumnIndexOrThrow("_data");
                if (cursor.moveToFirst()) {
                    return cursor.getString(column_index);
                }
            } catch (Exception e) {
                // Eat it
            }
        }
        else if ("file".equalsIgnoreCase(uri.getScheme())) {
            return uri.getPath();
        }

        return null;
    }

    public static boolean isEmailValid(String email){
        String PATTERN_EMAIL = "^[_A-Za-z0-9-\\+]+(\\.[_A-Za-z0-9-]+)*@"
                + "[A-Za-z0-9-]+(\\.[A-Za-z0-9]+)*(\\.[A-Za-z]{2,})$";
        Pattern pattern = Pattern.compile(PATTERN_EMAIL);
        Matcher matcher = pattern.matcher(email);
        return matcher.matches();
    }


    public static String fileExtension(String fileName){
        String extension = "";

        int i = fileName.lastIndexOf('.');
        if (i > 0) {
            extension = fileName.substring(i+1);
        }

        return extension;
    }

    public static String timestampToDate(int timestamp){
        SimpleDateFormat sdf = new SimpleDateFormat("dd/MM/yyyy HH:mm:ss");
        Date date = new Date((long) timestamp*1000);
        return sdf.format(date);
    }

    public static ArrayList<String> jsonArrayToArrayList(JSONArray jsonArray) {

        ArrayList<String> arrayList = new ArrayList<String>();
        for (int i = 0; i < jsonArray.length(); i++) {
            try {
                arrayList.add(jsonArray.get(i).toString());
            } catch (Exception e) {}

        }
        return arrayList;
    }

    public static JSONArray copyJsonArray(JSONArray jsonArray) {
        JSONArray newJsonArray = new JSONArray();
        for (int i = 0; i < jsonArray.length(); i++) {
            try {
                newJsonArray.put(jsonArray.get(i));
            } catch ( Exception e) {}

        }
        return newJsonArray;
    }

    public static JSONArray addJsonObjectAsFirstElement(JSONArray jsonArray, JSONObject newElement) {
        JSONArray newJsonArray = new JSONArray();
        try {
            newJsonArray.put(newElement);
            for (int i = 0; i < jsonArray.length(); i++) {
                newJsonArray.put(jsonArray.get(i));
            }
        } catch (Exception e) {}
        return newJsonArray;
    }

    public static JSONArray removeElementInJsonArray(JSONArray jsonArrayParam,int index)
    {
        JSONArray jsonArray = new JSONArray();
        try{

            for (int i=0;i<jsonArrayParam.length();i++)
            {
                if (i!= index) jsonArray.put(jsonArrayParam.get(i));
            }

        } catch (Exception e) {
            return jsonArray;
        }

        return jsonArray;
    }

    public static JSONArray concatArray(JSONArray arr1, JSONArray arr2) {
        JSONArray result = new JSONArray();
        try {
            for (int i = 0; i < arr1.length(); i++) {
                result.put(arr1.get(i));
            }
            for (int i = 0; i < arr2.length(); i++) {
                result.put(arr2.get(i));
            }
        } catch (Exception e) {}
        return result;
    }
}

