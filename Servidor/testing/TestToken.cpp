/*
 * TestToken.cpp
 *
 *  Created on: 21 de set. de 2015
 *      Author: pablo
 */

#include <gtest/gtest.h>
#include "TestToken.h"

TestToken::TestToken() {
	// TODO Auto-generated constructor stub

}

void TestToken::testLoadTokenIdNotFound() {
	Token token;
	ASSERT_FALSE(token.Load("123"));
}

void TestToken::testGenerateAndLoadToken() {
	Token* token = new Token();
	token->Save("pablo", token->GenerateGuid());
	ASSERT_TRUE(token->Load("pablo"));
	ASSERT_TRUE(token->IsGuidValid("pablo", token->GenerateGuid()));
}
