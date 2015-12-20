/*
 * TestToken.h
 *
 *  Created on: 21 de set. de 2015
 *      Author: pablo
 */

#ifndef TESTFILECONTENT_H_
#define TESTFILECONTENT_H_

#include <gtest/gtest.h>
#include "../src/entities/FileContent.h"

class TestFileContent : public testing::Test {
public:
	TestFileContent();
	void TestBody() {}
	void testLoadContentIdNotFound();
	void testGenerateAndLoadContent();
};

TEST_F(TestFileContent, testLoadContentIdNotFound) {
	TestFileContent tester;
	tester.testLoadContentIdNotFound();
}

TEST_F(TestFileContent, GenerateAndLoadContent) {
	TestFileContent tester;
	tester.testGenerateAndLoadContent();
}

#endif /* TESTFILECONTENT_H_ */
