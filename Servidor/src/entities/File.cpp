/*
 * File.cpp
 *
 *  Created on: 9 de set. de 2015
 *      Author: kevin
 */

#include "File.h"

File::File() {
	// TODO Auto-generated constructor stub

}

int File::CreateId() {
	 string value = RocksdbHandler::Get("lastId", PATH_DB_FILES);
	 int newId;
	 if (value == "")
		 newId = 1;
	 else
		 newId = stoi(value) + 1;
	 RocksdbHandler::Insert("lastId", to_string(newId), PATH_DB_FILES);
	 identifier = newId;
	 return newId;
}

void File::Generate(Json::Value root) {
	 if(not root["id"].isNull())
		identifier = root["id"].asInt();

	 if(not root["name"].isNull())
		 name = root["name"].asString();

	 if(not root["extension"].isNull())
		 extension = root["extension"].asString();

	 if(not root["owner"].isNull())
		 owner = root["owner"].asString();

	 if(not root["sizeMB"].isNull())
	 	size = root["sizeMB"].asInt();

	if(not root["type"].isNull())
		type = root["type"].asInt();
	else
		type = 1;

	 if(not root["lastModifiedBy"].isNull())
		 lastModifiedBy = root["lastModifiedBy"].asString();

	if(not root["lastModifiedOn"].isNull())
		 lastModifiedOn = (time_t) root["lastModifiedOn"].asInt();

	if(not root["lastVersion"].isNull())
		 lastVersion = root["lastVersion"].asInt();

	if(not root["delete"].isNull())
		 deleted = root["delete"].asBool();
	else
		deleted = false;

	if(not root["tags"].isNull()) {
		const Json::Value jsonTags = root["tags"];
		tags.clear();
		for (unsigned int index = 0; index < jsonTags.size(); ++index ) {
			tags.push_back(jsonTags[index].asString());
		}
	}

	if(not root["versions"].isNull()) {
		const Json::Value jsonVersions = root["versions"];
		for (unsigned int index = 0; index < jsonVersions.size(); ++index ) {
			FileVersion* v = new FileVersion();
			v->version = jsonVersions[index]["version"].asInt();
			v->fileContentIdentifier = jsonVersions[index]["file"].asString();
			v->date = (time_t) jsonVersions[index]["date"].asInt();
			versions.push_back(v);
		}
	}

}

bool File::Load(int identifier){

	string jsonFile = RocksdbHandler::Get("file_" + to_string(identifier), PATH_DB_FILES);

	Json::Value root;
	Json::Reader reader;
	bool parsedSuccess = reader.parse(jsonFile, root, false);

	if(parsedSuccess)
	{
		this->identifier = identifier;
		Generate(root);
		return true;
	}
	else
		return false;
}

bool File::IsDeleted() {
	return deleted;
}

void File::SetDeleted(bool deleted) {
	this->deleted = deleted;
}

string File::GetExtension() {
	return extension;
}

void File::SetExtension(string extension) {
	this->extension = extension;
}

int File::GetIdentifier() {
	return identifier;
}

void File::SetIdentifier(int identifier) {
	this->identifier = identifier;
}

time_t File::GetLastModifiedOn() {
	return lastModifiedOn;
}

void File::SetLastModifiedOn(time_t lastModifiedOn) {
	this->lastModifiedOn = lastModifiedOn;
}

string File::GetLastModifiedBy() {
	return lastModifiedBy;
}

void File::SetLastModifiedBy(string lastModifiedBy) {
	this->lastModifiedBy = lastModifiedBy;
}

string File::GetName() {
	return name;
}

void File::SetName(string name) {
	this->name = name;
}

string File::GetOwner() {
	return owner;
}

void File::SetOwner(string owner) {
	this->owner = owner;
}

int File::GetSize() {
	return size;
}

void File::SetSize(int size) {
	this->size = size;
}

int File::GetType() {
	return type;
}

void File::SetType(int type) {
	this->type = type;
}

int File::GetLastVersion() {
	return lastVersion;
}

void File::SetLastVersion(int lastVersion) {
	this->lastVersion = lastVersion;
}

void File::AddVersion(FileVersion* fileVersion){
	this->lastVersion++;
	this->lastModifiedOn = time(0);
	versions.push_back(fileVersion);
}

FileVersion* File::GetVersion(int version){
	if(versions.size() >= version)
		return versions[version - 1];
	else
		return NULL;
}

bool File::Save() {
	string json = GetJson();
	string key = "file_" + to_string(identifier);
	RocksdbHandler::Insert(key, json, PATH_DB_FILES);
	return true;
}

string File::GetJson(){
	string json = "{"
					"\"id\" : " + to_string(identifier) + ", "
					"\"name\" : \"" + name + "\", "
					"\"extension\" : \"" + extension + "\", "
					"\"owner\" : \"" + owner + "\", "
					"\"type\" : " + to_string(type) + ", "
					"\"sizeMB\" : " + to_string(size) + ", "
					"\"delete\" : " + std::to_string(deleted) + ", "
					"\"lastModifiedBy\" : \"" + lastModifiedBy + "\", "
					"\"lastModifiedOn\" : " + to_string(lastModifiedOn) + ", "
					"\"tags\" : " + GetTagsStr() + ", "
					"\"versions\" : " + GetVersionsStr() + ", "
					"\"lastVersion\" : " + to_string(lastVersion);
	json += "}";
	return json;
}

string File::GetDeletedStr() {
	if (deleted)
		return "true";
	return "false";
}

string File::GetTagsStr() {
	string tagsList = "[";
	for (unsigned int i = 0; i < tags.size(); ++i ) {
		tagsList = tagsList + "\"" + tags[i] + "\"";
		if (i < tags.size() - 1)
			tagsList += ",";
	}
	tagsList += "]";
	return tagsList;

}

string File::GetVersionsStr() {
	string versionsList = "[";
	for (unsigned int i = 0; i < versions.size(); ++i ) {
		versionsList = versionsList +
				"{\"version\": " + to_string(versions[i]->version) +
				", \"file\": \"" + versions[i]->fileContentIdentifier + "\"" +
				", \"date\": "+ to_string(versions[i]->date) + "}";
		if (i < versions.size() - 1)
			versionsList += ",";
	}
	versionsList += "]";
	return versionsList;

}

File::~File() {
	for (unsigned int i = 0; i < versions.size(); ++i )
		delete versions[i];
}

