#include <gtest/gtest.h>
#include "../compiler.hpp"

TEST(AssemblerTest, GeneratesCorrectAssemblerForSimpleFunction) {
    std::vector<Token> tokens = {
        {TokenType::KEYWORD, "int"},
        {TokenType::IDENTIFIER, "main"},
        {TokenType::OPENPARENTHESIS, "("},
        {TokenType::CLOSEPARENTHESIS, ")"},
        {TokenType::OPENBRACE, "{"},
        {TokenType::KEYWORD, "return"},
        {TokenType::LITERAL, "0"},
        {TokenType::SEMICOLON, ";"},
        {TokenType::CLOSEBRACE, "}"}
    };

    Parser parser(tokens);
    auto ast = parser.parse();
    AssemblerGenerator generator;
    std::ostringstream output;
    std::streambuf* oldCoutBuffer = std::cout.rdbuf(output.rdbuf());

    ast->accept(&generator);

    std::cout.rdbuf(oldCoutBuffer);

    std::string expectedOutput = "main:\n"
                                 "        mov     eax, 0\n"
                                 "        ret\n";

    EXPECT_EQ(output.str(), expectedOutput);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}