package fiuba.cliente.model;

import org.json.JSONArray;
import org.json.JSONObject;

import fiuba.cliente.Files.MetaFileHandler;

/**
 * Created by kevin on 10/10/15.
 */
public class SearchEngine {
    private MetaFileHandler metaFileHandler;

    public SearchEngine(MetaFileHandler metaFileHandler) {
        this.metaFileHandler = metaFileHandler;
    }

    public boolean searchFiles(int searchType, String searchValue) {
        switch (searchType) {
            case 1:
                return searchTag(searchValue);
            case 2:
                return searchExtension(searchValue);
            case 3:
                return  searchName(searchValue);
            case 4:
                return searchOwner(searchValue);
        }

        return  false;
    }

    public boolean searchInSingleFields(String fieldName, String valueToFind, boolean exactMatch) {
        JSONArray results = new JSONArray();
        searchInSingleFields(fieldName, valueToFind, results, metaFileHandler.getOwnFiles(), exactMatch);
        searchInSingleFields(fieldName, valueToFind, results, metaFileHandler.getSharedFiles(), exactMatch);
        metaFileHandler.updateVisibleFiles(results);
        return (results.length() > 0);
    }

    public void searchInSingleFields(String fieldName, String valueToFind, JSONArray results, JSONArray files, boolean exactMatch) {
        try {
            for (int i = 0; i < files.length(); i++) {
                JSONObject fileOrFolder = files.getJSONObject(i);
                if (fileOrFolder.getInt("type") == 2) {
                    searchInSingleFields(fieldName, valueToFind, results, fileOrFolder.getJSONArray("content"), exactMatch);
                }
                else {
                    if (exactMatch) {
                        if (fileOrFolder.get(fieldName).toString().equals(valueToFind)) {
                            results.put(fileOrFolder);
                        }
                    } else {
                        if (fileOrFolder.get(fieldName).toString().contains(valueToFind)) {
                            results.put(fileOrFolder);
                        }
                    }
                }
            }
        } catch (Exception e) {}
    }

    public boolean searchTag(String tag) {
        JSONArray results = new JSONArray();
        searchTag(tag, results, metaFileHandler.getOwnFiles());
        searchTag(tag, results, metaFileHandler.getSharedFiles());
        metaFileHandler.updateVisibleFiles(results);
        return (results.length() > 0);
    }

    public void searchTag(String tag, JSONArray results, JSONArray files) {
        try {
            for (int i = 0; i < files.length(); i++) {
                JSONObject fileOrFolder = files.getJSONObject(i);
                if (fileOrFolder.getInt("type") == 2) {
                    searchTag(tag, results, fileOrFolder.getJSONArray("content"));
                }
                else {
                    JSONArray tags = fileOrFolder.getJSONArray("tags");
                    if (Utils.jsonArrayToArrayList(tags).contains(tag)) {
                        results.put(fileOrFolder);
                    }
                }
            }
        } catch (Exception e) {}
    }

    public boolean searchExtension(String extension) {
        return searchInSingleFields("extension", extension, true);
    }

    public boolean searchName(String name) {
        return searchInSingleFields("name", name, false);
    }

    boolean searchOwner(String owner) {
        return searchInSingleFields("owner", owner, true);
    }


}
