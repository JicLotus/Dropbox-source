/*
 * TestUsers.cpp
 *
 *  Created on: 21 de set. de 2015
 *      Author: kevin
 */

#include <gtest/gtest.h>
#include "TestFileHandler.h"

TestFileHandler::TestFileHandler() {
	// TODO Auto-generated constructor stub
}

void TestFileHandler::testInsertFile(){
	RocksdbHandler::Delete("user_testunit3", PATH_DB_USERS);
	UserHandler userHandler;
	string response = userHandler.Insert("{\"identifier\": \"testunit3\", \"name\": \"name1\", \"md5\": \"passwd1\", \"email\" : \"test@test.com\"}");

	FileHandler fileHandler;

	response = fileHandler.Insert("{\"name\":\"Carpeta\", \"type\" : 2, \"owner\" : \"testunit3\"}");
	Json::Value root;
	Json::Reader reader;
	reader.parse(response, root, false);
	int metafile = root["metafile"].asInt();

	response = fileHandler.Insert("{\"name\":\"nombre1.txt\", \"" + std::to_string(metafile) + "\", \"extension\":\"txt\", \"content\":\"ZGF0YSB0byBiZSBlbmNvZGVk\", \"type\" : 1, \"owner\" : \"testunit3\"}");
	ASSERT_TRUE(response.find("\"result\" : \"OK\"") > 0);
}

void TestFileHandler::testInsertFolder(){
	FileHandler fileHandler;
	string response = fileHandler.Insert("{\"name\":\"Carpeta\", \"type\" : 2, \"owner\" : \"testunit3\"}");
	ASSERT_TRUE(response.find("\"result\" : \"OK\"") > 0);
}


void TestFileHandler::testGetMetadata() {
	FileHandler fileHandler;
	string response = fileHandler.Insert("{\"name\":\"nombre1.txt\", \"extension\":\"txt\", \"content\":\"ZGF0YSB0byBiZSBlbmNvZGVk\", \"type\" : 1, \"owner\" : \"testunit3\"}");
	Json::Value root;
	Json::Reader reader;
	reader.parse(response, root, false);
	int metafile = root["metafile"].asInt();
	string response2 = fileHandler.GetMetadata(std::to_string(metafile), "");
	reader.parse(response2, root, false);
	string name = root["name"].asString();
	ASSERT_STREQ(name.c_str(), "nombre1.txt");
}

void TestFileHandler::testGetMetadataOfInvalidFile() {
	FileHandler fileHandler;
	string response = fileHandler.GetMetadata("0", "");
	ASSERT_STREQ(response.c_str(), "{ \"result\" : \"ERROR\" ,  \"message\" : \"Invalid identifier\" }");
}

void TestFileHandler::testGetDocument() {
	FileHandler fileHandler;
	string response = fileHandler.Insert("{\"name\":\"nombre1.txt\", \"extension\":\"txt\", \"content\":\"ZGF0YSB0byBiZSBlbmNvZGVk\", \"type\" : 1, \"owner\" : \"testunit3\"}");
	Json::Value root;
	Json::Reader reader;
	reader.parse(response, root, false);
	int metafile = root["metafile"].asInt();
	response = fileHandler.GetDocument(std::to_string(metafile), "");
	reader.parse(response, root, false);
	string content = root["content"].asString();
	ASSERT_STREQ(content.c_str(), "ZGF0YSB0byBiZSBlbmNvZGVk");
}

void TestFileHandler::testGetDocumentOfInvalidFile() {
	FileHandler fileHandler;
	string response = fileHandler.GetDocument("0", "");
	ASSERT_STREQ(response.c_str(), "{ \"result\" : \"ERROR\" ,  \"message\" : \"Invalid identifier\" }");
}

void TestFileHandler::testGetDocumentPreviousVersion() {
	FileHandler fileHandler;
	string response = fileHandler.Insert("{\"name\":\"nombre1.txt\", \"extension\":\"txt\", \"content\":\"ZGF0YSB0byBiZSBlbmNvZGVk\", \"type\" : 1, \"owner\" : \"testunit3\"}");
	Json::Value root;
	Json::Reader reader;
	reader.parse(response, root, false);
	int metafile = root["metafile"].asInt();
	fileHandler.Insert("{\"id\":" + std::to_string(metafile) + ", \"name\":\"nombre1.txt\", \"extension\":\"txt\", \"content\":\"ZGF0YSB0byBiZSBlbmNvZGVk\", \"type\" : 1, \"owner\" : \"user_testunit3\"}");
	response = fileHandler.GetDocument(std::to_string(metafile), "", 1);
	reader.parse(response, root, false);
	string content = root["content"].asString();
	ASSERT_STREQ(content.c_str(), "ZGF0YSB0byBiZSBlbmNvZGVk");
}

void TestFileHandler::testDeleteFile(){
	FileHandler fileHandler;
	string response = fileHandler.Insert("{\"name\":\"nombre1.txt\", \"extension\":\"txt\", \"content\":\"ZGF0YSB0byBiZSBlbmNvZGVk\", \"type\" : 1, \"owner\" : \"testunit3\"}");
	Json::Value root;
	Json::Reader reader;
	reader.parse(response, root, false);
	int metafile = root["metafile"].asInt();
	response = fileHandler.Delete(std::to_string(metafile), "");
	ASSERT_STREQ(response.c_str(), "{ \"result\" : \"OK\" }");
}

void TestFileHandler::testDeleteFileOfInvalidFile(){
	FileHandler fileHandler;
	string response = fileHandler.Delete("0", "");
	ASSERT_STREQ(response.c_str(), "{ \"result\" : \"ERROR\" ,  \"message\" : \"Invalid identifier\" }");
}

void TestFileHandler::testUpdateFile(){
	FileHandler fileHandler;
	string response = fileHandler.Insert("{\"name\":\"nombre1.txt\", \"extension\":\"txt\", \"content\":\"ZGF0YSB0byBiZSBlbmNvZGVk\", \"type\" : 1, \"owner\" : \"testunit3\"}");
	Json::Value root;
	Json::Reader reader;
	reader.parse(response, root, false);
	int metafile = root["metafile"].asInt();
	response = fileHandler.Update(std::to_string(metafile), "{\"name\":\"nombre1.txt\", \"extension\":\"txt\", \"content\":\"ZGF0YSB0byBiZSBlbmNvZGVk\", \"type\" : 1, \"owner\" : \"testunit\"}");
	ASSERT_STREQ(response.c_str(), "{ \"result\" : \"OK\" ,  \"message\" : \"\" }");
}

void TestFileHandler::testAddAndDeleteSharedFile(){
	FileHandler fileHandler;
	string response = fileHandler.Insert("{\"name\":\"nombre1.txt\", \"extension\":\"txt\", \"content\":\"ZGF0YSB0byBiZSBlbmNvZGVk\", \"type\" : 1, \"owner\" : \"testunit3\"}");
	Json::Value root;
	Json::Reader reader;
	reader.parse(response, root, false);
	int metafile = root["metafile"].asInt();

	response = fileHandler.AddSharedFiles(std::to_string(metafile), "{\"users\": [ \"testunit3\" ]}");
	ASSERT_STREQ(response.c_str(), "{ \"result\" : \"OK\" ,  \"message\" : \"\" }");

	response = fileHandler.DeleteSharedFiles(std::to_string(metafile), "testunit3");
	ASSERT_STREQ(response.c_str(), "{ \"result\" : \"OK\" ,  \"message\" : \"\" }");
}

void TestFileHandler::testAddSharedFileWithInvalidJson(){
	FileHandler fileHandler;
	string response = fileHandler.AddSharedFiles("", "{\"users\": [ \"testunit\" ");
	ASSERT_STREQ(response.c_str(), "{ \"result\" : \"ERROR\" ,  \"message\" : \"Invalid JSON\" }");
}
