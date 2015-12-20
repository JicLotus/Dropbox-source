/*
 * File.cpp
 *
 *  Created on: 9 de set. de 2015
 *      Author: kevin
 */

#include "FileContent.h"

FileContent::FileContent() {
	// TODO Auto-generated constructor stub

}

void FileContent::Generate(Json::Value root) {
	 if(not root["identifier"].isNull())
		identifier = root["identifier"].asString();

	 if(not root["content"].isNull())
		 content = root["content"].asString();
}

bool FileContent::Load(string identifier){

	string jsonFile = RocksdbHandler::Get("filecontent_" + identifier, PATH_DB_FILES);

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

void FileContent::SetIdentifier(string identifier)
{
	this->identifier = identifier;
}

string FileContent::GetIdentifier()
{
	return identifier;
}

string FileContent::GetContent() {
	return content;
}

void FileContent::SetContent(string content) {
	this->content = content;
}

bool FileContent::Save() {
	string json = GetJson();
	string key = "filecontent_" + identifier;
	RocksdbHandler::Insert(key, json, PATH_DB_FILES);
	return true;
}

string FileContent::GetJson(){
	string json = "{"
					"\"identifier\" : \"" + identifier + "\", "
					"\"content\" : \"" + content + "\" }";
	return json;
}

FileContent::~FileContent() {
	// TODO Auto-generated destructor stub
}

