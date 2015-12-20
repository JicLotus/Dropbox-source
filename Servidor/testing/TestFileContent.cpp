/*
 * TestToken.cpp
 *
 *  Created on: 21 de set. de 2015
 *      Author: pablo
 */

#include <gtest/gtest.h>
#include "TestFileContent.h"

TestFileContent::TestFileContent() {
	// TODO Auto-generated constructor stub

}

void TestFileContent::testLoadContentIdNotFound() {
	FileContent fileContent;
	ASSERT_FALSE(fileContent.Load("123"));
}

void TestFileContent::testGenerateAndLoadContent() {
	FileContent* fileContent = new FileContent();
	fileContent->SetContent("1234");
	fileContent->SetIdentifier("1");
	fileContent->Save();

	FileContent* fileContentLoad = new FileContent();
	ASSERT_TRUE(fileContentLoad->Load("1"));
	fileContentLoad->GetJson();
	fileContentLoad->GetContent();
	fileContentLoad->GetIdentifier();
}
