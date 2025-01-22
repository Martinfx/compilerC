#pragma once
#include <vector>
#include <regex>
#include <fstream>
#include <cassert>

#include "compiler.hpp"

std::vector<std::pair<TokenType, std::regex>> tokenTable = {
    {TokenType::KEYWORD, std::regex("^void\\b|int\\b|return\\b")},
    {TokenType::LITERAL, std::regex("^\\d+")},
    {TokenType::IDENTIFIER, std::regex("^[a-zA-Z_][a-zA-Z0-9_]*")},
    {TokenType::OPERATOR, std::regex("^\\+|\\-|\\*|\\/|\\=|\\<|\\>|\\!")},
    {TokenType::WHITESPACE, std::regex("^\\s+")},
    {TokenType::OPENPARENTHESIS, std::regex("^\\(")},
    {TokenType::CLOSEPARENTHESIS, std::regex("^\\)")},
    {TokenType::OPENBRACE, std::regex("^\\{")},
    {TokenType::CLOSEBRACE, std::regex("^\\}")},
    {TokenType::SEMICOLON, std::regex("^\\;")},
    {TokenType::COMMA, std::regex("^,")},
    {TokenType::UNKNOWN, std::regex("^\\@|\\#")}
};

void trimWhitespace(std::string& input) {
    input.erase(0, input.find_first_not_of(" \t\n\r"));
}

void trimAt(std::string& input) {
    input.erase(0, input.find_first_not_of("@"));
}

std::string removeComments(const std::string& input) {
    static const std::regex singleLineComment("//.*");
    static const std::regex multiLineComment("/\\*.*?\\*/");

    std::string result = std::regex_replace(input, multiLineComment, "");
    result = std::regex_replace(result, singleLineComment, "");
    return result;
}

std::vector<Token> tokenize(const std::string& input, const std::vector<std::pair<TokenType, std::regex>>& tokenTable) {
    std::string remainingInput = input;
    std::vector<Token> tokens;

    while (!remainingInput.empty()) {
        trimWhitespace(remainingInput);
        //trimAt(remainingInput);

        bool matched = false;
        for (const auto& [type, regex] : tokenTable) {
            std::smatch match;
            if (std::regex_search(remainingInput, match, regex) && match.position() == 0) {
                tokens.push_back({type, match.str()});
                remainingInput = remainingInput.substr(match.length());
                matched = true;
                break;
            }
        }

        if (!matched) {
            throw std::runtime_error("Unrecognized token at: " + remainingInput);
        }
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
    default: os << "Unknown"; break;
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
    default: os << "UNKNOWN"; break;
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const Token& token) {
    os << "{Type: " << token.type << ", Value: " << token.value << "}";
    return os;
}
