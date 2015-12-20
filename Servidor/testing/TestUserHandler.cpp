/*
 * TestUsers.cpp
 *
 *  Created on: 21 de set. de 2015
 *      Author: kevin
 */

#include <gtest/gtest.h>
#include "TestUserHandler.h"

TestUserHandler::TestUserHandler() {
	// TODO Auto-generated constructor stub

}

void TestUserHandler::testGenerateUser() {
	RocksdbHandler::Delete("user_testunit", PATH_DB_USERS);
	UserHandler userHandler;
	string response = userHandler.Insert("{\"identifier\": \"testunit\", \"name\": \"name1\", \"md5\": \"passwd1\", \"email\" : \"test@test.com\"}");
	ASSERT_STREQ(response.c_str(), "{ \"result\" : \"OK\" ,  \"message\" : \"\" }");
}

void TestUserHandler::testLoadUser() {
	UserHandler userHandler;
	string jsonResponse = userHandler.Auth("testunit","passwd1");
	Json::Value root;
	Json::Reader reader;
	reader.parse(jsonResponse, root, false);
	string token = root["token"].asString();
	jsonResponse = userHandler.Get("testunit", token);
	ASSERT_STRNE(jsonResponse.c_str(), "{ \"result\" : \"ERROR\" ,  \"message\" : \"Invalid identifier\" }");
}

void TestUserHandler::testUpdateUser() {
	UserHandler userHandler;
	string jsonResponse = userHandler.Auth("testunit","passwd1");
	Json::Value root;
	Json::Reader reader;
	reader.parse(jsonResponse, root, false);
	string token = root["token"].asString();
	jsonResponse = userHandler.Update("testunit", "{\"token\": \"" + token + "\", \"identifier\": \"testunit\", \"name\": \"name1\", \"email\" : \"test@test.com\"}");
    ASSERT_STREQ(jsonResponse.c_str(), "{ \"result\" : \"OK\" ,  \"message\" : \"\" }");
}

void TestUserHandler::testDeleteUser(){
	UserHandler userHandler;
	string jsonResponse = userHandler.Auth("testunit","passwd1");
	Json::Value root;
	Json::Reader reader;
	reader.parse(jsonResponse, root, false);
	string token = root["token"].asString();
	jsonResponse = userHandler.Delete("testunit", token);
	ASSERT_STREQ(jsonResponse.c_str(), "{ \"result\" : \"OK\" ,  \"message\" : \"\" }");
}
