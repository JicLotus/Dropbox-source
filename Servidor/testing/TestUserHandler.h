/*
 * TestFiles.h
 *
 *  Created on: 21 de set. de 2015
 *      Author: kevin
 */

#ifndef TESTUSERHANDLER_H_
#define TESTUSERHANDLER_H_

#include <gtest/gtest.h>
#include <time.h>
#include "../src/jsoncpp/json.h"
#include "../src/entities/User.h"
#include "../src/entities/File.h"
#include "../src/mongoose/UserHandler.h"
#include "../src/rocksDb/RocksdbHandler.h"

class TestUserHandler : public testing::Test {
public:
	TestUserHandler();
	void TestBody() {}
	void testGenerateUser();
	void testLoadUser();
	void testUpdateUser();
	void testDeleteUser();
};

TEST_F(TestUserHandler, GenerateUser) {
	TestUserHandler tester;
	tester.testGenerateUser();
}

TEST_F(TestUserHandler, LoadUser) {
	TestUserHandler tester;
	tester.testLoadUser();
}

TEST_F(TestUserHandler, UpdateUser) {
	TestUserHandler tester;
	tester.testUpdateUser();
}

TEST_F(TestUserHandler, DeleteUser) {
	TestUserHandler tester;
	tester.testDeleteUser();
}

#endif /* TESTUSERHANDLER_H_ */
