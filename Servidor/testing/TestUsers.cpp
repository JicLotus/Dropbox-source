/*
 * TestUsers.cpp
 *
 *  Created on: 21 de set. de 2015
 *      Author: kevin
 */

#include <gtest/gtest.h>
#include "TestUsers.h"

TestUsers::TestUsers() {
	// TODO Auto-generated constructor stub

}

void TestUsers::testLoadUserIdNotFound() {
	User user;
	ASSERT_FALSE(user.Load("123"));
}

void TestUsers::testGenerateUser() {
	User* user = new User();
	user->SetIdentifier("Pablo");
	user->SetAvatar("http://img.google.com/test.png");
	user->SetEmail("pablo@albanese.com");
	user->SetName("Pablo");
	user->SetLastName("Albanese");
	user->SetIsActive(true);
	user->SetMD5("md5");
	user->SetQuotaMB(100);
	user->SetQuotaLimitMB(1000);
	user->SetLastLatitude("32.454554");
	user->SetLastLongitude("32.443433");
	ASSERT_TRUE(user->Save());
}

void TestUsers::testLoadUser() {
	User* user = new User();
	ASSERT_TRUE(user->Load("Pablo"));
	ASSERT_TRUE(user->GetIsActive() == true);
	ASSERT_TRUE(user->GetQuotaMB() == 100);
	ASSERT_TRUE(user->GetQuotaLimitMB() == 1000);
	ASSERT_STREQ(user->GetIdentifier().c_str(), "Pablo");
	ASSERT_STREQ(user->GetAvatar().c_str(), "http://img.google.com/test.png");
	ASSERT_STREQ(user->GetEmail().c_str(), "pablo@albanese.com");
	ASSERT_STREQ(user->GetLastLatitude().c_str(), "32.454554");
	ASSERT_STREQ(user->GetLastLongitude().c_str(), "32.443433");
	ASSERT_STREQ(user->GetName().c_str(), "Pablo");
	ASSERT_STREQ(user->GetLastName().c_str(), "Albanese");
	ASSERT_STREQ(user->GetMD5().c_str(), "md5");
}

void TestUsers::testUserJsonParse() {
	string json = "{\"identifier\": \"pabloalbanese\", \"name\": \"name1\", \"md5\": \"passwd1\", \"email\" : \"test@test.com\"}";
	User* user = new User();
	std::set<string> ignoredAttributes;
	user->SetAllAttributes(json, ignoredAttributes);
	ASSERT_STREQ(user->GetIdentifier().c_str(), "pabloalbanese");
}

void TestUsers::testAddOwnFileWithFolder(){

	User* user = new User();
	user->Load("Pablo");
	File* fileFolder = new File();
	fileFolder->SetIdentifier(10);
	fileFolder->SetType(2);
	fileFolder->Save();
	File* file = new File();
	file->SetIdentifier(20);
	file->SetType(1);
	file->Save();
	ASSERT_TRUE(user->AddOwnFile(NULL, fileFolder));
	//ASSERT_TRUE(user->AddOwnFile(fileFolder, file));
	//user->GetJson(true);
	//ASSERT_TRUE(user->Save());
	//ASSERT_TRUE(user->Load("Pablo"));
}

void TestUsers::testAddAndDeleteSharedFile(){
	User* user = new User();
	ASSERT_TRUE(user->Load("Pablo"));
	File* file = new File();
	file->SetIdentifier(1);
	file->SetType(1);
	file->Save();
	ASSERT_TRUE(user->AddSharedFile(file));
	ASSERT_TRUE(user->Save());
	ASSERT_TRUE(user->Load("Pablo"));
	ASSERT_TRUE(user->DeleteSharedFile(file));
}
