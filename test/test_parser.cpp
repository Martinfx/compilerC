#include <gtest/gtest.h>
#include "../compiler.hpp"

TEST(ParserTest, ParsesSimpleFunction) {
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
    ASSERT_NE(ast, nullptr);
    ASSERT_EQ(ast->type, NodeType::BlockStatement);
    auto block = std::static_pointer_cast<BlockStatement>(ast);
    ASSERT_EQ(block->statements.size(), 1);

    auto func = std::static_pointer_cast<FunctionDeclaration>(block->statements[0]);
    EXPECT_EQ(func->name, "main");
    EXPECT_EQ(func->params.size(), 0);
    //EXPECT_EQ(func->body->statements.size(), 1);

    //auto ret = std::static_pointer_cast<ReturnStatement>(func->body->statements[0]);
    //auto literal = std::static_pointer_cast<Literal>(ret->expression);
    //EXPECT_EQ(literal->value, "0");
}

TEST(ParserTest, ParsesVariableDeclaration) {
    std::vector<Token> tokens = {
        {TokenType::KEYWORD, "int"},
        {TokenType::IDENTIFIER, "main"},
        {TokenType::OPENPARENTHESIS, "("},
        {TokenType::CLOSEPARENTHESIS, ")"},
        {TokenType::OPENBRACE, "{"},
        {TokenType::KEYWORD, "int"},
        {TokenType::IDENTIFIER, "x"},
        {TokenType::OPERATOR, "="},
        {TokenType::LITERAL, "42"},
        {TokenType::SEMICOLON, ";"},
        {TokenType::CLOSEBRACE, "}"}
    };

    Parser parser(tokens);
    auto ast = parser.parse();
    ASSERT_NE(ast, nullptr);
    ASSERT_EQ(ast->type, NodeType::BlockStatement);
    auto block = std::static_pointer_cast<BlockStatement>(ast);
    ASSERT_EQ(block->statements.size(), 1);

    auto varDecl = std::static_pointer_cast<VariableDeclaration>(block->statements[0]);
    EXPECT_EQ(varDecl->name, "x");

    auto literal = std::static_pointer_cast<Literal>(varDecl->init);
    EXPECT_EQ(literal->value, "42");
}

TEST(ParserTest, ParsesIfStatement) {
    std::vector<Token> tokens = {
        {TokenType::KEYWORD, "if"},
        {TokenType::OPENPARENTHESIS, "("},
        {TokenType::IDENTIFIER, "x"},
        {TokenType::OPERATOR, "=="},
        {TokenType::LITERAL, "0"},
        {TokenType::CLOSEPARENTHESIS, ")"},
        {TokenType::OPENBRACE, "{"},
        {TokenType::KEYWORD, "return"},
        {TokenType::LITERAL, "1"},
        {TokenType::SEMICOLON, ";"},
        {TokenType::CLOSEBRACE, "}"},
        {TokenType::KEYWORD, "else"},
        {TokenType::OPENBRACE, "{"},
        {TokenType::KEYWORD, "return"},
        {TokenType::LITERAL, "0"},
        {TokenType::SEMICOLON, ";"},
        {TokenType::CLOSEBRACE, "}"}
    };

    Parser parser(tokens);
    auto ast = parser.parse();
    ASSERT_NE(ast, nullptr);
    ASSERT_EQ(ast->type, NodeType::BlockStatement);
    auto block = std::static_pointer_cast<BlockStatement>(ast);
    ASSERT_EQ(block->statements.size(), 1);

    auto ifStmt = std::static_pointer_cast<IfStatement>(block->statements[0]);
    auto condition = std::static_pointer_cast<BinaryOperation>(ifStmt->condition);
    EXPECT_EQ(condition->op.value, "==");

    auto thenStmt = std::static_pointer_cast<ReturnStatement>(ifStmt->thenStatement);
    auto thenLiteral = std::static_pointer_cast<Literal>(thenStmt->expression);
    EXPECT_EQ(thenLiteral->value, "1");

    auto elseStmt = std::static_pointer_cast<ReturnStatement>(ifStmt->elseStatement);
    auto elseLiteral = std::static_pointer_cast<Literal>(elseStmt->expression);
    EXPECT_EQ(elseLiteral->value, "0");
}

TEST(ParserTest, ParsesWhileStatement) {
    std::vector<Token> tokens = {
        {TokenType::KEYWORD, "while"},
        {TokenType::OPENPARENTHESIS, "("},
        {TokenType::IDENTIFIER, "x"},
        {TokenType::OPERATOR, "<"},
        {TokenType::LITERAL, "10"},
        {TokenType::CLOSEPARENTHESIS, ")"},
        {TokenType::OPENBRACE, "{"},
        {TokenType::IDENTIFIER, "x"},
        {TokenType::OPERATOR, "++"},
        {TokenType::SEMICOLON, ";"},
        {TokenType::CLOSEBRACE, "}"}
    };

    Parser parser(tokens);
    auto ast = parser.parse();
    ASSERT_NE(ast, nullptr);
    ASSERT_EQ(ast->type, NodeType::BlockStatement);
    auto block = std::static_pointer_cast<BlockStatement>(ast);
    ASSERT_EQ(block->statements.size(), 1);

    auto whileStmt = std::static_pointer_cast<WhileStatement>(block->statements[0]);
    auto condition = std::static_pointer_cast<BinaryOperation>(whileStmt->condition);
    EXPECT_EQ(condition->op.value, "<");

    auto body = std::static_pointer_cast<BlockStatement>(whileStmt->body);
    ASSERT_EQ(body->statements.size(), 1);

    auto exprStmt = std::static_pointer_cast<ExpressionStatement>(body->statements[0]);
    auto identifier = std::static_pointer_cast<Identifier>(exprStmt->expression);
    EXPECT_EQ(identifier->name, "x");
}

TEST(ParserTest, ParsesForStatement) {
    std::vector<Token> tokens = {
        {TokenType::KEYWORD, "for"},
        {TokenType::OPENPARENTHESIS, "("},
        {TokenType::KEYWORD, "int"},
        {TokenType::IDENTIFIER, "i"},
        {TokenType::OPERATOR, "="},
        {TokenType::LITERAL, "0"},
        {TokenType::SEMICOLON, ";"},
        {TokenType::IDENTIFIER, "i"},
        {TokenType::OPERATOR, "<"},
        {TokenType::LITERAL, "10"},
        {TokenType::SEMICOLON, ";"},
        {TokenType::IDENTIFIER, "i"},
        {TokenType::OPERATOR, "++"},
        {TokenType::CLOSEPARENTHESIS, ")"},
        {TokenType::OPENBRACE, "{"},
        {TokenType::KEYWORD, "return"},
        {TokenType::LITERAL, "i"},
        {TokenType::SEMICOLON, ";"},
        {TokenType::CLOSEBRACE, "}"}
    };

    Parser parser(tokens);
    auto ast = parser.parse();
    ASSERT_NE(ast, nullptr);
    ASSERT_EQ(ast->type, NodeType::BlockStatement);
    auto block = std::static_pointer_cast<BlockStatement>(ast);
    ASSERT_EQ(block->statements.size(), 1);

    auto forStmt = std::static_pointer_cast<ForStatement>(block->statements[0]);
    auto init = std::static_pointer_cast<VariableDeclaration>(forStmt->init);
    EXPECT_EQ(init->name, "i");

    auto condition = std::static_pointer_cast<BinaryOperation>(forStmt->condition);
    EXPECT_EQ(condition->op.value, "<");

    auto update = std::static_pointer_cast<Identifier>(forStmt->update);
    EXPECT_EQ(update->name, "i");

    auto body = std::static_pointer_cast<BlockStatement>(forStmt->body);
    ASSERT_EQ(body->statements.size(), 1);

    auto retStmt = std::static_pointer_cast<ReturnStatement>(body->statements[0]);
    auto retExpr = std::static_pointer_cast<Identifier>(retStmt->expression);
    EXPECT_EQ(retExpr->name, "i");
}
