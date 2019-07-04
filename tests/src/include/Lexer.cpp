/*
	The ALB Programming Language
	Alb Developers Team (C) 2019

	This software is distributed under the MIT license
	Visit https://github.com/albertonl/alb/LICENSE for further details
*/

#include <gtest/gtest.h>
#include <include/Lexer.hpp>
#include <absl/strings/match.h>
#include <include/Resources.hpp>

TEST(tokenizer_tests, tokenizer_tests_constructor_Test) {
  ASSERT_NO_THROW(alb_lang::Lexer{});
}

TEST(tokenizer_tests, tokenizer_tests_utf8_parsing_token_count_Test) {
  char testData[] = "BEGIN .\nEND";
  std::vector<alb_lang::Token*> tokens;
  alb_lang::Lexer::tokenizeUTF8(testData, sizeof(testData), tokens);
  ASSERT_EQ(tokens.size(), 3);
}

TEST(tokenizer_tests, tokenizer_tests_utf8_parsing_comment_Test) {
  char testData[] = "BEGIN .\n// This is a comment\nEND";
  std::vector<alb_lang::Token*> tokens;
  alb_lang::Lexer::tokenizeUTF8(testData, sizeof(testData), tokens);
  EXPECT_EQ(tokens.size(), 3);
  ASSERT_GE(tokens.size(), 3);
  EXPECT_EQ(tokens[0]->getTextContents(), std::string{"BEGIN"});
  EXPECT_EQ(tokens[1]->getTextContents(), std::string{"."});
  EXPECT_EQ(tokens[2]->getTextContents(), std::string{"END"});
}

TEST(tokenizer_tests, tokenizer_tests_utf8_parsing_multiline_comment_Test) {
  char testData[] = "BEGIN .\n/*This is\na comment*/END";
  std::vector<alb_lang::Token*> tokens;
  alb_lang::Lexer::tokenizeUTF8(testData, sizeof(testData), tokens);
  EXPECT_EQ(tokens.size(), 3);
  ASSERT_GE(tokens.size(), 3);
  EXPECT_EQ(tokens[0]->getTextContents(), std::string{"BEGIN"});
  EXPECT_EQ(tokens[1]->getTextContents(), std::string{"."});
  EXPECT_EQ(tokens[2]->getTextContents(), std::string{"END"});
}

TEST(tokenizer_tests, tokenizer_tests_utf8_parsing_simple_string_Test) {
  char testData[] = "BEGIN .\nvar {testString \"randomText\"}\nEND";
  std::vector<alb_lang::Token*> tokens;
  alb_lang::Lexer::tokenizeUTF8(testData, sizeof(testData), tokens);
  EXPECT_EQ(tokens.size(), 8);
  ASSERT_GE(tokens.size(), 5);
  EXPECT_EQ(tokens[0]->getTextContents(), std::string{"BEGIN"});
  EXPECT_EQ(tokens[1]->getTextContents(), std::string{"."});
  EXPECT_EQ(tokens[2]->getTextContents(), std::string{"var"});
  EXPECT_EQ(tokens[3]->getTextContents(), std::string{"{"});
  EXPECT_EQ(tokens[4]->getTextContents(), std::string{"testString"});
  ASSERT_GE(tokens.size(), 7);
  EXPECT_TRUE(absl::StartsWith(tokens[5]->getTextContents(), "::internal::strings::"));
  EXPECT_EQ(alb_lang::Resources::stringStore().getString(tokens[5]->getTextContents()), std::string{"randomText"});
  EXPECT_EQ(tokens[6]->getTextContents(), std::string{"}"});
  EXPECT_EQ(tokens[7]->getTextContents(), std::string{"END"});
}