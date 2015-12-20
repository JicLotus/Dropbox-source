/*
 * TestToken.h
 *
 *  Created on: 21 de set. de 2015
 *      Author: pablo
 */

#ifndef TESTTOKEN_H_
#define TESTTOKEN_H_

#include <gtest/gtest.h>
#include "../src/entities/Token.h"

class TestToken : public testing::Test {
public:
	TestToken();
	void TestBody() {}
	void testLoadTokenIdNotFound();
	void testGenerateAndLoadToken();
};

TEST_F(TestToken, testLoadTokenIdNotFound) {
	TestToken tester;
	tester.testLoadTokenIdNotFound();
}

TEST_F(TestToken, GenerateAndLoadToken) {
	TestToken tester;
	tester.testGenerateAndLoadToken();
}

#endif /* TESTTOKEN_H_ */
