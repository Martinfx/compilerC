#include <gtest/gtest.h>
#include "../compiler.hpp"
#include "../lexer.hpp"

TEST(LexerTest, RecognizesIdentifiers) {
    std::string input = "abc def";
    auto tokens = tokenize(input, tokenTable);

    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].type, TokenType::IDENTIFIER);
    EXPECT_EQ(tokens[0].value, "abc");
    EXPECT_EQ(tokens[1].type, TokenType::IDENTIFIER);
    EXPECT_EQ(tokens[1].value, "def");
}

TEST(LexerTest, RecognizesNumbers) {
    std::string input = "123 456";
    auto tokens = tokenize(input, tokenTable);

    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].type, TokenType::LITERAL);
    EXPECT_EQ(tokens[0].value, "123");
    EXPECT_EQ(tokens[1].type, TokenType::LITERAL);
    EXPECT_EQ(tokens[1].value, "456");
}

TEST(LexerTest, RecognizesOperators) {
    std::string input = "+-*/";
    auto tokens = tokenize(input, tokenTable);

    ASSERT_EQ(tokens.size(), 4);
    EXPECT_EQ(tokens[0].type, TokenType::OPERATOR);
    EXPECT_EQ(tokens[0].value, "+");
    EXPECT_EQ(tokens[1].type, TokenType::OPERATOR);
    EXPECT_EQ(tokens[1].value, "-");
    EXPECT_EQ(tokens[2].type, TokenType::OPERATOR);
    EXPECT_EQ(tokens[2].value, "*");
    EXPECT_EQ(tokens[3].type, TokenType::OPERATOR);
    EXPECT_EQ(tokens[3].value, "/");
}

TEST(LexerTest, RecognizesUnknowns) {
    std::string input = "abc@#";
    auto tokens = tokenize(input, tokenTable);

    ASSERT_EQ(tokens.size(), 3);
    EXPECT_EQ(tokens[0].type, TokenType::IDENTIFIER);
    EXPECT_EQ(tokens[0].value, "abc");
    EXPECT_EQ(tokens[1].type, TokenType::UNKNOWN);
    EXPECT_EQ(tokens[1].value, "@");
    EXPECT_EQ(tokens[2].type, TokenType::UNKNOWN);
    EXPECT_EQ(tokens[2].value, "#");
}

TEST(LexerTest, RecognizesKeywords) {
    std::string input = "int return";
    auto tokens = tokenize(input, tokenTable);

    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].type, TokenType::KEYWORD);
    EXPECT_EQ(tokens[0].value, "int");
    EXPECT_EQ(tokens[1].type, TokenType::KEYWORD);
    EXPECT_EQ(tokens[1].value, "return");
}

TEST(LexerTest, RecognizesIdentifiersWithNumbers) {
    std::string input = "int void";
    auto tokens = tokenize(input, tokenTable);

    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].type, TokenType::KEYWORD);
    EXPECT_EQ(tokens[0].value, "int");
    EXPECT_EQ(tokens[1].type, TokenType::KEYWORD);
    EXPECT_EQ(tokens[1].value, "void");
}

TEST(LexerTest, RecognizesCompoundOperators) {
    std::string input = "== != <= >=";
    auto tokens = tokenize(input, tokenTable);

    ASSERT_EQ(tokens.size(), 8);
    EXPECT_EQ(tokens[0].type, TokenType::OPERATOR);
    EXPECT_EQ(tokens[0].value, "=");
    EXPECT_EQ(tokens[1].type, TokenType::OPERATOR);
    EXPECT_EQ(tokens[1].value, "=");
    EXPECT_EQ(tokens[2].type, TokenType::OPERATOR);
    EXPECT_EQ(tokens[2].value, "!");
    EXPECT_EQ(tokens[3].type, TokenType::OPERATOR);
    EXPECT_EQ(tokens[3].value, "=");
    EXPECT_EQ(tokens[4].type, TokenType::OPERATOR);
    EXPECT_EQ(tokens[4].value, "<");
    EXPECT_EQ(tokens[5].type, TokenType::OPERATOR);
    EXPECT_EQ(tokens[5].value, "=");
    EXPECT_EQ(tokens[6].type, TokenType::OPERATOR);
    EXPECT_EQ(tokens[6].value, ">");
    EXPECT_EQ(tokens[7].type, TokenType::OPERATOR);
    EXPECT_EQ(tokens[7].value, "=");
}

TEST(LexerTest, RecognizesFloatingPointNumbers) {
    std::string input = "3.14 0.001";
    auto tokens = tokenize(input, tokenTable);

    ASSERT_EQ(tokens.size(), 3);
    EXPECT_EQ(tokens[0].type, TokenType::NUMBER);
    EXPECT_EQ(tokens[0].value, "3.14");
    EXPECT_EQ(tokens[1].type, TokenType::WHITESPACE);
    EXPECT_EQ(tokens[1].value, " ");
    EXPECT_EQ(tokens[2].type, TokenType::NUMBER);
    EXPECT_EQ(tokens[2].value, "0.001");
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
