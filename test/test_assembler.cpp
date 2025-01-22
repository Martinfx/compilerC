#include <gtest/gtest.h>
#include "../compiler.hpp"

std::string remove_whitespace(const std::string& str) {
    std::string result;
    std::copy_if(str.begin(), str.end(), std::back_inserter(result), [](char c) {
        return !std::isspace(static_cast<unsigned char>(c));
    });
    return result;
}

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

    std::string expectedOutput =
        "        .globl  main\n"
        "        .type   main, @function\n"
        "main:\n"
        "        mov     $0, %eax\n"
        "        ret\n"
        "        .size   main, .-main\n";

    EXPECT_EQ(remove_whitespace(output.str()), remove_whitespace(expectedOutput));
}

TEST(AssemblerTest, GeneratesCorrectAssemblerForSimpleFunction2) {
    std::vector<Token> tokens = {
        {TokenType::KEYWORD, "int"},
        {TokenType::IDENTIFIER, "main"},
        {TokenType::OPENPARENTHESIS, "("},
        {TokenType::KEYWORD, "void"},
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

    std::string expectedOutput =
        "        .globl  main\n"
        "        .type   main, @function\n"
        "main:\n"
        "        mov     $0, %eax\n"
        "        ret\n"
        "        .size   main, .-main\n";

    EXPECT_EQ(remove_whitespace(output.str()), remove_whitespace(expectedOutput));
}

TEST(AssemblerTest, GeneratesCorrectAssemblerForSimpleFunction3) {
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

    std::string expectedOutput =
        "        .globl  main\n"
        "        .type   main, @function\n"
        "main:\n"
        "        mov     $0, %eax\n"
        "        ret\n"
        "        .size   main, .-main\n";

    EXPECT_EQ(remove_whitespace(output.str()), remove_whitespace(expectedOutput));
}


// broken AST

/*
TEST(AssemblerTest, GeneratesCorrectAssemblerForFunctionWithMultipleStatements4) {
    std::vector<Token> tokens = {
        {TokenType::KEYWORD, "int"},
        {TokenType::IDENTIFIER, "main"},
        {TokenType::OPENPARENTHESIS, "("},
        {TokenType::CLOSEPARENTHESIS, ")"},
        {TokenType::OPENBRACE, "{"},
        {TokenType::KEYWORD, "int"},
        {TokenType::IDENTIFIER, "a"},
        {TokenType::EQUAL, "="},
        {TokenType::LITERAL, "5"},
        {TokenType::SEMICOLON, ";"},
        {TokenType::KEYWORD, "int"},
        {TokenType::IDENTIFIER, "b"},
        {TokenType::EQUAL, "="},
        {TokenType::LITERAL, "10"},
        {TokenType::SEMICOLON, ";"},
        {TokenType::KEYWORD, "return"},
        {TokenType::IDENTIFIER, "a"},
        {TokenType::OPERATOR, "+"},
        {TokenType::IDENTIFIER, "b"},
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

    std::string expectedOutput =
        "        .globl  main\n"
        "        .type   main, @function\n"
        "main:\n"
        "        mov     $5, %eax\n"
        "        mov     %eax, [a]\n"
        "        mov     $10, %eax\n"
        "        mov     %eax, [b]\n"
        "        mov     %eax, [a]\n"
        "        add     %eax, [b]\n"
        "        ret\n"
        "        .size   main, .-main\n";

    EXPECT_EQ(remove_whitespace(output.str()), remove_whitespace(expectedOutput));
}*/
/*
TEST(AssemblerTest, GeneratesCorrectAssemblerForConditionalStatement5) {
    std::vector<Token> tokens = {
        {TokenType::KEYWORD, "int"},
        {TokenType::IDENTIFIER, "main"},
        {TokenType::OPENPARENTHESIS, "("},
        {TokenType::CLOSEPARENTHESIS, ")"},
        {TokenType::OPENBRACE, "{"},
        {TokenType::KEYWORD, "int"},
        {TokenType::IDENTIFIER, "a"},
        {TokenType::EQUAL, "="},
        {TokenType::LITERAL, "5"},
        {TokenType::SEMICOLON, ";"},
        {TokenType::KEYWORD, "if"},
        {TokenType::OPENPARENTHESIS, "("},
        {TokenType::IDENTIFIER, "a"},
        {TokenType::OPERATOR, ">"},
        {TokenType::LITERAL, "0"},
        {TokenType::CLOSEPARENTHESIS, ")"},
        {TokenType::OPENBRACE, "{"},
        {TokenType::KEYWORD, "return"},
        {TokenType::LITERAL, "1"},
        {TokenType::SEMICOLON, ";"},
        {TokenType::CLOSEBRACE, "}"},
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

    std::string expectedOutput =
        "        .globl  main\n"
        "        .type   main, @function\n"
        "main:\n"
        "        mov     $5, %eax\n"
        "        mov     %eax, [a]\n"
        "        mov     %eax, [a]\n"
        "        cmp     %eax, 0\n"
        "        jle     else_label\n"
        "        mov     $1, %eax\n"
        "        ret\n"
        "else_label:\n"
        "        mov     $0, %eax\n"
        "        ret\n"
        "        .size   main, .-main\n";

    EXPECT_EQ(remove_whitespace(output.str()), remove_whitespace(expectedOutput));
}*/

