#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <cctype>
#include <fstream>
#include <cassert>

class AstNode;
class Expression;
class Identifier;
class Literal;
class BinaryOperation;
class ExpressionStatement;
class BlockStatement;
class IfStatement;
class WhileStatement;
class ForStatement;
class FunctionDeclaration;
class ReturnStatement;
class VariableDeclaration;

class AstVisitor {
public:
    virtual void visit(Expression* node) = 0;
    virtual void visit(Identifier* node) = 0;
    virtual void visit(Literal* node) = 0;
    virtual void visit(BinaryOperation* node) = 0;
    virtual void visit(ExpressionStatement* node) = 0;
    virtual void visit(BlockStatement* node) = 0;
    virtual void visit(IfStatement* node) = 0;
    virtual void visit(WhileStatement* node) = 0;
    virtual void visit(ForStatement* node) = 0;
    virtual void visit(FunctionDeclaration* node) = 0;
    virtual void visit(ReturnStatement* node) = 0;
    virtual void visit(VariableDeclaration* node) = 0;
};

enum class TokenType
{
    KEYWORD,
    NUMBER,
    IDENTIFIER,
    OPERATOR,
    LITERAL,
    WHITESPACE,
    OPENPARENTHESIS,
    CLOSEPARENTHESIS,
    OPENBRACE,
    CLOSEBRACE,
    SEMICOLON,
    COMMA,
    EQUAL
};


struct Token
{
    TokenType type;
    std::string value;
};

// Enum for different types of nodes in the AST
enum class NodeType {
    Literal,
    Number,
    BinaryExpr,
    UnaryExpr,
    Variable,
    FunctionCall,
    Declaration,
    Statement,
    ExpressionStatement,
    FunctionDef,
    Program,
    Identifier,
    BlockStatement,
    IfStatement,
    WhileStatement,
    ForStatement,
    ReturnStatement,
    VariableDeclaration
};

class AstNode {
public:
    NodeType type;
    virtual ~AstNode() = default;
    virtual void accept(AstVisitor* visitor) = 0;
};

// Example for Expression node
class Expression : public AstNode {
public:
    Expression() {
        this->type = NodeType::Literal;
    }
    virtual ~Expression() {}
    virtual void accept(AstVisitor* visitor) override {
        visitor->visit(this);
    }
};

// Implement accept method for other nodes similarly
class Identifier : public Expression {
public:
    std::string name;
    Identifier(const std::string &name) : name(name) {
        this->type = NodeType::Identifier;
    }
    virtual void accept(AstVisitor* visitor) override {
        visitor->visit(this);
    }
};

class Literal : public Expression
{
public:
    std::string value;

    Literal(const std::string &value) : value(value) {
        this->type = NodeType::Literal;
    }

    const std::string& getValue() const {
        return value;
    }

    virtual void accept(AstVisitor* visitor) override {
        visitor->visit(this);
    }
};

class BinaryOperation : public Expression
{
public:
    Token op;
    std::shared_ptr<AstNode> left;
    std::shared_ptr<AstNode> right;
    BinaryOperation(const Token &op, std::shared_ptr<AstNode> left, std::shared_ptr<AstNode> right) : op(op), left(left), right(right) {
        this->type = NodeType::BinaryExpr;
    }

    virtual void accept(AstVisitor* visitor) override {
        visitor->visit(this);
    }
};

// Statements
class Statement : public AstNode
{
public:
    virtual void accept(AstVisitor* visitor) = 0;
};

class ExpressionStatement : public Statement
{
public:
    std::shared_ptr<AstNode> expression;
    ExpressionStatement(std::shared_ptr<AstNode> expression) : expression(expression) {
        this->type = NodeType::ExpressionStatement;
    }

    virtual ~ExpressionStatement() {}

    virtual void accept(AstVisitor* visitor) override {
        visitor->visit(this);
    }
};

class BlockStatement : public Statement
{
public:
    std::vector<std::shared_ptr<AstNode>> statements;

    BlockStatement() {
        this->type = NodeType::BlockStatement;
    }

    virtual void accept(AstVisitor* visitor) override {
        visitor->visit(this);
    }
};

class IfStatement : public Statement
{
public:
    std::shared_ptr<AstNode> condition;
    std::shared_ptr<AstNode> thenStatement;
    std::shared_ptr<AstNode> elseStatement;
    IfStatement(std::shared_ptr<AstNode> condition, std::shared_ptr<AstNode> thenStatement, std::shared_ptr<AstNode> elseStatement)
        : condition(condition), thenStatement(thenStatement), elseStatement(elseStatement) {
        this->type = NodeType::IfStatement;
    }

    virtual void accept(AstVisitor* visitor) override {
        visitor->visit(this);
    }
};

class WhileStatement : public Statement
{
public:
    std::shared_ptr<AstNode> condition;
    std::shared_ptr<AstNode> body;
    WhileStatement(std::shared_ptr<AstNode> condition, std::shared_ptr<AstNode> body) : condition(condition), body(body) {
        this->type = NodeType::WhileStatement;
    }

    virtual void accept(AstVisitor* visitor) override {
        visitor->visit(this);
    }
};

class ForStatement : public Statement
{
public:
    std::shared_ptr<AstNode> init;
    std::shared_ptr<AstNode> condition;
    std::shared_ptr<AstNode> update;
    std::shared_ptr<AstNode> body;
    ForStatement(std::shared_ptr<AstNode> init, std::shared_ptr<AstNode> condition, std::shared_ptr<AstNode> update, std::shared_ptr<AstNode> body)
        : init(init), condition(condition), update(update), body(body) {
        this->type = NodeType::ForStatement;
    }

    virtual void accept(AstVisitor* visitor) override {
        visitor->visit(this);
    }
};

class FunctionDeclaration : public Statement
{
public:
    std::string name;
    std::vector<std::string> params;
    std::shared_ptr<AstNode> body;
    FunctionDeclaration(const std::string &name, const std::vector<std::string> &params, std::shared_ptr<AstNode> body)
        : name(name), params(params), body(body) {
        this->type = NodeType::FunctionDef;
    }

    virtual void accept(AstVisitor* visitor) override {
        visitor->visit(this);
    }
};

class ReturnStatement : public Statement
{
public:
    std::shared_ptr<AstNode> expression;
    ReturnStatement(std::shared_ptr<AstNode> expression) : expression(expression) {
        this->type = NodeType::ReturnStatement;
    }

    virtual void accept(AstVisitor* visitor) override {
        visitor->visit(this);
    }
};

class VariableDeclaration : public Statement
{
public:
    std::string name;
    std::shared_ptr<AstNode> init;
    VariableDeclaration(const std::string &name, std::shared_ptr<AstNode> init) : name(name), init(init) {
        this->type = NodeType::VariableDeclaration;
    }

    virtual void accept(AstVisitor* visitor) override {
        visitor->visit(this);
    }
};

class Parser
{
public:
    Parser(const std::vector<Token> &tokens) : tokens(tokens), position(0) {}

    std::shared_ptr<AstNode> parse()
    {
        std::vector<std::shared_ptr<AstNode>> statements;
        while (!eof())
        {
            statements.push_back(parseStatement());
        }
        auto block = std::make_shared<BlockStatement>();
        block->statements = statements;
        return block;
    }

private:
    std::vector<Token> tokens;
    size_t position;

    bool eof() const
    {
        return position >= tokens.size();
    }

    const Token &peek() const
    {
        if (eof()) {
            throw std::runtime_error("Unexpected end of input");
        }
        return tokens.at(position);
    }

    const Token &next()
    {
        if (eof()) {
            throw std::runtime_error("Unexpected end of input");
        }
        return tokens.at(++position);
    }

    void advance()
    {
        if (!eof()) {
            position++;
        }
    }

    bool match(TokenType type)
    {
        if (!eof() && peek().type == type)
        {
            advance();
            return true;
        }
        return false;
    }

    std::shared_ptr<AstNode> parseStatement() {
        std::shared_ptr<AstNode> statement;

        if (peek().type == TokenType::KEYWORD && peek().value == "int") {
            advance();

            if (peek().type == TokenType::IDENTIFIER) {

                if (tokens[position + 1].type == TokenType::OPENPARENTHESIS) {
                    return parseFunctionDeclaration(); // Funkce
                } else {
                    return parseVariableDeclaration(); // Proměnná
                }
            } else {
                throw std::runtime_error("Expected identifier after 'int'");
            }
        }

        // next work with "return, if, ..."
        else if (peek().type == TokenType::KEYWORD && peek().value == "return") {
            advance();
            auto expr = parseExpression();
            if (!match(TokenType::SEMICOLON)) {
                throw std::runtime_error("Expected ';' after return statement");
            }
            return std::make_shared<ReturnStatement>(expr);
        } else {
            statement = parseExpressionStatement();
        }

        return statement;
    }

    std::shared_ptr<AstNode> parseExpression()
    {
        std::shared_ptr<AstNode> left = parsePrimary();

        while (peek().type == TokenType::OPERATOR)
        {
            Token op = peek();
            advance();
            std::shared_ptr<AstNode> right = parsePrimary();
            left = std::make_shared<BinaryOperation>(op, left, right);
        }

        return left;
    }

    std::shared_ptr<AstNode> parsePrimary()
    {
        if (peek().type == TokenType::IDENTIFIER)
        {
            return parseIdentifier();
        }
        else if (peek().type == TokenType::LITERAL)
        {
            return parseLiteral();
        }
        else if (match(TokenType::OPENPARENTHESIS) && peek().value == "(")
        {
            std::shared_ptr<AstNode> expr = parseExpression();
            if (!match(TokenType::CLOSEPARENTHESIS) || peek().value != ")")
            {
                // Error: unmatched parenthesis
                std::cerr << "Error: unmatched parenthesis" << std::endl;
                exit(1);
            }
            return expr;
        }
        // Error: unexpected token
        std::cerr << "Error: unexpected token in ast " << peek().value << std::endl;
        exit(1);
    }

    std::shared_ptr<AstNode> parseIdentifier()
    {
        auto identifier = std::make_shared<Identifier>(peek().value);
        advance();
        return identifier;
    }

    std::shared_ptr<AstNode> parseLiteral() {
        if (peek().type != TokenType::LITERAL) {
            throw std::runtime_error("Expected a literal token");
        }

        auto literal = std::make_shared<Literal>(peek().value);
        assert(literal);
        advance();
        return literal;
    }

    std::shared_ptr<AstNode> parseExpressionStatement()
    {
        std::shared_ptr<AstNode> expr = parseExpression();
        if (match(TokenType::SEMICOLON) && peek().value == ";")
        {
            advance();
        }
        return std::make_shared<ExpressionStatement>(expr);
    }

    std::shared_ptr<AstNode> parseIfStatement()
    {
        if (!match(TokenType::OPENPARENTHESIS) || peek().value != "(")
        {
            // Error: expected '(' after 'if'
            std::cerr << "Error: expected '(' after 'if'" << std::endl;
            exit(1);
        }
        advance();
        std::shared_ptr<AstNode> condition = parseExpression();
        if (!match(TokenType::CLOSEPARENTHESIS) || peek().value != ")")
        {
            // Error: expected ')' after if condition
            std::cerr << "Error: expected ')' after if condition" << std::endl;
            exit(1);
        }
        advance();
        std::shared_ptr<AstNode> thenStatement = parseStatement();
        std::shared_ptr<AstNode> elseStatement;
        if (match(TokenType::KEYWORD) && peek().value == "else")
        {
            advance();
            elseStatement = parseStatement();
        }
        return std::make_shared<IfStatement>(condition, thenStatement, elseStatement);
    }

    std::shared_ptr<AstNode> parseWhileStatement()
    {
        if (!match(TokenType::OPENPARENTHESIS) || peek().value != "(")
        {
            // Error: expected '(' after 'while'
            std::cerr << "Error: expected '(' after 'while'" << std::endl;
            exit(1);
        }
        advance();
        std::shared_ptr<AstNode> condition = parseExpression();
        if (!match(TokenType::CLOSEPARENTHESIS) || peek().value != ")")
        {
            // Error: expected ')' after while condition
            std::cerr << "Error: expected ')' after while condition" << std::endl;
            exit(1);
        }
        advance();
        std::shared_ptr<AstNode> body = parseStatement();
        return std::make_shared<WhileStatement>(condition, body);
    }

    std::shared_ptr<AstNode> parseForStatement()
    {
        if (!match(TokenType::OPENPARENTHESIS) || peek().value != "(")

        {
            // Error: expected '(' after 'for'
            std::cerr << "Error: expected '(' after 'for'" << std::endl;
            exit(1);
        }
        advance();
        std::shared_ptr<AstNode> init = parseStatement();
        std::shared_ptr<AstNode> condition = parseExpression();
        if (!match(TokenType::SEMICOLON) || peek().value != ";")
        {
            // Error: expected ';' after for condition
            std::cerr << "Error: expected ';' after for condition" << std::endl;
            exit(1);
        }
        advance();
        std::shared_ptr<AstNode> update = parseExpression();
        if (!match(TokenType::CLOSEPARENTHESIS) || peek().value != ")")
        {
            // Error: expected ')' after for update
            std::cerr << "Error: expected ')' after for update" << std::endl;
            exit(1);
        }
        advance();
        std::shared_ptr<AstNode> body = parseStatement();
        return std::make_shared<ForStatement>(init, condition, update, body);
    }

    std::shared_ptr<AstNode> parseFunctionDeclaration() {

        if (peek().type != TokenType::IDENTIFIER) {
            throw std::runtime_error("Expected function name after 'int'");
        }

        auto name = parseIdentifier(); // name fuction


        if (!match(TokenType::OPENPARENTHESIS)) {
            throw std::runtime_error("Expected '(' after function name");
        }


        std::vector<std::string> params;
        while (peek().type == TokenType::IDENTIFIER || peek().type == TokenType::KEYWORD && peek().value == "void") {
            if (peek().value != "void") {
                params.push_back(peek().value);
            }
            advance();
            if (!match(TokenType::COMMA)) {
                break;
            }
        }

        if (!match(TokenType::CLOSEPARENTHESIS)) {
            throw std::runtime_error("Expected ')' after parameters");
        }


        if (!match(TokenType::OPENBRACE)) {
            throw std::runtime_error("Expected '{' for function body");
        }

        auto body = std::make_shared<BlockStatement>();
        while (!match(TokenType::CLOSEBRACE)) {
            body->statements.push_back(parseStatement());
        }

        return std::make_shared<FunctionDeclaration>(
            static_cast<Identifier*>(name.get())->name, params, body);
    }

    std::shared_ptr<AstNode> parseReturnStatement()
    {
        std::shared_ptr<AstNode> expression = parseExpression();
        if (match(TokenType::SEMICOLON) && peek().value == ";")
        {
            advance();
        }
        return std::make_shared<ReturnStatement>(expression);
    }

    std::shared_ptr<AstNode> parseVariableDeclaration()
    {
        std::shared_ptr<AstNode> name = parseIdentifier();
        std::shared_ptr<AstNode> init;
        if (match(TokenType::OPERATOR) && peek().value == "=")
        {
            advance();
            init = parseExpression();
        }
        if (match(TokenType::SEMICOLON) && peek().value == ";")
        {
            advance();
        }
        return std::make_shared<VariableDeclaration>(static_cast<Identifier *>(name.get())->name, init);
    }
};

///////////////////////////////////////////////
/// Lexer
///////////////////////////////////////////////

void trimWhitespace(std::string& input) {
    input.erase(0, input.find_first_not_of(" \t\n\r"));
}

void trimAt(std::string& input) {
    input.erase(0, input.find_first_not_of("@"));
}

std::vector<Token> tokenize(const std::string& input, const std::vector<std::pair<TokenType, std::regex>>& tokenTable) {
    std::string remainingInput = input;
    std::vector<Token> tokens;

    while (!remainingInput.empty()) {
        trimWhitespace(remainingInput);
        trimAt(remainingInput);

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

std::string removeComments(const std::string& input) {
    static const std::regex singleLineComment("//.*");
    static const std::regex multiLineComment("/\\*.*?\\*/");

    std::string result = std::regex_replace(input, multiLineComment, "");
    result = std::regex_replace(result, singleLineComment, "");
    return result;
}

std::string readFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        throw std::runtime_error("Could not open file: " + filename);
    }

    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    return content;
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
class AssemblerGenerator : public AstVisitor {
public:
    void visit(Expression* node) override {
        std::cerr << "Error: Encountered generic Expression node. This should be handled by a derived class." << std::endl;
        exit(1); // Terminate the program with an error
    }

    void visit(Identifier* node) override {
        std::cout << "        mov     %eax, [" << node->name << "]" << std::endl;
    }

    void visit(Literal* node) override {
        std::cout << "        mov     $" << node->value << ", %eax" << std::endl;
    }

    void visit(BinaryOperation* node) override {
        node->left->accept(this);
        std::cout << "        push    %rax" << std::endl;
        node->right->accept(this);
        std::cout << "        pop     %rbx" << std::endl;
        if (node->op.value == "+") {
            std::cout << "        add     %eax, %ebx" << std::endl;
        } else if (node->op.value == "-") {
            std::cout << "        sub     %eax, %ebx" << std::endl;
        } else if (node->op.value == "*") {
            std::cout << "        imul    %eax, %ebx" << std::endl;
        } else if (node->op.value == "/") {
            std::cout << "        cdq" << std::endl; // Sign extend EAX into EDX:EAX
            std::cout << "        idiv    %ebx" << std::endl;
        }
    }

    void visit(ExpressionStatement* node) override {
        node->expression->accept(this);
    }

    void visit(BlockStatement* node) override {
        for (const auto& stmt : node->statements) {
            stmt->accept(this);
        }
    }

    void visit(IfStatement* node) override {
        node->condition->accept(this);
        std::cout << "        cmp     %eax, 0" << std::endl;
        std::cout << "        je      else_label" << std::endl;
        node->thenStatement->accept(this);
        std::cout << "        jmp     endif_label" << std::endl;
        std::cout << "else_label:" << std::endl;
        if (node->elseStatement) {
            node->elseStatement->accept(this);
        }
        std::cout << "endif_label:" << std::endl;
    }

    void visit(WhileStatement* node) override {
        std::cout << "while_label:" << std::endl;
        node->condition->accept(this);
        std::cout << "        cmp     %eax, 0" << std::endl;
        std::cout << "        je      endwhile_label" << std::endl;
        node->body->accept(this);
        std::cout << "        jmp     while_label" << std::endl;
        std::cout << "endwhile_label:" << std::endl;
    }

    void visit(ForStatement* node) override {
        node->init->accept(this);
        std::cout << "for_label:" << std::endl;
        node->condition->accept(this);
        std::cout << "        cmp     %eax, 0" << std::endl;
        std::cout << "        je      endfor_label" << std::endl;
        node->body->accept(this);
        node->update->accept(this);
        std::cout << "        jmp     for_label" << std::endl;
        std::cout << "endfor_label:" << std::endl;
    }

    void visit(FunctionDeclaration* node) override {
        std::cout << "        .globl  " << node->name << std::endl;
        std::cout << "        .type   " << node->name << ", @function" << std::endl;
        std::cout << node->name << ":" << std::endl;
        node->body->accept(this);
        std::cout << "        ret" << std::endl;
        std::cout << "        .size   " << node->name << ", .-" << node->name << std::endl;
    }

    void visit(ReturnStatement* node) override {
        node->expression->accept(this);
    }

    void visit(VariableDeclaration* node) override {
        if (node->init) {
            node->init->accept(this);
            std::cout << "        mov     [" << node->name << "], %eax" << std::endl;
        }
    }
};


class AstPrinter : public AstVisitor {
public:
    void visit(Expression* node) override {
        std::cout << "Visiting Expression node\n";
    }

    void visit(Identifier* node) override {
        std::cout << "Visiting Identifier node: " << node->name << "\n";
    }

    void visit(Literal* node) override {
        std::cout << "Visiting Literal node: " << node->getValue() << "\n";
    }

    void visit(BinaryOperation* node) override {
        std::cout << "Visiting BinaryOperation node with operator: " << node->op.value << "\n";
        node->left->accept(this);
        node->right->accept(this);
    }

    void visit(ExpressionStatement* node) override {
        std::cout << "Visiting ExpressionStatement node\n";
        node->expression->accept(this);
    }

    void visit(BlockStatement* node) override {
        std::cout << "Visiting BlockStatement node\n";
        for (const auto& stmt : node->statements) {
            stmt->accept(this);
        }
    }

    void visit(IfStatement* node) override {
        std::cout << "Visiting IfStatement node\n";
        node->condition->accept(this);
        node->thenStatement->accept(this);
        if (node->elseStatement) {
            node->elseStatement->accept(this);
        }
    }

    void visit(WhileStatement* node) override {
        std::cout << "Visiting WhileStatement node\n";
        node->condition->accept(this);
        node->body->accept(this);
    }

    void visit(ForStatement* node) override {
        std::cout << "Visiting ForStatement node\n";
        node->init->accept(this);
        node->condition->accept(this);
        node->update->accept(this);
        node->body->accept(this);
    }

    void visit(FunctionDeclaration* node) override {
        std::cout << "Visiting FunctionDeclaration node: " << node->name << "\n";
        for (const auto& param : node->params) {
            std::cout << "Param: " << param << "\n";
        }
        node->body->accept(this);
    }

    void visit(ReturnStatement* node) override {
        std::cout << "Visiting ReturnStatement node\n";
        node->expression->accept(this);
    }

    void visit(VariableDeclaration* node) override {
        std::cout << "Visiting VariableDeclaration node: " << node->name << "\n";
        if (node->init) {
            node->init->accept(this);
        }
    }
};
