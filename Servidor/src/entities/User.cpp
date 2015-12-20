/*
 * User.cpp
 *
 *  Created on: Aug 30, 2015
 *      Author: pablo
 */

#include "User.h"

User::User() {
	// TODO Auto-generated constructor stub
}

User::~User() {
	// TODO Auto-generated destructor stub
}

bool User::Load(string identifier){

	string jsonUser = RocksdbHandler::Get("user_" + identifier, PATH_DB_USERS);

	set<string> ignoredAttributes;
	return SetAllAttributes(jsonUser, ignoredAttributes);
}

//Setea todos los atributos que se hayan pasado en el json
bool User::SetAllAttributes(string json, std::set<string> ignoredAttributes){
	Json::Value root;
	Json::Reader reader;
	bool parsedSuccess = reader.parse(json, root, false);

	if(parsedSuccess)
	{
		 if(not root["identifier"].isNull()){
			if(ignoredAttributes.find("identifier") == ignoredAttributes.end())
				this->identifier = root["identifier"].asString();
		 }

		 if(not root["name"].isNull()){
			 if(ignoredAttributes.find("name") == ignoredAttributes.end())
				 name = root["name"].asString();
		 }

		 if(not root["lastName"].isNull()){
			 if(ignoredAttributes.find("lastName") == ignoredAttributes.end())
				 lastName = root["lastName"].asString();
		 }

		 if(not root["md5"].isNull()){
			 if(ignoredAttributes.find("md5") == ignoredAttributes.end())
				 md5 = root["md5"].asString();
		 }

		 if(not root["quotaMB"].isNull()){
			 if(ignoredAttributes.find("quotaMB") == ignoredAttributes.end())
				 quotaMB = (int)root["quotaMB"].asInt();
		 }

		 if(not root["quotaLimitMB"].isNull()){
			 if(ignoredAttributes.find("quotaLimitMB") == ignoredAttributes.end())
				 quotaLimitMB = (int)root["quotaLimitMB"].asInt();
		 }

		 if(not root["email"].isNull()){
			 if(ignoredAttributes.find("email") == ignoredAttributes.end())
				 email = root["email"].asString();
		 }

		 if(not root["avatar"].isNull()){
			 if(ignoredAttributes.find("avatar") == ignoredAttributes.end())
				 avatar = root["avatar"].asString();
		 }

		 if(not root["lastLatitude"].isNull()){
			 if(ignoredAttributes.find("lastLatitude") == ignoredAttributes.end())
				 lastLatitude = root["lastLatitude"].asString();
		 }

		 if(not root["lastLongitude"].isNull()){
			 if(ignoredAttributes.find("lastLongitude") == ignoredAttributes.end())
				 lastLongitude = root["lastLongitude"].asString();
		 }

		 if(not root["isActive"].isNull()){
			 if(ignoredAttributes.find("isActive") == ignoredAttributes.end())
				 isActive = root["isActive"].asBool();
		 }

		 if(not root["ownFiles"].isNull()){
			 if(ignoredAttributes.find("ownFiles") == ignoredAttributes.end())
			 {
				 const Json::Value jownFiles = root["ownFiles"];
				 SetOwnFilesByJson(root["ownFiles"], NULL);
			 }
		 }

		 if(not root["sharedFiles"].isNull()){
			 if(ignoredAttributes.find("sharedFiles") == ignoredAttributes.end())
			 {
				 const Json::Value jownFiles = root["sharedFiles"];
				 SetSharedFilesByJson(root["sharedFiles"]);
			 }
		 }

		return true;
	}
	else
		return false;
}

void User::SetOwnFilesByJson(Json::Value root, UserContent* father)
{
	 for (unsigned int index = 0; index < root.size(); ++index )
	 {
		 UserContent* userContent = new UserContent();
		 userContent->identifier = root[index]["identifier"].asInt();
		 userContent->type = root[index]["type"].asInt();
		 if(userContent->type == 2)
			 SetOwnFilesByJson((Json::Value)root[index]["content"], userContent);
		 if(father)
			 father->content.push_back(userContent);
		 else
			 ownFiles.push_back(userContent);
	 }
}

void User::SetSharedFilesByJson(Json::Value root)
{
	 for (unsigned int index = 0; index < root.size(); ++index )
		 sharedFiles.push_back(root[index].asInt());
}

bool User::AddOwnFile(File* father, File* file){
	UserContent* userContent = new UserContent();
	userContent->identifier = file->GetIdentifier();
	userContent->type = file->GetType();

	if(father){
		UserContent* userContentFather = GetUserContentByFather(ownFiles, father);
		if(userContentFather)
			userContentFather->content.push_back(userContent);
		else
			return false;
	}
	else
		ownFiles.push_back(userContent);

	return true;
}

UserContent* User::GetUserContentByFather(std::list<UserContent*> list, File* father) {
	for (std::list<UserContent*>::iterator it=list.begin(); it != list.end(); ++it)
	{
		if((*it)->identifier == father->GetIdentifier())
			return (*it);
		else
			if((*it)->type == 2)
			{
				UserContent* userContent = GetUserContentByFather((*it)->content, father);
				if(userContent)
					return userContent;
			}
	}
	return NULL;
}

bool User::AddSharedFile(File* file){
	sharedFiles.push_back(file->GetIdentifier());
	return true;
}

bool User::DeleteSharedFile(File* file){
	sharedFiles.remove(file->GetIdentifier());
	return true;
}

string User::GetJson(bool full){
	string json = "{"
					"\"identifier\" : \"" + identifier + "\", "
					"\"isActive\" : " + std::to_string(isActive) + ", "
					"\"name\" : \"" + name + "\", "
					"\"lastName\" : \"" + lastName + "\", "
					"\"md5\" : \"" + md5 + "\", "
					"\"quotaMB\" : " + std::to_string(quotaMB) + ", "
					"\"quotaLimitMB\" : " + std::to_string(quotaLimitMB) + ", "
					"\"email\" : \"" + email + "\", "
					"\"avatar\" : \"" + avatar + "\", "
					"\"lastLatitude\" : \"" + lastLatitude + "\", "
					"\"lastLongitude\" : \"" + lastLongitude + "\", "
					"\"ownFiles\": [";


	json += GetJsonByUserContentList(ownFiles, full);
	json += "], ";

	json += "\"sharedFiles\": [";
	json += GetJsonBySharedFilesList(full);
	json += "]";

	json += "}";
	return json;
}

string User::GetJsonByUserContentList(std::list<UserContent*> list, bool full)
{
	string json;
	for (std::list<UserContent*>::iterator it=list.begin(); it != list.end(); ++it)
	{
		json += "{";
		json += "\"identifier\" : " + std::to_string((*it)->identifier)  + ",";
		json += "\"type\" : " + std::to_string((*it)->type) + ",";

		if(full)
		{
			File* file = new File();
			file->Load((*it)->identifier);

			json += "\"name\" : \"" + file->GetName() + "\",";
			json += "\"extension\" : \"" + file->GetExtension() + "\",";
			json += "\"sizeMB\" : " + std::to_string(file->GetSize()) + ",";
			json += "\"owner\" : " + file->GetOwner() + ",";
			json += "\"tags\" : " + file->GetTagsStr() + ",";
			if(file->IsDeleted())
				json += "\"delete\" : true,";
			else
				json += "\"delete\" : false,";
			json += "\"lastModifiedBy\" : \"" + file->GetLastModifiedBy() + "\", ";
			json += "\"lastModifiedOn\" : \"" + to_string(file->GetLastModifiedOn())+ "\",";
			json += "\"lastVersion\" : " + std::to_string(file->GetLastVersion()) + ",";
			json += "\"versions\" :" + file->GetVersionsStr() + ",";

			delete file;
		}

		json += "\"content\" : [";
		if((*it)->type == 2)
			json += GetJsonByUserContentList((*it)->content, full);
		json += "]";
		json += "},";
	}
	return json.substr(0, json.length() - 1);
}

string User::GetJsonBySharedFilesList(bool full)
{
	string json;
	for (std::list<int>::iterator it=sharedFiles.begin(); it != sharedFiles.end(); ++it)
	{
		if(full)
		{
			File* file = new File();
			file->Load(*it);

			json += "{";
			json += "\"identifier\" : " + std::to_string(file->GetIdentifier())  + ",";
			json += "\"type\" : " + std::to_string(file->GetType()) + ",";
			json += "\"name\" : \"" + file->GetName() + "\",";
			json += "\"extension\" : \"" + file->GetExtension() + "\",";
			json += "\"sizeMB\" : " + std::to_string(file->GetSize()) + ",";
			json += "\"owner\" : " + file->GetOwner() + ",";
			json += "\"tags\" : " + file->GetTagsStr() + ",";
			if(file->IsDeleted())
				json += "\"delete\" : true,";
			else
				json += "\"delete\" : false,";
			json += "\"lastModifiedOn\" : \"" + to_string(file->GetLastModifiedOn()) + "\",";
			json += "\"lastModifiedBy\" : \"" + file->GetLastModifiedBy() + "\",";
			json += "\"lastVersion\" : " + std::to_string(file->GetLastVersion()) + ",";
			json += "\"versions\" : " + file->GetVersionsStr();

			delete file;

			json += "},";

		}
		else
			json += std::to_string(*it) + ",";
	}
	return json.substr(0, json.length() - 1);
}

bool User::Save()
{
	string json = GetJson(false);
	RocksdbHandler::Insert("user_" + GetIdentifier(), json, PATH_DB_USERS);
	return true;
}

void User::SetIdentifier(string identifier){
	this->identifier = identifier;
}

string User::GetIdentifier(){
	return this->identifier;
}

void User::SetName(string name){
	this->name = name;
}

string User::GetName(){
	return this->name;
}

void User::SetLastName(string lastname){
	this->lastName = lastname;
}

string User::GetLastName(){
	return this->lastName;
}

void User::SetMD5(string md5){
	this->md5 = md5;
}

string User::GetMD5(){
	return this->md5;
}

void User::SetQuotaMB(int quotaMB){
	this->quotaMB = quotaMB;
}

int User::GetQuotaMB(){
	return this->quotaMB;
}

void User::SetQuotaLimitMB(int quotaLimitMB){
	this->quotaLimitMB = quotaLimitMB;
}

int User::GetQuotaLimitMB(){
	return this->quotaLimitMB;
}

void User::SetEmail(string email){
	this->email = email;
}

string User::GetEmail(){
	return this->email;
}

void User::SetAvatar(string avatar){
	this->avatar = avatar;
}

string User::GetAvatar(){
	return this->avatar;
}

void User::SetLastLatitude(string lastLatitude){
	this->lastLatitude = lastLatitude;
}

string User::GetLastLatitude(){
	return this->lastLatitude;
}

void User::SetLastLongitude(string lastLongitude){
	this->lastLongitude = lastLongitude;
}

string User::GetLastLongitude(){
	return this->lastLongitude;
}

void User::SetIsActive(bool isActive){
	this->isActive = isActive;
}

bool User::GetIsActive(){
	return isActive;
}
