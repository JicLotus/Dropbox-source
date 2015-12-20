/*
 * TestFiles.h
 *
 *  Created on: 21 de set. de 2015
 *      Author: kevin
 */

#ifndef TESTFILES_H_
#define TESTFILES_H_

#include <gtest/gtest.h>
#include <time.h>
#include "../src/jsoncpp/json.h"
#include "../src/entities/FileContent.h"
#include "../src/entities/File.h"
#include "../src/entities/User.h"

class TestFiles : public testing::Test {
public:
	TestFiles();
	void TestBody() {}
	void testLoadIdNotFound();
	void testGenerateFileFromJson();
	void testAddVersion();
};

TEST_F(TestFiles, LoadFileIdNotFoundTest) {
	TestFiles tester;
	tester.testLoadIdNotFound();
}

TEST_F(TestFiles, GenerateFileFromJsonTest) {
	TestFiles tester;
	tester.testGenerateFileFromJson();
}

TEST_F(TestFiles, AddVersionTest) {
	TestFiles tester;
	tester.testAddVersion();
}
#endif /* TESTFILES_H_ */
