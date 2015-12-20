/*
 * TestFiles.cpp
 *
 *  Created on: 21 de set. de 2015
 *      Author: kevin
 */

#include <gtest/gtest.h>
#include "TestFiles.h"

TestFiles::TestFiles() {
	// TODO Auto-generated constructor stub

}

void TestFiles::testLoadIdNotFound() {
	File file;
	ASSERT_FALSE(file.Load(123));
}

void TestFiles::testGenerateFileFromJson() {
	string json = "{\"name\":\"nombre1.txt\", \"extension\":\"txt\", \"content\":\"ZGF0YSB0byBiZSBlbmNvZGVk\", \"type\" : 1}";
	Json::Value root;
	Json::Reader reader;
	bool parsedSuccess = reader.parse(json, root, false);

	File file;
	file.Generate(root);
	ASSERT_STREQ(file.GetName().c_str(), "nombre1.txt");
	ASSERT_STREQ(file.GetExtension().c_str(), "txt");
}

void TestFiles::testAddVersion() {
	time_t now = time(0);

	File* file = new File();
	file->CreateId();
	file->SetDeleted(false);
	file->SetExtension("txt");
	file->SetLastModifiedBy("Pablo");
	file->SetName("archivo1");
	file->SetSize(10);
	file->SetType(1);
	file->SetOwner("Pablo");
	file->SetLastModifiedOn(now);
	file->Save();
	FileVersion fileVersion;
	fileVersion.version = 1;

	fileVersion.date = now;
	file->AddVersion(&fileVersion);
	file->Save();
	file->GetJson();

	ASSERT_EQ(file->GetLastModifiedOn(), now);
	ASSERT_EQ(file->GetVersion(1)->version, 1);
}
