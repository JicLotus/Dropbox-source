/*
 * TestFiles.h
 *
 *  Created on: 21 de set. de 2015
 *      Author: kevin
 */

#ifndef TESTFILEHANDLER_H_
#define TESTFILEHANDLER_H_

#include <gtest/gtest.h>
#include <time.h>
#include "../src/jsoncpp/json.h"
#include "../src/entities/User.h"
#include "../src/entities/File.h"
#include "../src/mongoose/FileHandler.h"
#include "../src/mongoose/UserHandler.h"
#include "../src/rocksDb/RocksdbHandler.h"

class TestFileHandler : public testing::Test {
public:
	TestFileHandler();
	void TestBody() {}
	void testInsertFile();
	void testInsertFolder();
	void testGetMetadata();
	void testGetMetadataOfInvalidFile();
	void testGetDocument();
	void testGetDocumentOfInvalidFile();
	void testGetDocumentPreviousVersion();
	void testDeleteFile();
	void testDeleteFileOfInvalidFile();
	void testUpdateFile();
	void testAddAndDeleteSharedFile();
	void testAddSharedFileWithInvalidJson();
};

TEST_F(TestFileHandler, InsertFile) {
	TestFileHandler tester;
	tester.testInsertFile();
}

TEST_F(TestFileHandler, InsertFolder) {
	TestFileHandler tester;
	tester.testInsertFolder();
}

TEST_F(TestFileHandler, GetMetadata) {
	TestFileHandler tester;
	tester.testGetMetadata();
}

TEST_F(TestFileHandler, GetMetadataOfInvalidFile) {
	TestFileHandler tester;
	tester.testGetMetadataOfInvalidFile();
}

TEST_F(TestFileHandler, testGetDocument) {
	TestFileHandler tester;
	tester.testGetDocument();
}

TEST_F(TestFileHandler, testGetDocumentOfInvalidFile) {
	TestFileHandler tester;
	tester.testGetDocumentOfInvalidFile();
}

TEST_F(TestFileHandler, GetDocumentPreviousVersion) {
	TestFileHandler tester;
	tester.testGetDocumentPreviousVersion();
}

TEST_F(TestFileHandler, DeleteFile) {
	TestFileHandler tester;
	tester.testDeleteFile();
}

TEST_F(TestFileHandler, DeleteFileOfInvalidFile) {
	TestFileHandler tester;
	tester.testDeleteFileOfInvalidFile();
}

TEST_F(TestFileHandler, UpdateFile) {
	TestFileHandler tester;
	tester.testUpdateFile();
}

TEST_F(TestFileHandler, AddAndDeleteSharedFile) {
	TestFileHandler tester;
	tester.testAddAndDeleteSharedFile();
}

TEST_F(TestFileHandler, AddSharedFileOfInvalidFile) {
	TestFileHandler tester;
	tester.testAddSharedFileWithInvalidJson();
}
#endif /* TESTFILEHANDLER_H_ */
