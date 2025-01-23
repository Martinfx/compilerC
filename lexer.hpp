#pragma once
#include <vector>
#include <regex>
#include <fstream>
#include <cassert>

#include "compiler.hpp"

std::vector<std::pair<TokenType, std::regex>> tokenTable = {
    {TokenType::KEYWORD, std::regex("^void\\b|int\\b|return\\b|if\\b|else\\b|while\\b|break\\b|continue\\b")},
    {TokenType::LITERAL, std::regex("^\\d+")},
    {TokenType::IDENTIFIER, std::regex("^[a-zA-Z_][a-zA-Z0-9_]*")},
    {TokenType::OPERATOR, std::regex("^\\+|\\-|\\*|\\/|\\%|\\=|\\==|\\<|\\>|\\!|\\~|\\&|\\^|\\<<|\\>>")},
    {TokenType::WHITESPACE, std::regex("^\\s+|\\s+$")},
    {TokenType::OPENPARENTHESIS, std::regex("^\\(")},
    {TokenType::CLOSEPARENTHESIS, std::regex("^\\)")},
    {TokenType::OPENBRACE, std::regex("^\\{")},
    {TokenType::CLOSEBRACE, std::regex("^\\}")},
    {TokenType::SEMICOLON, std::regex("^\\;")},
    {TokenType::COMMA, std::regex("^,")},
    {TokenType::UNKNOWN, std::regex("^\\@|\\#")},
    {TokenType::SIGNGLECOMMENT, std::regex("^//[^\n]*")},
    {TokenType::NEWLINE, std::regex("^\n")}
};

void trimWhitespace(std::string& input) {
    input.erase(0, input.find_first_not_of(" \t\r\v"));
}

std::vector<Token> tokenize(const std::string& input, const std::vector<std::pair<TokenType, std::regex>>& tokenTable) {
    std::string remainingInput = input;
    std::vector<Token> tokens;
    std::vector<char> bracketStack;
    int lineCount = 1;

    while (!remainingInput.empty()) {
        trimWhitespace(remainingInput);

        if (!remainingInput.empty() && remainingInput[0] == '\n') {
            lineCount++;
            remainingInput = remainingInput.substr(1);
            tokens.push_back({TokenType::NEWLINE, "\n"});
            continue;
        }

        if (!remainingInput.empty() && remainingInput[0] == '/' && remainingInput[1] == '/') {
            size_t commentEnd = remainingInput.find('\n');
            if (commentEnd != std::string::npos) {
                remainingInput = remainingInput.substr(commentEnd);
                lineCount++;
            } else {
                remainingInput.clear();
            }
            continue;
        }

        if (!remainingInput.empty() && remainingInput[0] == '/' && remainingInput[1] == '*') {
            size_t commentEnd = remainingInput.find("*/");
            if (commentEnd != std::string::npos) {
                remainingInput = remainingInput.substr(commentEnd + 2);
            } else {
                std::cerr << "Error: Unterminated comment at line " << lineCount << std::endl;
                break;
            }
            continue;
        }

        //bool matched = false;
        for (const auto& [type, regex] : tokenTable) {
            std::smatch match;
            if (std::regex_search(remainingInput, match, regex) && match.position() == 0) {
                tokens.push_back({type, match.str()});

                char currentChar = match.str()[0];
                if (currentChar == '(' || currentChar == '{' || currentChar == '[') {
                    bracketStack.push_back(currentChar);
                } else if (currentChar == ')' || currentChar == '}' || currentChar == ']') {
                    if (bracketStack.empty()) {
                        std::string str(1, currentChar);
                        throw std::runtime_error("Unmatched closing bracket "  + str +
                                                                                       + " at line " +  std::to_string(lineCount));
                        break;
                    }
                    char lastBracket = bracketStack.back();
                    if ((currentChar == ')' && lastBracket != '(') ||
                        (currentChar == '}' && lastBracket != '{') ||
                        (currentChar == ']' && lastBracket != '[')) {
                        throw std::runtime_error("Mismatched brackets at line " + std::to_string(lineCount));
                        break;
                    }
                    bracketStack.pop_back();
                }

                remainingInput = remainingInput.substr(match.length());
                //matched = true;
                break;
            }
        }

        /*if (!matched) {
            std::cerr << "Warning: Unrecognized token at line " << lineCount << ": " << remainingInput << std::endl;
            break;
        }*/
    }

    if (!bracketStack.empty()) {
        throw std::runtime_error("Unmatched opening brackets found. Ensure all brackets are closed. Line: " + std::to_string(lineCount));
    }

    return tokens;
}

std::ostream& operator<<(std::ostream& os, const NodeType& type) {
    switch (type) {
    case NodeType::Literal: os << "Literal"; break;
    case NodeType::Number: os << "Number"; break;
    case NodeType::BinaryExpr: os << "BinaryExpr"; break;
    case NodeType::UnaryExpr: os << "UnaryExpr"; break;
    case NodeType::Variable: os << "Variable"; break;
    case NodeType::FunctionCall: os << "FunctionCall"; break;
    case NodeType::Declaration: os << "Declaration"; break;
    case NodeType::Statement: os << "Statement"; break;
    case NodeType::ExpressionStatement: os << "ExpressionStatement"; break;
    case NodeType::FunctionDef: os << "FunctionDef"; break;
    case NodeType::Program: os << "Program"; break;
    case NodeType::Identifier: os << "Identifier"; break;
    default: os << "Unknown case"; break;
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const TokenType& type) {
    switch (type) {
    case TokenType::IDENTIFIER: os << "IDENTIFIER"; break;
    case TokenType::KEYWORD: os << "KEYWORD"; break;
    case TokenType::LITERAL: os << "LITERAL"; break;
    case TokenType::OPERATOR: os << "OPERATOR"; break;
    case TokenType::OPENBRACE: os << "OPENBRACE"; break;
    case TokenType::CLOSEBRACE: os << "CLOSEBRACE"; break;
    case TokenType::OPENPARENTHESIS: os << "OPENPARENTHESIS"; break;
    case TokenType::CLOSEPARENTHESIS: os << "CLOSEPARENTHESIS"; break;
    case TokenType::COMMA: os << "COMMA"; break;
    case TokenType::SEMICOLON: os << "SEMICOLON"; break;
    case TokenType::EQUAL: os << "EQUAL"; break;
    case TokenType::SIGNGLECOMMENT: os << "SINGLECOMMENT"; break;
    case TokenType::NEWLINE: os << "NEWLINE"; break;
    default: os << "UNKNOWN case"; break;
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const Token& token) {
    os << "{Type: " << token.type << ", Value: " << token.value << "}";
    return os;
}
