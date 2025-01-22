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

/*
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
}*/


TEST(LexerTest, RecognizesComments) {
    std::string input = "// This is a comment\nint x = 0;";
    auto tokens = tokenize(input, tokenTable);

    ASSERT_EQ(tokens.size(), 6);
    EXPECT_EQ(tokens[0].type, TokenType::NEWLINE);
    EXPECT_EQ(tokens[0].value, "\n");
    EXPECT_EQ(tokens[1].type, TokenType::KEYWORD);
    EXPECT_EQ(tokens[1].value, "int");
    EXPECT_EQ(tokens[2].type, TokenType::IDENTIFIER);
    EXPECT_EQ(tokens[2].value, "x");
    EXPECT_EQ(tokens[3].type, TokenType::OPERATOR);
    EXPECT_EQ(tokens[3].value, "=");
    EXPECT_EQ(tokens[4].type, TokenType::LITERAL);
    EXPECT_EQ(tokens[4].value, "0");
    EXPECT_EQ(tokens[5].type, TokenType::SEMICOLON);
    EXPECT_EQ(tokens[5].value, ";");
}

TEST(LexerTest, RecognizesComments2) {
    std::string input = "/*"
                        " This is a comment"
                        "*/"
                        "int x = 0;";
    auto tokens = tokenize(input, tokenTable);

    ASSERT_EQ(tokens.size(), 5);
    EXPECT_EQ(tokens[0].type, TokenType::KEYWORD);
    EXPECT_EQ(tokens[0].value, "int");
    EXPECT_EQ(tokens[1].type, TokenType::IDENTIFIER);
    EXPECT_EQ(tokens[1].value, "x");
    EXPECT_EQ(tokens[2].type, TokenType::OPERATOR);
    EXPECT_EQ(tokens[2].value, "=");
    EXPECT_EQ(tokens[3].type, TokenType::LITERAL);
    EXPECT_EQ(tokens[3].value, "0");
    EXPECT_EQ(tokens[4].type, TokenType::SEMICOLON);
    EXPECT_EQ(tokens[4].value, ";");
}

/// Test invalid input
TEST(LexerTest, RecognizesInvalidInput) {
    std::string input = "int main() { int x = 0;";
    try {
        auto tokens = tokenize(input, tokenTable);
        FAIL();  // exception not thrown as expected
    } catch (std::runtime_error& ex) {
        EXPECT_STREQ("Unmatched opening brackets found. Ensure all brackets are closed. Line: 1", ex.what());
    }
}

TEST(LexerTest, RecognizesInvalidInput2) {
    std::string input = "int main()  int x = 0;}";
    try {
        auto tokens = tokenize(input, tokenTable);
        FAIL();  // exception not thrown as expected
    } catch (std::runtime_error& ex) {
        EXPECT_STREQ("Unmatched closing bracket } at line 1", ex.what());
    }
}

TEST(LexerTest, RecognizesInvalidInput3) {
    std::string input = "int main) {  int x = 0;}";
    try {
        auto tokens = tokenize(input, tokenTable);
        FAIL();  // exception not thrown as expected
    } catch (std::runtime_error& ex) {
        EXPECT_STREQ("Unmatched closing bracket ) at line 1", ex.what());
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
