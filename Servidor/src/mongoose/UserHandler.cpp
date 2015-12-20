/*
 * UserHandler.cpp
 *
 *  Created on: Aug 30, 2015
 *      Author: pablo
 */

#include "UserHandler.h"

UserHandler::UserHandler() {
	// TODO Auto-generated constructor stub

}

UserHandler::~UserHandler() {
	// TODO Auto-generated destructor stub
}

string UserHandler::Get(string identifier, string guid) {
	User user;
	if(user.Load(identifier) && user.GetIsActive()){
		return Get_ValidUserChecked(user, guid);
	}

	else{
		Log::insert(Log::TYPE_ERROR, "Error al obtener la información del usuario " + identifier + ". Usuario inválido.");
		return "{ \"result\" : \"ERROR\" ,  \"message\" : \"Invalid identifier\" }";
	}
}

string UserHandler::Get_ValidUserChecked(User user, string guid){
	if(Token::IsGuidValid(user.GetIdentifier(), guid)){
		Log::insert(Log::TYPE_INFORMATION, "Obtenida exitosamente la información del usuario " + user.GetIdentifier());
		return user.GetJson(true);
	}
	else{
		Log::insert(Log::TYPE_ERROR, "Error al obtener la información del usuario " + user.GetIdentifier() + ". Token inválido.");
		return "{ \"result\" : \"ERROR\" ,  \"message\" : \"Invalid token\" }";
	}
}

string UserHandler::Auth(string identifier, string md5) {

	User user;
	user.Load(identifier);

	if(user.GetMD5() == md5 && user.GetIsActive())
	{
		Token token;
		string guid = token.GenerateGuid();
		token.Save(identifier, guid);
		Log::insert(Log::TYPE_INFORMATION, "Autenticado exitosamente el usuario " + identifier);
		return "{ \"result\" : \"OK\" ,  \"message\" : \"\", \"token\" : \"" + guid + "\" }";
	}
	else{
		Log::insert(Log::TYPE_ERROR, "Error en la autenticación del usuario " + identifier + ". Usuario o password inválido.");
		return "{ \"result\" : \"ERROR\" ,  \"message\" : \"Invalid user or password\", \"token\" : \"\" }";
	}
}

string UserHandler::Insert(string json) {

	User user;
	std::set<string> ignoredAttributes;
	bool parsedSuccess = user.SetAllAttributes(json, ignoredAttributes);
	if(parsedSuccess)
	{
		return Insert_ValidJsonChecked(user);
	}
	else{
		Log::insert(Log::TYPE_ERROR, "Error en la creación del usuario. JSON inválido.");
		return "{ \"result\" : \"ERROR\" ,  \"message\" : \"Invalid JSON\" }";
	}
}

string UserHandler::Insert_ValidJsonChecked(User user){
	if(!user.GetIdentifier().empty()){
		return Insert_UsernameInJsonChecked(user);
	}
	else{
		Log::insert(Log::TYPE_ERROR, "Error en la creación del usuario. No se encuentra el nombre de usuario en el JSON.");
		return "{ \"result\" : \"ERROR\" ,  \"message\" : \"Null identifier in JSON\" }";
	}
}

string UserHandler::Insert_UsernameInJsonChecked(User user){
	User verifyExistence;
	if(!verifyExistence.Load(user.GetIdentifier())){
		user.SetQuotaMB(0);
		user.SetQuotaLimitMB(GetConfigQuotaLimitMB());
		user.SetIsActive(true);
		user.Save();

		Log::insert(Log::TYPE_INFORMATION, "Creado exitosamente el usuario.");
		return "{ \"result\" : \"OK\" ,  \"message\" : \"\" }";
	}
	else{
		Log::insert(Log::TYPE_ERROR, "Error al crear el usuario. El usuario ya existe.");
		return "{ \"result\" : \"ERROR\" ,  \"message\" : \"User already exists\" }";
	}
}

string UserHandler::Update(string identifier, string json) {

	Json::Value root;
	Json::Reader reader;
	bool parsedSuccess = reader.parse(json, root, false);
	if(parsedSuccess)
	{
		return Update_ValidJsonChecked(identifier, root["token"], json);
	}
	else{
		Log::insert(Log::TYPE_ERROR, "Error en la actualización del usuario " + identifier + ". JSON inválido.");
		return "{ \"result\" : \"ERROR\" ,  \"message\" : \"Invalid JSON\" }";
	}
}

string UserHandler::Update_ValidJsonChecked(string identifier, Json::Value rootToken, string json){
	if(not rootToken.isNull()){
		return Update_TokenReceivedChecked(identifier, rootToken, json);
	}
	else{
		Log::insert(Log::TYPE_ERROR, "Error en la actualización del usuario " + identifier + ". No se recibió el token.");
		return "{ \"result\" : \"ERROR\" ,  \"message\" : \"Null token\" }";
	}
}

string UserHandler::Update_TokenReceivedChecked(string identifier, Json::Value rootToken, string json){
	User user;
	if(user.Load(identifier) && user.GetIsActive()){
		return UserHandler::Update_ExistentUserChecked(identifier, rootToken, json, user);
	}
	else{
		Log::insert(Log::TYPE_ERROR, "Error en la actualización del usuario " + identifier + ". Usuario inexistente.");
		return "{ \"result\" : \"ERROR\" ,  \"message\" : \"Invalid user\" }";
	}
}

string UserHandler::Update_ExistentUserChecked(string identifier, Json::Value rootToken, string json, User user){
	if(Token::IsGuidValid(identifier, rootToken.asString())){
		std::set<string> ignoredAttributes;
		ignoredAttributes.insert("identifier");
		ignoredAttributes.insert("md5");
		ignoredAttributes.insert("root");
		ignoredAttributes.insert("quotaLimitMB");
		ignoredAttributes.insert("isActive");

		user.SetAllAttributes(json, ignoredAttributes);

		user.Save();

		Log::insert(Log::TYPE_INFORMATION, "Actualizado exitosamente el usuario " + identifier);
		return "{ \"result\" : \"OK\" ,  \"message\" : \"\" }";
	}
	else{
		Log::insert(Log::TYPE_ERROR, "Error en la actualización del usuario " + identifier + ". Token inválido.");
		return "{ \"result\" : \"ERROR\" ,  \"message\" : \"Invalid token\" }";
	}
}

string UserHandler::Delete(string identifier, string guid){
	User user;
	if(user.Load(identifier) && user.GetIsActive()){
		return Delete_ExistentUserChecked(user, guid);
	}
	else{
		Log::insert(Log::TYPE_ERROR, "Error en la eliminación del usuario " + identifier + ". Usuario inválido.");
		return "{ \"result\" : \"ERROR\" ,  \"message\" : \"Invalid user\" }";
	}
}

string UserHandler::Delete_ExistentUserChecked(User user, string guid){
	if(Token::IsGuidValid(user.GetIdentifier(), guid)){
		user.SetIsActive(false);
		user.Save();
		Log::insert(Log::TYPE_INFORMATION, "Eliminado exitosamente el usuario " + user.GetIdentifier());
		return "{ \"result\" : \"OK\" ,  \"message\" : \"\" }";
	}
	else{
		Log::insert(Log::TYPE_ERROR, "Error en la eliminación del usuario " + user.GetIdentifier() + ". Token inválido.");
		return "{ \"result\" : \"ERROR\" ,  \"message\" : \"Invalid token\" }";
	}
}

int UserHandler::GetConfigQuotaLimitMB(){
	ifstream logFile;
	logFile.open("server.config", ios::in | ios::binary);
	string line;
	while (getline(logFile, line)){
		if(line.find("QUOTA_LIMIT_MB") != std::string::npos){
			int limit = stoi(line.substr(line.find("=")+1));
			logFile.close();
			return limit;
		}
	}
	logFile.close();
	return 800000;
}

