package fiuba.cliente.activity;

import android.app.AlertDialog;
import android.app.ListActivity;

import android.content.DialogInterface;
import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;

import android.view.Menu;

import android.view.MenuItem;
import android.view.View;

import android.widget.AdapterView;
import android.widget.EditText;
import android.widget.FrameLayout;
import android.widget.ImageButton;
import android.widget.ListView;
import android.widget.RelativeLayout;

import android.widget.TextView;
import android.widget.Toast;
import android.view.LayoutInflater;

import java.io.File;
import java.util.ArrayDeque;

import fiuba.cliente.R;
import fiuba.cliente.Files.FileHandler;
import fiuba.cliente.Files.MetaFileHandler;
import fiuba.cliente.model.IconArrayAdapter;
import fiuba.cliente.model.Request;
import fiuba.cliente.model.RequestHandler;
import fiuba.cliente.model.Response;
import fiuba.cliente.model.SearchEngine;
import fiuba.cliente.model.Utils;
import fiuba.cliente.model.gpsLocation;


public class DocumentsActivity extends ListActivity {
    private String token;

    private FileHandler fileHandler;
    private MetaFileHandler metaFileHandler;
    private SearchEngine searchEngine;
    private String userName;
    private Menu optionsMenu;
    private boolean searchMode = false;
    private boolean sharedFilesMode = false;
    private boolean deletedFilesMode = false;
    private int fabId = 2;

    private static final int FILE_SELECT_CODE = 0;

    private static final int MESSAGE_REMOVE_USER = 1;
    private static final int PROMPT_FOLDER_NAME = 2;
    private static final int SHOW_OPTIONS_FILE = 3;
    private static final int SHOW_OPTIONS_UPLOAD_FILE = 4;
    private static final int SHOW_VERSIONS_FILE = 5;
    private static final int SHOW_DETAILS_FILE = 6;
    private static final int SHOW_TAGS_FILE = 7;
    private static final int SHOW_PROMPT_ADD_TAG = 8;
    private static final int SHOW_PROMPT_SHARED = 9;

    private static final int CHANGE_FILE_NAME = 10;
    private static final int DELETE_SHARED_WITH = 11;

    final CharSequence optionOwnFiles[] = { "Descargar", "Modificar Archivo", "Versiones", "Eliminar","Detalles", "Compartir", "Quitar compartido" };
    final CharSequence optionSharedFiles[] = { "Descargar", "Modificar Archivo", "Versiones", "Eliminar", "Detalles" };
    final CharSequence optionFolders[] = { "Compartir", "Eliminar","Detalles"};
    final CharSequence optionCreateElement[] = { "Subir archivo", "Crear carpeta"};


    private fiuba.cliente.model.gpsLocation gpsLocation;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        token = getIntent().getStringExtra("token");
        userName = getIntent().getStringExtra("username");

        fileHandler = new FileHandler(userName);
        metaFileHandler = new MetaFileHandler(token, userName);
        searchEngine = new SearchEngine(metaFileHandler);
        gpsLocation = new gpsLocation(this);

        if (savedInstanceState != null) {
            int actualidentifier = savedInstanceState.getInt("actualidentifier");
            metaFileHandler.setActualIdentifier(actualidentifier);
        }

        buildDirectory();
        createFab();
        sendLatitudeLongitude();

        this.getListView().setLongClickable(true);
        this.getListView().setOnItemLongClickListener(new AdapterView.OnItemLongClickListener() {
            public boolean onItemLongClick(AdapterView<?> parent, View v, int position, long id) {

                if (!metaFileHandler.isFolder(position) && !metaFileHandler.getMetafileIsDeleted(position))
                    showMessageOrPromptPosition(SHOW_OPTIONS_FILE, position);

                return true;
            }
        });

    }

    public void sendLatitudeLongitude()
    {
        double latitude = gpsLocation.getLatitude();
        double longitude = gpsLocation.getLongitude();

        metaFileHandler.setLongitudeLatitude(latitude, longitude);
    }

    private void createFab() {
        View fab = new ImageButton(this);
        fab.setId(fabId);
        fab.setClickable(true);
        fab.setBackgroundResource(R.drawable.addbutton);

        RelativeLayout mRel =  new RelativeLayout(this);
        RelativeLayout.LayoutParams mParams = new RelativeLayout.LayoutParams(100, 100);
        mParams.setMargins(0, 0, 100, 100);
        mParams.addRule(RelativeLayout.ALIGN_PARENT_RIGHT);
        mParams.addRule(RelativeLayout.ALIGN_PARENT_BOTTOM);

        fab.setLayoutParams(mParams);
        mRel.addView(fab);

        FrameLayout layout = (FrameLayout) getListView().getParent();
        layout.addView(mRel);

        fab.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                addFileOrFolder();
            }
        });
    }


    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle item selection
        switch (item.getItemId()) {
            case R.id.sharedFiles:
                showSharedFiles();
                return true;
            case R.id.searchTag:
                showPromptSearch(1, "Ingresar etiqueta");
                return true;
            case R.id.searchExtension:
                showPromptSearch(2, "Ingresar extensión");
                return true;
            case R.id.searchName:
                showPromptSearch(3, "Ingresar nombre");
                return true;
            case R.id.searchOwner:
                showPromptSearch(4, "Ingresar propietario");
                return true;
            case R.id.backToMyFiles:
                this.backToMyFiles();
                return true;
            case R.id.updateAll:
                this.updateAndBackToActualFolder();
                return true;
            case R.id.details:
                metaFileHandler.updateUserInfo();
                Intent userDetailsActivity = new Intent(this, UserDetails.class);
                userDetailsActivity.putExtra("details", metaFileHandler.getUserDetails());
                userDetailsActivity.putExtra("token", token);
                startActivity(userDetailsActivity);
                return true;
            case R.id.logOut:
                finish();
                return true;
            case R.id.removeUser:
                showMessageOrPrompt(MESSAGE_REMOVE_USER);
                return true;
            case R.id.recoverFile:
                showDeletedFiles();
                return true;
        }
        return true;
    }

    private void showDeletedFiles() {
        metaFileHandler.setDeletedFilesVisible();
        cargarListado();
        setSearchOptionMenu();
        sharedFilesMode = false;
        deletedFilesMode = true;
        findViewById(fabId).setVisibility(View.INVISIBLE);
        updateTitle();
    }

    public boolean isDeletedFilesMode(){
        return deletedFilesMode;
    }


    public void showPromptSearch(final int searchType, String searchMessage) {
        LayoutInflater li = LayoutInflater.from(this);
        View promptsView = li.inflate(R.layout.prompt_input, null);

        AlertDialog.Builder alertDialogBuilder = new AlertDialog.Builder(this);

        alertDialogBuilder.setView(promptsView);

        TextView searchTitle = (TextView) promptsView.findViewById(R.id.title);
        searchTitle.setText(searchMessage);
        final EditText userInput = (EditText) promptsView
                .findViewById(R.id.editTextDialogUserInput);

        // set dialog message
        alertDialogBuilder
                .setCancelable(false)
                .setPositiveButton("OK",
                        new DialogInterface.OnClickListener() {
                            public void onClick(DialogInterface dialog, int id) {
                                // get user input and set it to result
                                // edit text

                                boolean result = searchEngine.searchFiles(searchType, userInput.getText().toString());
                                cargarListado();
                                setSearchOptionMenu();
                                searchMode = true;
                                updateTitle();
                                String message;
                                if (!result)
                                    message = "No se han encontrado archivos que cumplan con el criterio de búsqueda ";
                                else
                                    message = "Listado de archivos encontrados";
                                Toast.makeText(getApplicationContext(), message, Toast.LENGTH_LONG).show();
                            }
                        })
                .setNegativeButton("Cancel",
                        new DialogInterface.OnClickListener() {
                            public void onClick(DialogInterface dialog, int id) {
                                dialog.cancel();
                            }
                        });

        AlertDialog alertDialog = alertDialogBuilder.create();
        alertDialog.show();
    }

    public void showMessageOrPrompt(int optionType) {
        AlertDialog.Builder alertDialogBuilder = new AlertDialog.Builder(this);
        alertDialogBuilder
                .setCancelable(false)
                .setNegativeButton("Cancel",
                        new DialogInterface.OnClickListener() {
                            public void onClick(DialogInterface dialog, int id) {
                                dialog.cancel();
                            }
                        });

        switch (optionType) {
            case MESSAGE_REMOVE_USER:
                showMessageRemoveUser(alertDialogBuilder);
                break;

            case PROMPT_FOLDER_NAME:
                showPromptFolderName(alertDialogBuilder);
                break;
        }

        AlertDialog alertDialog = alertDialogBuilder.create();
        alertDialog.show();
    }


    public void showMessageRemoveUser(AlertDialog.Builder alertDialogBuilder) {

        alertDialogBuilder
                .setTitle("¿Confirma eliminación de su cuenta?")
                .setPositiveButton("OK",
                        new DialogInterface.OnClickListener() {
                            public void onClick(DialogInterface dialog, int id) {
                                String result = removeUser();
                                Toast.makeText(getApplicationContext(), result, Toast.LENGTH_LONG).show();
                                finish();
                            }
                        });
    }

    private void showPromptFolderName(AlertDialog.Builder alertDialogBuilder) {
        LayoutInflater li = LayoutInflater.from(this);
        View promptsView = li.inflate(R.layout.prompt_input, null);
        ((TextView) promptsView.findViewById(R.id.title)).setText("Nueva carpeta");

        alertDialogBuilder.setView(promptsView);

        final EditText userInput = (EditText) promptsView
                .findViewById(R.id.editTextDialogUserInput);

        // set dialog message
        alertDialogBuilder
                .setPositiveButton("OK",
                        new DialogInterface.OnClickListener() {
                            public void onClick(DialogInterface dialog, int id) {
                                // get user input and set it to result
                                // edit text
                                String result = crearCarpeta(userInput.getText().toString());
                                Toast.makeText(getApplicationContext(), result, Toast.LENGTH_LONG).show();
                                updateAndBackToActualFolder();

                            }
                        });
    }

    @Override
    public void onBackPressed(){

        if (!searchMode && !sharedFilesMode && !deletedFilesMode) {
            if (metaFileHandler.isMainRoot()) {
                finish();
                return;
            } else {
                metaFileHandler.updateIdentifier(0);
                setListAdapter(new IconArrayAdapter(this, metaFileHandler.getEmpytArrayString()));
                ((IconArrayAdapter) getListAdapter()).setMetaFiles(metaFileHandler);
            }
        } else {
            backToMyFiles();
        }
        updateTitle();
    }

    @Override
    public void onSaveInstanceState(Bundle savedInstanceState) {

        int actualidentifier =metaFileHandler.getActualIdentifier();
        savedInstanceState.putInt("actualidentifier", actualidentifier);
        // Always call the superclass so it can save the view hierarchy state
        super.onSaveInstanceState(savedInstanceState);
    }

    public void buildDirectory()
    {
        String respuesta=metaFileHandler.requestMetaFiles();
        this.cargarListado();
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {

        getMenuInflater().inflate(R.menu.menu_documents, menu);
        optionsMenu = menu;

        return true;
    }


    public void updateTitle() {
        String title = "TP Taller 2 - ";
        if (searchMode) {
            title += "Búsqueda";
        } else {
            if (sharedFilesMode)
                title += "Archivos compartidos";
            else if (deletedFilesMode)
                title += "Papelera";
            else {
                String path = metaFileHandler.getPathActualFolder();
                if (path.length() > 10)
                    path = ".." + path.substring(path.length() - 11);

                title += path;
            }
        }
        setTitle(title);
    }

    public String removeUser() {
        Request request = new Request("DELETE","users/"+userName+"?token="+token);
        Response resp = new RequestHandler().sendRequest(request);
        if (resp.getStatus())
            return "Usuario " + userName +  " eliminado";
        else
            return "Error al intentar eliminar el usuario " + userName;
    }

    public void showSharedFiles() {
        metaFileHandler.setSharedFilesVisible();
        cargarListado();
        setSearchOptionMenu();
        deletedFilesMode = false;
        sharedFilesMode = true;
        findViewById(fabId).setVisibility(View.INVISIBLE);
        updateTitle();
    }


    void setSearchOptionMenu() {
        MenuItem itemBackToMyFiles = optionsMenu.findItem((R.id.backToMyFiles));
        itemBackToMyFiles.setVisible(true);
        findViewById(fabId).setVisibility(View.INVISIBLE);

    }

    private void backToMyFiles() {
        searchMode = false;
        sharedFilesMode = false;
        deletedFilesMode = false;
        MenuItem itemBackToMyFiles = optionsMenu.findItem((R.id.backToMyFiles));
        itemBackToMyFiles.setVisible(false);
        buildDirectory();
        findViewById(fabId).setVisibility(View.VISIBLE);
    }

    private void showFileChooser() {
        Intent intent = new Intent(Intent.ACTION_GET_CONTENT);
        intent.setType("*/*");
        intent.addCategory(Intent.CATEGORY_OPENABLE);

        try {
            startActivityForResult(
                    Intent.createChooser(intent, "Select a File to Upload"),
                    FILE_SELECT_CODE);
        } catch (android.content.ActivityNotFoundException ex) {
            // Potentially direct the user to the Market with a Dialog
            Toast.makeText(this, "Please install a File Manager.",
                    Toast.LENGTH_SHORT).show();
        }
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        switch (requestCode) {
            case FILE_SELECT_CODE:
                if (resultCode == RESULT_OK) {

                    Uri uri = data.getData();
                    String path;

                    try {
                        path = Utils.getPath(this, uri);
                        Toast.makeText(getApplicationContext(), this.subirArchivo(path), Toast.LENGTH_LONG).show();
                        updateAndBackToActualFolder();
                    }
                    catch(Exception e){
                        Toast.makeText(getApplicationContext(), "No se pudo seleccionar el directorio", Toast.LENGTH_LONG).show();
                    }
                }
                break;
        }
        super.onActivityResult(requestCode, resultCode, data);
    }

    public void updateAndBackToActualFolder() {
        ArrayDeque<Integer> pathToActualFolder = metaFileHandler.getIdsPathToActualFolder();
        metaFileHandler.requestMetaFiles();
        metaFileHandler.moveThroughPath(pathToActualFolder);
        cargarListado();
    }

    public String subirArchivo(String path)
    {
        return fileHandler.uploadFile(path, metaFileHandler.getActualIdentifier());
    }

    public String crearCarpeta(String name)
    {
        return fileHandler.createFolder(name, metaFileHandler.getActualIdentifier());
    }

    public void descargarArchivo(String fileName,int fileIdentifier)
    {
        String respuesta = fileHandler.downloadFile("/mnt/sdcard/Download/"+fileName, fileIdentifier, token);
        Toast.makeText(getApplicationContext(), respuesta, Toast.LENGTH_LONG).show();
    }


    public void cargarListado()
    {
        IconArrayAdapter asd = new IconArrayAdapter(this, metaFileHandler.getEmpytArrayString());

        setListAdapter(asd);
        ((IconArrayAdapter) getListAdapter()).setMetaFiles(metaFileHandler);
    }

    @Override
    protected void onListItemClick(ListView l, View v, int position, long id) {

        if (metaFileHandler.isFolder(position)) {
            // Carga los archivos de adentro de la carpeta donde se entra
            // en ownFilesShowing de metaFileHandler
            metaFileHandler.updateIdentifier(position);
            setListAdapter(new IconArrayAdapter(this, metaFileHandler.getEmpytArrayString()));
            ((IconArrayAdapter) getListAdapter()).setMetaFiles(metaFileHandler);
            updateTitle();
        } else if(metaFileHandler.getMetafileIsDeleted(position)) {
            messageRecoverFile(position);

        } else {
            openFile(position);
        }
    }

    public void openFile(int position) {
        Uri uri = Uri.parse("/mnt/sdcard/Download/" + metaFileHandler.getMetaFileName(position));
        File file = new File(uri.getPath());
        if (!file.canRead()) {
            descargarArchivo(metaFileHandler.getMetaFileName(position), metaFileHandler.getMetaFileIdentifier(position));
        }

        if (metaFileHandler.getMetaFileExtension(position).toUpperCase().equals("JPG")) {
            Intent intent = new Intent(Intent.ACTION_VIEW);
            intent.setDataAndType(uri, "image/*");
            startActivity(intent);
        } else if (metaFileHandler.getMetaFileExtension(position).toUpperCase().equals("PDF")) {
            Intent intent = new Intent(Intent.ACTION_VIEW);
            intent.setDataAndType(uri, "application/pdf");
            startActivity(intent);
        } else if (metaFileHandler.getMetaFileExtension(position).toUpperCase().equals("TXT")) {
            Intent intent = new Intent(Intent.ACTION_VIEW);
            intent.setDataAndType(uri, "text/plain");
            startActivity(intent);
        }

    }

    private void deleteFile(int position) {
        boolean successDelete = metaFileHandler.setMetafileDeleted(position);
        String result;
        if (successDelete) {
            result = "Archivo eliminado correctamente";
            updateAndBackToActualFolder();
        }
        else
            result = "Error al intentar eliminar el archivo";
        Toast.makeText(getApplicationContext(), result, Toast.LENGTH_LONG).show();
    }


    private String addNewTag(String tag, int position) {
        int id = metaFileHandler.getMetaFileIdentifier(position);
        return metaFileHandler.addNewTag(tag, id);
    }

    private String recoverFile(int position) {
        int id = metaFileHandler.getMetaFileIdentifier(position);
        return metaFileHandler.recoverFile(id);
    }


    private void showMessageOrPromptPosition(int optionType, final int position) {
        AlertDialog.Builder alertDialogBuilder = new AlertDialog.Builder(this);
        alertDialogBuilder
                .setCancelable(false)
                .setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int id) {
                        dialog.cancel();
                    }
                });

        switch (optionType) {
            case SHOW_OPTIONS_FILE:
                showOptionFiles(alertDialogBuilder, position);
                break;
            case SHOW_OPTIONS_UPLOAD_FILE:
                selectOptionsUpload(alertDialogBuilder, position);
                break;
            case SHOW_VERSIONS_FILE:
                showVersions(alertDialogBuilder, position);
                break;
            case SHOW_DETAILS_FILE:
                showDetails(alertDialogBuilder, position);
                break;
            case SHOW_TAGS_FILE:
                showTags(alertDialogBuilder, position);
                break;
            case SHOW_PROMPT_ADD_TAG:
                showPromptAddTag(alertDialogBuilder, position);
                break;
            case SHOW_PROMPT_SHARED:
                shareFile(alertDialogBuilder, position, true);
                break;
            case CHANGE_FILE_NAME:
                showPromptChangeName(alertDialogBuilder,position);
                break;
            case DELETE_SHARED_WITH:
                shareFile(alertDialogBuilder, position, false);
                break;

        }

        AlertDialog alertDialog = alertDialogBuilder.create();

        // show it
        alertDialog.show();
    }


    private void showOptionFiles(AlertDialog.Builder alertDialogBuilder, final int position)
    {

        // set dialog message
        alertDialogBuilder.setTitle("Opciones");

        String fileOwner = metaFileHandler.getMetafileOwner(position);

        if (fileOwner.equals(userName))
            showOptionsOwnFile(alertDialogBuilder, position);
        else
            showOptionsSharedFile(alertDialogBuilder, position);

    }


    private void showOptionsOwnFile(AlertDialog.Builder alertDialogBuilder, final int position) {
        alertDialogBuilder.setItems(optionOwnFiles, new DialogInterface.OnClickListener() {
            public void onClick(DialogInterface dialog, int which) {

                switch (which) {
                    case 0:
                        //Descargar
                        descargarArchivo(metaFileHandler.getMetaFileName(position), metaFileHandler.getMetaFileIdentifier(position));
                        break;
                    case 1:
                        //Modificar
                        modifyFile(position);
                        break;
                    case 2:
                        //Versiones
                        showMessageOrPromptPosition(SHOW_VERSIONS_FILE, position);
                        break;
                    case 3:
                        //Eliminar
                        deleteFile(position);
                        break;

                    case 4:
                        //Detalles
                        showMessageOrPromptPosition(SHOW_DETAILS_FILE, position);
                        break;

                    case 5:
                        //Compartir
                        showMessageOrPromptPosition(SHOW_PROMPT_SHARED, position);
                        break;
                    case 6:
                        //Sacar compartido
                        showMessageOrPromptPosition(DELETE_SHARED_WITH, position);
                        break;

                }

            }
        });
    }

    private void showOptionsSharedFile(AlertDialog.Builder alertDialogBuilder, final int position) {
        alertDialogBuilder.setItems(optionSharedFiles, new DialogInterface.OnClickListener() {
            public void onClick(DialogInterface dialog, int which) {

                switch (which) {
                    case 0:
                        //Descargar
                        descargarArchivo(metaFileHandler.getMetaFileName(position), metaFileHandler.getMetaFileIdentifier(position));
                        break;
                    case 1:
                        //Modificar archivo
                        modifyFile(position);
                        break;
                    case 2:
                        //Versiones
                        showMessageOrPromptPosition(SHOW_VERSIONS_FILE, position);
                        break;
                    case 3:
                        //Eliminar
                        deleteFile(position);
                        break;
                    case 4:
                        //Detalles
                        showMessageOrPromptPosition(SHOW_DETAILS_FILE, position);
                        break;


                }

            }
        });
    }

    private void selectOptionsUpload(AlertDialog.Builder alertDialogBuilder, final int position) {

        // set dialog message
        alertDialogBuilder
                .setTitle("Conflicto de versiones. Desea sobreescribir?")
                .setPositiveButton("OK",
                        new DialogInterface.OnClickListener() {
                            public void onClick(DialogInterface dialog, int id) {
                                Toast.makeText(getApplicationContext(), "Subido correctamente", Toast.LENGTH_LONG).show();

                                final CharSequence versions[] = metaFileHandler.getVersions(position);
                                int lastVersion = Integer.parseInt(versions[versions.length - 1].toString());
                                int fileId = metaFileHandler.getMetaFileIdentifier(position);
                                int actualId = metaFileHandler.getMetafileFolder(fileId);
                                String path = "/mnt/sdcard/Download/" + metaFileHandler.getMetaFileName(position);
                                boolean result = fileHandler.modifyFile(path, fileId, actualId, lastVersion, true);
                            }
                        });
    }


    private void showVersions(AlertDialog.Builder alertDialogBuilder, final int position) {
        final CharSequence versions[] = metaFileHandler.getVersions(position);
        final CharSequence versionsDate[] = metaFileHandler.getVersionsDate(position);

        // set dialog message
        alertDialogBuilder
                .setTitle("Versiones")

                .setItems(versionsDate, new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int which) {
                        int version = Integer.parseInt(versions[which].toString());
                        String respuesta = fileHandler.downloadFile("/mnt/sdcard/Download/" + metaFileHandler.getMetaFileName(position), metaFileHandler.getMetaFileIdentifier(position), token, version);
                        Toast.makeText(getApplicationContext(), respuesta, Toast.LENGTH_LONG).show();
                    }
                });
    }


    private void showDetails(AlertDialog.Builder alertDialogBuilder, final int position) {

        CharSequence[] ownFileDetails = metaFileHandler.fileDetails(position);

        alertDialogBuilder
                .setTitle("Detalles")
                .setItems(ownFileDetails, new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int which) {

                        switch (which) {

                            //Modificar nombre
                            case 6:
                                showMessageOrPromptPosition(CHANGE_FILE_NAME, position);
                                break;
                            //Agregar tag
                            case 7:
                                showMessageOrPromptPosition(SHOW_PROMPT_ADD_TAG, position);
                                break;
                            //Ver tags
                            case 8:
                                showMessageOrPromptPosition(SHOW_TAGS_FILE, position);
                                break;
                        }

                    }
                });
    }


    private void showTags(final AlertDialog.Builder alertDialogBuilder, final int position)
    {
        final CharSequence tags[]=metaFileHandler.getTags(position);

        // set dialog message
        alertDialogBuilder

                .setTitle("Tags")

                .setItems(tags, new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int which) {
                        deleteTag(position, which);
                    }
                });
    }


    private void deleteTag(final int position, final int indiceTag) {
        AlertDialog.Builder alertDialogBuilder = new AlertDialog.Builder(this);

        // set dialog message
        alertDialogBuilder
                .setTitle("Desea eliminar el Tag?")
                .setCancelable(false)
                .setPositiveButton("OK",
                        new DialogInterface.OnClickListener() {
                            public void onClick(DialogInterface dialog, int id) {

                                int idFile = metaFileHandler.getMetaFileIdentifier(position);
                                String result = metaFileHandler.deleteTag(indiceTag, idFile);
                                Toast.makeText(getApplicationContext(), result, Toast.LENGTH_LONG).show();
                                updateAndBackToActualFolder();
                            }
                        })
                .setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int id) {
                        dialog.cancel();
                    }
                });

        AlertDialog alertDialog = alertDialogBuilder.create();
        alertDialog.show();

    }

    private void messageRecoverFile(final int position) {
        AlertDialog.Builder alertDialogBuilder = new AlertDialog.Builder(this);

        // set dialog message
        alertDialogBuilder
                .setTitle("Desea recuperar el archivo?")
                .setCancelable(false)
                .setPositiveButton("OK",
                        new DialogInterface.OnClickListener() {
                            public void onClick(DialogInterface dialog, int id) {
                                String result = recoverFile(position);
                                Toast.makeText(getApplicationContext(), result, Toast.LENGTH_LONG).show();
                                backToMyFiles();
                                updateTitle();
                            }
                        })
                .setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int id) {
                        dialog.cancel();
                    }
                });

        AlertDialog alertDialog = alertDialogBuilder.create();
        alertDialog.show();

    }


    private void showPromptAddTag(AlertDialog.Builder alertDialogBuilder, final int position) {
        LayoutInflater li = LayoutInflater.from(this);
        View promptsView = li.inflate(R.layout.prompt_input, null);
        ((TextView) promptsView.findViewById(R.id.title)).setText("Nuevo tag");

        alertDialogBuilder.setView(promptsView);

        final EditText userInput = (EditText) promptsView
                .findViewById(R.id.editTextDialogUserInput);

        // set dialog message
        alertDialogBuilder
                .setPositiveButton("OK",
                        new DialogInterface.OnClickListener() {
                            public void onClick(DialogInterface dialog, int id) {
                                // get user input and set it to result
                                // edit text
                                String result = addNewTag(userInput.getText().toString(), position);
                                Toast.makeText(getApplicationContext(), result, Toast.LENGTH_LONG).show();
                                updateAndBackToActualFolder();

                            }
                        });
    }


    private void showPromptChangeName(AlertDialog.Builder alertDialogBuilder, final int position) {
        LayoutInflater li = LayoutInflater.from(this);
        View promptsView = li.inflate(R.layout.prompt_input, null);
        ((TextView) promptsView.findViewById(R.id.title)).setText("Cambio de nombre");

        alertDialogBuilder.setView(promptsView);

        final EditText userInput = (EditText) promptsView
                .findViewById(R.id.editTextDialogUserInput);

        // set dialog message
        alertDialogBuilder
                .setPositiveButton("OK",
                        new DialogInterface.OnClickListener() {
                            public void onClick(DialogInterface dialog, int id) {
                                // get user input and set it to result
                                // edit text
                                int fileId = metaFileHandler.getMetaFileIdentifier(position);
                                String result = metaFileHandler.changeFileName(userInput.getText().toString(), fileId);
                                Toast.makeText(getApplicationContext(), result, Toast.LENGTH_LONG).show();
                                updateAndBackToActualFolder();
                            }
                        });
    }



    private void shareFile(AlertDialog.Builder alertDialogBuilder, final int position, final boolean addUser) {
        LayoutInflater li = LayoutInflater.from(this);
        View promptsView = li.inflate(R.layout.prompt_input, null);
        if(addUser) {
            ((TextView) promptsView.findViewById(R.id.title)).setText("Compartir con");
            ((TextView) promptsView.findViewById(R.id.subtitle)).setText("Ingresar usuarios separados por comas");
        }
        else {
            ((TextView) promptsView.findViewById(R.id.title)).setText("No compartir con");
        }

        alertDialogBuilder.setView(promptsView);

        final EditText userInput = (EditText) promptsView
                .findViewById(R.id.editTextDialogUserInput);

        // set dialog message
        alertDialogBuilder
                .setPositiveButton("OK",
                        new DialogInterface.OnClickListener() {
                            public void onClick(DialogInterface dialog, int id) {
                                if(addUser) {
                                    boolean status = metaFileHandler.shareFile(position, userInput.getText().toString());
                                    Toast.makeText(getApplicationContext(), "Compartido exitosamente", Toast.LENGTH_LONG).show();
                                } else {
                                    boolean status = metaFileHandler.unshareFile(position, userInput.getText().toString());
                                    Toast.makeText(getApplicationContext(), "Compartido borrado exitosamente", Toast.LENGTH_LONG).show();

                                }
                                updateAndBackToActualFolder();

                            }
                        });
    }


    private void modifyFile(final int position)
    {
        final CharSequence versions[] = metaFileHandler.getVersions(position);

        int lastVersion = Integer.parseInt(versions[versions.length - 1].toString());
        int id = metaFileHandler.getMetaFileIdentifier(position);
        int actualId = -1;
        if (!sharedFilesMode) {
            actualId = metaFileHandler.getMetafileFolder(id);
        }
        String path = "/mnt/sdcard/Download/" + metaFileHandler.getMetaFileName(position);

        boolean result = fileHandler.modifyFile(path, id, actualId, lastVersion, false);
        if (result){
            this.updateAndBackToActualFolder();
            Toast.makeText(getApplicationContext(), metaFileHandler.getMetaFileName(position) + " subido correctamente.", Toast.LENGTH_LONG).show();
        } else {
            showMessageOrPromptPosition(SHOW_OPTIONS_UPLOAD_FILE, position);
        }
    }

    public void addFileOrFolder() {

        AlertDialog.Builder alertDialogBuilder = new AlertDialog.Builder(this);
        alertDialogBuilder
                .setCancelable(false)
                .setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int id) {
                        dialog.cancel();
                    }
                })
                .setItems(optionCreateElement, new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int which) {

                        switch (which) {

                            //Subir archivo
                            case 0:
                                showFileChooser();
                                break;
                            //Crear carpeta
                            case 1:
                                showMessageOrPrompt(PROMPT_FOLDER_NAME);
                                break;

                        }

                    }
                });
        AlertDialog alertDialog = alertDialogBuilder.create();
        alertDialog.show();
    }

    private void update() {
        if (sharedFilesMode) {
            metaFileHandler.requestMetaFiles();
            showSharedFiles();
        }
        else if (!searchMode && !deletedFilesMode) {
            updateAndBackToActualFolder();
        }
    }

}
