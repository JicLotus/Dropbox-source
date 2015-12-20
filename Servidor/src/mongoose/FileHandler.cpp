/*
 * FileHandler.cpp
 *
 *  Created on: 9 de set. de 2015
 *      Author: kevin
 */

#include "FileHandler.h"

FileHandler::FileHandler() {
	// TODO Auto-generated constructor stub

}

string FileHandler::GetMetadata(string identifier, string guid) {
	File file;
	if(file.Load(std::stoi(identifier))) {
		Log::insert(Log::TYPE_INFORMATION, "Obtenida exitosamente la información del archivo " + identifier);
		return file.GetJson();
	} else {
		Log::insert(Log::TYPE_ERROR, "Error al obtener la información del archivo " + identifier + ". Archivo inválido.");
		return "{ \"result\" : \"ERROR\" ,  \"message\" : \"Invalid identifier\" }";
	}
}

string FileHandler::GetDocument(string identifier, string guid) {
	File file;
	FileContent fileContent;
	if(file.Load(std::stoi(identifier))) {
		if(!file.GetType()==2)
		{
			Log::insert(Log::TYPE_ERROR, "No se puede descargar contenido de una carpeta " + identifier + "." + fileContent.GetJson());
			return "{ \"result\" : \"ERROR\" ,  \"message\" : \"Invalid file requested\" }";
		}
		FileVersion* fileVersion = file.GetVersion(file.GetLastVersion());
		if(!fileVersion)
		{
			Log::insert(Log::TYPE_ERROR, "Version de archivo invalida " + identifier + ". Archivo inválido." + fileContent.GetJson());
			return "{ \"result\" : \"ERROR\" ,  \"message\" : \"Invalid version\" }";
		}
		if(!fileContent.Load(fileVersion->fileContentIdentifier))
		{
			Log::insert(Log::TYPE_ERROR, "Error al obtener la información del archivo " + identifier + ". Archivo inválido." + fileContent.GetJson());
			return "{ \"result\" : \"ERROR\" ,  \"message\" : \"File corrupted\" }";
		}
		Log::insert(Log::TYPE_INFORMATION, "Obtenida exitosamente la información del archivo " + identifier);
		return "{ \"result\" : \"OK\" ,  \"content\" : \"" + fileContent.GetContent() + "\" }";
	} else {
		Log::insert(Log::TYPE_ERROR, "Error al obtener la información del archivo " + identifier + ". Archivo inválido.");
		return "{ \"result\" : \"ERROR\" ,  \"message\" : \"Invalid identifier\" }";
	}
}

string FileHandler::GetDocument(string identifier, string guid, int version) {
	File file;
	FileContent fileContent;
	if(file.Load(std::stoi(identifier))) {
		FileVersion* fileVersion = file.GetVersion(version);
		if(!fileVersion)
		{
			Log::insert(Log::TYPE_ERROR, "Version de archivo invalida " + identifier + ". Archivo inválido." + fileContent.GetJson());
			return "{ \"result\" : \"ERROR\" ,  \"message\" : \"Invalid version\" }";
		}
		if(!fileContent.Load(fileVersion->fileContentIdentifier))
		{
			Log::insert(Log::TYPE_ERROR, "Error al obtener la información del archivo " + identifier + ". Archivo inválido." + fileContent.GetJson());
			return "{ \"result\" : \"ERROR\" ,  \"message\" : \"File corrupted\" }";
		}
		Log::insert(Log::TYPE_INFORMATION, "Obtenida exitosamente la información del archivo " + identifier);
		return "{ \"result\" : \"OK\" ,  \"content\" : \"" + fileContent.GetContent() + "\" }";
	} else {
		Log::insert(Log::TYPE_ERROR, "Error al obtener la información del archivo " + identifier + ". Archivo inválido.");
		return "{ \"result\" : \"ERROR\" ,  \"message\" : \"Invalid identifier\" }";
	}
}

string FileHandler::Delete(string identifier, string guid) {
	File file;
	if(file.Load(std::stoi(identifier))) {
		file.SetDeleted(true);
		file.Save();

		User user;
		user.Load(file.GetOwner());
		user.SetQuotaMB(user.GetQuotaMB() - file.GetSize());
		user.Save();

		Log::insert(Log::TYPE_INFORMATION, "Borrado de archivo exitoso: " + identifier);
		return "{ \"result\" : \"OK\" }";
	}
	else {
			Log::insert(Log::TYPE_ERROR, "Error al obtener la información del archivo " + identifier + ". Archivo inválido.");
			return "{ \"result\" : \"ERROR\" ,  \"message\" : \"Invalid identifier\" }";
	}
}

string FileHandler::Insert(string json) {

	Json::Value root;
	Json::Reader reader;
	bool parsedSuccess = reader.parse(json, root, false);
	if(parsedSuccess) {
		File file;
		User user;

		if(root["id"].isNull()) {
			file.CreateId();
			file.SetLastVersion(0);
			file.SetLastModifiedOn(time(0));
			file.Generate(root);
		} else {
			if(!file.Load(root["id"].asInt())) {
				Log::insert(Log::TYPE_ERROR, "Identificador invalido");
				return "{ \"result\" : \"ERROR\" ,  \"message\" : \"Invalid identifier\" }";
			}

			if(root["lastVersion"].isNull()) {
				Log::insert(Log::TYPE_ERROR, "Debe especificar version de archivo");
				return "{ \"result\" : \"ERROR\" ,  \"message\" : \"LastVersion not included\" }";
			} else if(file.GetLastVersion() != root["lastVersion"].asInt()
					&& (root["forceCreation"].isNull()
						|| not root["forceCreation"].asBool()))	{
				Log::insert(Log::TYPE_ERROR, "Archivo desactualizado");
				Log::insert(Log::TYPE_ERROR, to_string(root["lastVersion"].asInt()) + " " + to_string(file.GetLastVersion()));
				return "{ \"result\" : \"ERROR\" ,  \"message\" : \"Lastversion Outofdate\" }";
			}
			if(!root["lastModifiedBy"].isNull()) {
				file.SetLastModifiedBy(root["lastModifiedBy"].asString());
			}
		}

		if(file.GetType() == 1) {
			FileContent fileContent;
			fileContent.SetIdentifier(to_string(file.GetIdentifier()) + "_" + to_string(file.GetLastVersion() + 1));
			if(not root["content"].isNull()) {
				fileContent.SetContent(root["content"].asString());
			} else {
				Log::insert(Log::TYPE_ERROR, "No se especifico contenido del archivo");
				return "{ \"result\" : \"ERROR\" ,  \"message\" : \"File must be a content\" }";
			}
			fileContent.Save();
			file.SetSize(root["content"].asString().length() / 1024);

			FileVersion* fileVersion = new FileVersion();
			fileVersion->fileContentIdentifier = fileContent.GetIdentifier();
			fileVersion->version = file.GetLastVersion() + 1;
			fileVersion->date = time(0);
			file.AddVersion(fileVersion);

		}

		if(user.Load(file.GetOwner())) {
			if(file.GetType() == 1) {
				int size = root["content"].asString().length() / 1024;
				if(user.GetQuotaLimitMB() < user.GetQuotaMB() + size){
					Log::insert(Log::TYPE_ERROR, "Limite Quota usuario excedido");
					return "{ \"result\" : \"ERROR\" ,  \"message\" : \"Quota exceeded\" }";
				}
				user.SetQuotaMB(user.GetQuotaMB() + size);
			}
			if(root["lastModifiedBy"].isNull()) {
				file.SetLastModifiedBy(user.GetIdentifier());
			}
		} else {
			Log::insert(Log::TYPE_ERROR, "Error en la creación del archivo. Usuario inválido.");
			return "{ \"result\" : \"ERROR\" ,  \"message\" : \"Invalid User\" }";
		}

		if(not root["father"].isNull())	{
			File fileFather;
			if(!fileFather.Load(root["father"].asInt())) {
				Log::insert(Log::TYPE_ERROR, "Error en la creación del archivo. Father inválido.");
				return "{ \"result\" : \"ERROR\" ,  \"message\" : \"Father JSON\" }";
			}
			if(fileFather.GetType() != 2) {
				Log::insert(Log::TYPE_ERROR, "Error en la creación del archivo. Father must be a folder.");
				return "{ \"result\" : \"ERROR\" ,  \"message\" : \"Father must be a folder\" }";
			}
			if(file.GetLastVersion() == 1 || file.GetType() == 2)
				user.AddOwnFile(&fileFather, &file);
		} else {
			if(file.GetLastVersion() == 1 || file.GetType() == 2)
				user.AddOwnFile(NULL, &file);
		}

		file.Save();
		user.Save();

		Log::insert(Log::TYPE_INFORMATION, file.GetJson());

		return "{ \"result\" : \"OK\" ,  \"message\" : \"\", \"metafile\": " + to_string(file.GetIdentifier()) + "}";
	} else {
		Log::insert(Log::TYPE_ERROR, "Error en la creación del archivo. JSON inválido.");
		return "{ \"result\" : \"ERROR\" ,  \"message\" : \"Invalid JSON\" }";
	}
}

string FileHandler::Update(string identifier, string json) {

	Json::Value root;
	Json::Reader reader;
	bool parsedSuccess = reader.parse(json, root, false);
	if(parsedSuccess) {
		File file;
		file.Load(std::stoi(identifier));
		file.Generate(root);
		file.Save();
		return "{ \"result\" : \"OK\" ,  \"message\" : \"\" }";
	} else {
		Log::insert(Log::TYPE_ERROR, "Error en la actualización del metafile " + identifier + ". JSON inválido.");
		return "{ \"result\" : \"ERROR\" ,  \"message\" : \"Invalid JSON\" }";
	}
}

string FileHandler::AddSharedFiles(string identifier, string json) {

	Json::Value root;
	Json::Reader reader;
	bool parsedSuccess = reader.parse(json, root, false);
	if(parsedSuccess) {
		File file;
		if(!file.Load(std::stoi(identifier)))
		{
			Log::insert(Log::TYPE_ERROR, "Error en compartir archivos. File inválido.");
			return "{ \"result\" : \"ERROR\" ,  \"message\" : \"Invalid father\" }";
		}

		if(not root["users"].isNull()) {
			const Json::Value jsonUsers = root["users"];
			for (unsigned int index = 0; index < jsonUsers.size(); ++index ) {
				User user;
				if(user.Load(jsonUsers[index].asString()))
				{
					user.AddSharedFile(&file);
					user.Save();
					Log::insert(Log::TYPE_INFORMATION, "Archivo compartido exitosamente.");
				}
			}
		}

		return "{ \"result\" : \"OK\" ,  \"message\" : \"\" }";
	} else {
		Log::insert(Log::TYPE_ERROR, "Error en la actualización del usuario " + identifier + ". JSON inválido.");
		return "{ \"result\" : \"ERROR\" ,  \"message\" : \"Invalid JSON\" }";
	}
}

string FileHandler::DeleteSharedFiles(string fileIdentifier, string userIdentifier) {
	User user;
	if(user.Load(userIdentifier))
	{
		File file;
		if(file.Load(std::stoi(fileIdentifier)))
		{
			if(user.DeleteSharedFile(&file)) {
				user.Save();
				return "{ \"result\" : \"OK\" ,  \"message\" : \"\" }";
			}
			else
				return "{ \"result\" : \"ERROR\" ,  \"message\" : \"Invalid File\" }";
		}
		else
		{
			Log::insert(Log::TYPE_ERROR, "Error en eliminar compartir archivo del usuario " + userIdentifier + ". Archivo inválido.");
			return "{ \"result\" : \"ERROR\" ,  \"message\" : \"Invalid File\" }";
		}
	}
	else
	{
		Log::insert(Log::TYPE_ERROR, "Error en eliminar compartir archivo del usuario " + userIdentifier + ". Usuario inválido.");
		return "{ \"result\" : \"ERROR\" ,  \"message\" : \"Invalid User\" }";
	}

}

FileHandler::~FileHandler() {
	// TODO Auto-generated destructor stub
}
