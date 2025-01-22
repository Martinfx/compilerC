#include <iostream>
#include <string>
#include <cctype>

#include "compiler.hpp"
#include "lexer.hpp"

std::string readFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        throw std::runtime_error("Could not open file: " + filename);
    }

    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    return content;
}

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

    //std::string sourceCode = readFile(filename);
    std::string sourceCode = "int main() { int x = 42; return 0; } ";
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
