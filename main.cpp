#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <cctype>
#include <fstream>
#include <cassert>

#include "compiler.hpp"


int main(int argc, char* argv[]) {
    /*if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <filename.c>\n";
        return 1;
    }

    std::string filename = argv[1];
    if (filename.substr(filename.find_last_of(".") + 1) != "c") {
        std::cerr << "Error: Input file must have a .c extension\n";
        return 1;
    }*/

    std::vector<std::pair<TokenType, std::regex>> tokenTable = {
        {TokenType::KEYWORD, std::regex("^void\\b|int\\b|return\\b")},
        {TokenType::LITERAL, std::regex("^\\d+")},
        {TokenType::IDENTIFIER, std::regex("^[a-zA-Z_][a-zA-Z0-9_]*")},
        {TokenType::OPERATOR, std::regex("^\\+|\\-|\\*|\\/|\\=")},
        {TokenType::WHITESPACE, std::regex("^\\s+")},
        {TokenType::OPENPARENTHESIS, std::regex("^\\(")},
        {TokenType::CLOSEPARENTHESIS, std::regex("^\\)")},
        {TokenType::OPENBRACE, std::regex("^\\{")},
        {TokenType::CLOSEBRACE, std::regex("^\\}")},
        {TokenType::SEMICOLON, std::regex("^\\;")},
        {TokenType::COMMA, std::regex("^,")}
    };

    //std::string sourceCode = readFile(filename);
    std::string sourceCode = "int main() { int x = 4+4 return x;}";
    sourceCode = removeComments(sourceCode);

    try {
        auto tokens = tokenize(sourceCode, tokenTable);

        for (const auto& token : tokens) {
            std::cout << "Type: " << token.type << ", Value: " << token.value << '\n';
        }

        Parser parser(tokens);
        auto ast = parser.parse();

        // Create a visitor and traverse the AST
        AstPrinter printer;
        ast->accept(&printer);

        std::cout << "Parsing completed successfully.\n";

        // Start assembler output
        std::cout << ".file   \"test.c\"" << std::endl;
        std::cout << ".text" << std::endl;

        AssemblerGenerator generator;
        ast->accept(&generator);

        // End assembler output
        std::cout << ".ident  \"CompilerC: 0.1\"" << std::endl;
        std::cout << ".section        .note.GNU-stack,\"\",@progbits" << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
    }

    return 0;
}
