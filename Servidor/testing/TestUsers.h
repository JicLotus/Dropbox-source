/*
 * TestFiles.h
 *
 *  Created on: 21 de set. de 2015
 *      Author: kevin
 */

#ifndef TESTUSERS_H_
#define TESTUSERS_H_

#include <gtest/gtest.h>
#include <time.h>
#include "../src/jsoncpp/json.h"
#include "../src/entities/User.h"
#include "../src/entities/File.h"

class TestUsers : public testing::Test {
public:
	TestUsers();
	void TestBody() {}
	void testLoadUserIdNotFound();
	void testGenerateUser();
	void testLoadUser();
	void testUserJsonParse();
	void testAddOwnFileWithFolder();
	void testAddAndDeleteSharedFile();
};

TEST_F(TestUsers, LoadUserIdNotFoundTest) {
	TestUsers tester;
	tester.testLoadUserIdNotFound();
}

TEST_F(TestUsers, GenerateUserTest) {
	TestUsers tester;
	tester.testGenerateUser();
}

TEST_F(TestUsers, LoadUser) {
	TestUsers tester;
	tester.testLoadUser();
}

TEST_F(TestUsers, JsonParse) {
	TestUsers tester;
	tester.testUserJsonParse();
}

TEST_F(TestUsers, AddOwnFileWithFolder) {
	TestUsers tester;
	tester.testAddOwnFileWithFolder();
}

TEST_F(TestUsers, AddAndDeleteSharedFile) {
	TestUsers tester;
	tester.testAddAndDeleteSharedFile();
}

#endif /* TESTUSERS_H_ */
