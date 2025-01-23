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
class UnaryOperation;
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
    virtual void visit(UnaryOperation* node) = 0;
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
    EQUAL,
    UNKNOWN,
    SIGNGLECOMMENT,
    NEWLINE
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

class UnaryOperation : public Expression {
public:
    Token op;
    std::shared_ptr<AstNode> operand;

    UnaryOperation(const Token& op, std::shared_ptr<AstNode> operand)
        : op(op), operand(operand) {
        this->type = NodeType::UnaryExpr;
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

    bool match(TokenType type) {
        if (!eof() && peek().type == type) {
            //std::cout << "Matched token: " << peek().value << " at position " << position << std::endl;
            advance();
            return true;
        }
        //std::cout << "Token did not match: " << peek().value << " at position " << position << std::endl;
        return false;
    }


    std::shared_ptr<AstNode> parseStatement() {
        std::shared_ptr<AstNode> statement;

        if (peek().type == TokenType::KEYWORD && peek().value == "int") {
            advance();

            if (peek().type == TokenType::IDENTIFIER) {

                if (tokens[position + 1].type == TokenType::OPENPARENTHESIS) {
                    return parseFunctionDeclaration();
                } else {
                    return parseVariableDeclaration();
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

    std::shared_ptr<AstNode> parsePrimary() {
        if (peek().type == TokenType::OPERATOR && (peek().value == "-" || peek().value == "~" || peek().value == "!")) {
            Token op = peek();
            advance();
            std::shared_ptr<AstNode> operand = parsePrimary();
            return std::make_shared<UnaryOperation>(op, operand);
        }

        if (peek().type == TokenType::IDENTIFIER) {
            return parseIdentifier();
        } else if (peek().type == TokenType::LITERAL) {
            return parseLiteral();
        } else if(peek().type == TokenType::KEYWORD && (peek().value == "if")) {
            return parseIfStatement();
        } else if (match(TokenType::OPENPARENTHESIS)) {
                auto expr = parseExpression();
                if (!match(TokenType::CLOSEPARENTHESIS)) {
                    std::cerr << "Error: unmatched parenthesis at position " << position << std::endl;
                    exit(1);
                }
                return expr;
        }

        std::cerr << "Error: unexpected token in ast: " << peek().value << std::endl;
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

    std::shared_ptr<AstNode> parseIfStatement() {
        if (!match(TokenType::KEYWORD) || peek().value != "if") {
            throw std::runtime_error("Expected 'if' keyword");
        }
        advance();

        if (!match(TokenType::OPENPARENTHESIS)) {
            throw std::runtime_error("Expected '(' after 'if'");
        }

        auto condition = parseExpression();

        if (!match(TokenType::CLOSEPARENTHESIS)) {
            throw std::runtime_error("Expected ')' after if condition");
        }

        auto thenStatement = parseStatement();

        std::shared_ptr<AstNode> elseStatement = nullptr;
        if (peek().type == TokenType::KEYWORD && peek().value == "else") {
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

        auto name = parseIdentifier();


        if (!match(TokenType::OPENPARENTHESIS)) {
            throw std::runtime_error("Expected '(' after function name");
        }


        std::vector<std::string> params;
        while ((peek().type == TokenType::IDENTIFIER || peek().type == TokenType::KEYWORD) && (peek().value == "void")) {
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

    void visit(UnaryOperation* node) override {
        node->operand->accept(this);

        if (node->op.value == "-") {
            std::cout << "        neg     %eax" << std::endl;
        } else if (node->op.value == "~") {
            std::cout << "        not     %eax" << std::endl;
        } else if (node->op.value == "!") {
            std::cout << "        cmp     %eax, 0" << std::endl;
            std::cout << "        mov     $0, %eax" << std::endl;
            std::cout << "        sete    %al" << std::endl;
        } else {
            std::cerr << "Error: Unsupported unary operator: " <<  node->op.value << std::endl;
            exit(1);
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
        std::cout << ".file   \"test.c\"" << std::endl;
        std::cout << ".text" << std::endl;
        std::cout << "        .globl  " << node->name << std::endl;
        std::cout << "        .type   " << node->name << ", @function" << std::endl;
        std::cout << node->name << ":" << std::endl;
        node->body->accept(this);
        std::cout << "        ret" << std::endl;
        std::cout << "        .size   " << node->name << ", .-" << node->name << std::endl;
        std::cout << ".ident  \"CompilerC: 0.1\"" << std::endl;
        std::cout << ".section        .note.GNU-stack,\"\",@progbits" << std::endl;
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
    AstPrinter() : indentLevel(0) {}

    void visit(Expression* node) override {
        printIndent();
        std::cout << "Expression node\n";
    }

    void visit(Identifier* node) override {
        printIndent();
        std::cout << "Identifier node: " << node->name << "\n";
    }

    void visit(Literal* node) override {
        printIndent();
        std::cout << "Literal node: " << node->getValue() << "\n";
    }

    void visit(BinaryOperation* node) override {
        printIndent();
        std::cout << "BinaryOperation node with operator: " << node->op.value << "\n";
        indentLevel++;
        node->left->accept(this);
        node->right->accept(this);
        indentLevel--;
    }

    void visit(UnaryOperation* node) override {
        printIndent();
        std::cout << "Unary node with operator: " << node->op.value << "\n";
        indentLevel++;
        node->operand->accept(this);
        indentLevel--;
    }

    void visit(ExpressionStatement* node) override {
        printIndent();
        std::cout << "ExpressionStatement node\n";
        indentLevel++;
        node->expression->accept(this);
        indentLevel--;
    }

    void visit(BlockStatement* node) override {
        printIndent();
        std::cout << "BlockStatement node\n";
        indentLevel++;
        for (const auto& stmt : node->statements) {
            stmt->accept(this);
        }
        indentLevel--;
    }

    void visit(IfStatement* node) override {
        printIndent();
        std::cout << "IfStatement node\n";
        indentLevel++;
        printIndent();
        std::cout << "Condition:\n";
        indentLevel++;
        node->condition->accept(this);
        indentLevel--;
        printIndent();
        std::cout << "Then:\n";
        indentLevel++;
        node->thenStatement->accept(this);
        indentLevel--;
        if (node->elseStatement) {
            printIndent();
            std::cout << "Else:\n";
            indentLevel++;
            node->elseStatement->accept(this);
            indentLevel--;
        }
        indentLevel--;
    }

    void visit(WhileStatement* node) override {
        printIndent();
        std::cout << "WhileStatement node\n";
        indentLevel++;
        printIndent();
        std::cout << "Condition:\n";
        indentLevel++;
        node->condition->accept(this);
        indentLevel--;
        printIndent();
        std::cout << "Body:\n";
        indentLevel++;
        node->body->accept(this);
        indentLevel--;
        indentLevel--;
    }

    void visit(ForStatement* node) override {
        printIndent();
        std::cout << "ForStatement node\n";
        indentLevel++;
        printIndent();
        std::cout << "Initialization:\n";
        indentLevel++;
        node->init->accept(this);
        indentLevel--;
        printIndent();
        std::cout << "Condition:\n";
        indentLevel++;
        node->condition->accept(this);
        indentLevel--;
        printIndent();
        std::cout << "Update:\n";
        indentLevel++;
        node->update->accept(this);
        indentLevel--;
        printIndent();
        std::cout << "Body:\n";
        indentLevel++;
        node->body->accept(this);
        indentLevel--;
        indentLevel--;
    }

    void visit(FunctionDeclaration* node) override {
        printIndent();
        std::cout << "FunctionDeclaration node: " << node->name << "\n";
        indentLevel++;
        printIndent();
        std::cout << "Parameters:\n";
        indentLevel++;
        for (const auto& param : node->params) {
            printIndent();
            std::cout << "Param: " << param << "\n";
        }
        indentLevel--;
        printIndent();
        std::cout << "Body:\n";
        indentLevel++;
        node->body->accept(this);
        indentLevel--;
        indentLevel--;
    }

    void visit(ReturnStatement* node) override {
        printIndent();
        std::cout << "ReturnStatement node\n";
        indentLevel++;
        node->expression->accept(this);
        indentLevel--;
    }

    void visit(VariableDeclaration* node) override {
        printIndent();
        std::cout << "VariableDeclaration node: " << node->name << "\n";
        if (node->init) {
            indentLevel++;
            node->init->accept(this);
            indentLevel--;
        }
    }

private:
    int indentLevel;

    void printIndent() const {
        for (int i = 0; i < indentLevel; ++i) {
            std::cout << "    ";
        }
    }
};
