#include <gtest/gtest.h>
#include "../compiler.hpp"

TEST(UnaryOperationTest, TestUnaryNegationInFunction) {
    std::vector<Token> tokens = {
        {TokenType::KEYWORD, "int"},
        {TokenType::IDENTIFIER, "main"},
        {TokenType::OPENPARENTHESIS, "("},
        {TokenType::CLOSEPARENTHESIS, ")"},
        {TokenType::OPENBRACE, "{"},
        {TokenType::KEYWORD, "return"},
        {TokenType::OPERATOR, "-"},
        {TokenType::LITERAL, "5"},
        {TokenType::SEMICOLON, ";"},
        {TokenType::CLOSEBRACE, "}"}
    };

    Parser parser(tokens);
    auto ast = parser.parse();

    ASSERT_EQ(ast->type, NodeType::BlockStatement);
    auto block = std::dynamic_pointer_cast<BlockStatement>(ast);
    ASSERT_NE(block, nullptr);
    ASSERT_EQ(block->statements.size(), 1);

    auto functionDecl = std::dynamic_pointer_cast<FunctionDeclaration>(block->statements[0]);
    ASSERT_NE(functionDecl, nullptr);
    EXPECT_EQ(functionDecl->name, "main");

    auto functionBody = std::dynamic_pointer_cast<BlockStatement>(functionDecl->body);
    ASSERT_NE(functionBody, nullptr);
    ASSERT_EQ(functionBody->statements.size(), 1);

    auto returnStmt = std::dynamic_pointer_cast<ReturnStatement>(functionBody->statements[0]);
    ASSERT_NE(returnStmt, nullptr);

    auto unaryOp = std::dynamic_pointer_cast<UnaryOperation>(returnStmt->expression);
    ASSERT_NE(unaryOp, nullptr);
    EXPECT_EQ(unaryOp->op.value, "-");
    ASSERT_EQ(unaryOp->operand->type, NodeType::Literal);
}

TEST(UnaryOperationTest, TestLogicalNegation) {
    std::vector<Token> tokens = {
        {TokenType::KEYWORD, "int"},
        {TokenType::IDENTIFIER, "main"},
        {TokenType::OPENPARENTHESIS, "("},
        {TokenType::CLOSEPARENTHESIS, ")"},
        {TokenType::OPENBRACE, "{"},
        {TokenType::KEYWORD, "return"},
        {TokenType::OPERATOR, "!"},
        {TokenType::LITERAL, "0"},
        {TokenType::SEMICOLON, ";"},
        {TokenType::CLOSEBRACE, "}"}
    };

    Parser parser(tokens);
    auto ast = parser.parse();

    ASSERT_EQ(ast->type, NodeType::BlockStatement);
    auto block = std::dynamic_pointer_cast<BlockStatement>(ast);
    ASSERT_NE(block, nullptr);
    ASSERT_EQ(block->statements.size(), 1);

    auto functionDecl = std::dynamic_pointer_cast<FunctionDeclaration>(block->statements[0]);
    ASSERT_NE(functionDecl, nullptr);
    EXPECT_EQ(functionDecl->name, "main");

    auto functionBody = std::dynamic_pointer_cast<BlockStatement>(functionDecl->body);
    ASSERT_NE(functionBody, nullptr);
    ASSERT_EQ(functionBody->statements.size(), 1);

    auto returnStmt = std::dynamic_pointer_cast<ReturnStatement>(functionBody->statements[0]);
    ASSERT_NE(returnStmt, nullptr);

    auto unaryOp = std::dynamic_pointer_cast<UnaryOperation>(returnStmt->expression);
    ASSERT_NE(unaryOp, nullptr);
    EXPECT_EQ(unaryOp->op.value, "!");
    ASSERT_EQ(unaryOp->operand->type, NodeType::Literal);
}

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

    auto functionBody = std::dynamic_pointer_cast<BlockStatement>(func->body);
    ASSERT_NE(functionBody, nullptr);
    ASSERT_EQ(functionBody->statements.size(), 1);

    auto ret = std::static_pointer_cast<ReturnStatement>(functionBody->statements[0]);
    auto literal = std::static_pointer_cast<Literal>(ret->expression);
    EXPECT_EQ(literal->value, "0");
}

TEST(ParserTest, ParsesVariableDeclaration) {
    // Token stream representing the code:
    // int main() {
    //     int x = 42;
    //     return x;
    // }
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

    // Ensure the AST is not null
    ASSERT_NE(ast, nullptr);

    // Verify the root node is a BlockStatement
    ASSERT_EQ(ast->type, NodeType::BlockStatement);
    auto block = std::dynamic_pointer_cast<BlockStatement>(ast);
    ASSERT_NE(block, nullptr);
    ASSERT_EQ(block->statements.size(), 1);

    // Verify the root block contains a FunctionDeclaration
    auto funcDecl = std::dynamic_pointer_cast<FunctionDeclaration>(block->statements[0]);
    ASSERT_NE(funcDecl, nullptr);
    EXPECT_EQ(funcDecl->name, "main");

    // Check that the function body is a BlockStatement
    auto funcBody = std::dynamic_pointer_cast<BlockStatement>(funcDecl->body);
    ASSERT_NE(funcBody, nullptr);
    ASSERT_EQ(funcBody->statements.size(), 2);

    // Verify that the body contains a VariableDeclaration
    auto varDecl = std::dynamic_pointer_cast<VariableDeclaration>(funcBody->statements[0]);
    ASSERT_NE(varDecl, nullptr);
    EXPECT_EQ(varDecl->name, "x");

    // Verify the initializer is a Literal
    auto literal = std::dynamic_pointer_cast<Literal>(varDecl->init);
    EXPECT_EQ(literal->type, NodeType::Literal);
    ASSERT_NE(literal, nullptr);
    EXPECT_EQ(literal->value, "42");
}


TEST(ParserTest, ParsesIfStatement) {
    // Token stream representing the code:
    // int main() {
    //     if (x == 0) {
    //         return 1;
    //     } else {
    //         return 0;
    //     }
    // }
    std::vector<Token> tokens = {
        {TokenType::KEYWORD, "int"},
        {TokenType::IDENTIFIER, "main"},
        {TokenType::OPENPARENTHESIS, "("},
        {TokenType::CLOSEPARENTHESIS, ")"},
        {TokenType::OPENBRACE, "{"},
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
        {TokenType::CLOSEBRACE, "}"},
        {TokenType::CLOSEBRACE, "}"}
    };

    Parser parser(tokens);
    auto ast = parser.parse();

    // Ensure the AST is not null
    ASSERT_NE(ast, nullptr);

    // Check that the root node is a BlockStatement
    ASSERT_EQ(ast->type, NodeType::BlockStatement);
    auto block = std::dynamic_pointer_cast<BlockStatement>(ast);
    ASSERT_NE(block, nullptr);
    ASSERT_EQ(block->statements.size(), 1);

    // Verify the root block contains a FunctionDeclaration
    auto funcDecl = std::dynamic_pointer_cast<FunctionDeclaration>(block->statements[0]);
    ASSERT_NE(funcDecl, nullptr);
    EXPECT_EQ(funcDecl->name, "main");

    // Check that the function body is a BlockStatement
    auto funcBody = std::dynamic_pointer_cast<BlockStatement>(funcDecl->body);
    ASSERT_NE(funcBody, nullptr);
    ASSERT_EQ(funcBody->statements.size(), 1);

    // Verify that the body contains an IfStatement
    auto ifStmt = std::dynamic_pointer_cast<IfStatement>(funcBody->statements[0]);
    ASSERT_NE(ifStmt, nullptr);

    // Check the condition of the IfStatement
    auto condition = std::dynamic_pointer_cast<BinaryOperation>(ifStmt->condition);
    ASSERT_NE(condition, nullptr);
    EXPECT_EQ(condition->op.value, "==");

    // Validate the left and right operands of the condition
    auto leftOperand = std::dynamic_pointer_cast<Identifier>(condition->left);
    auto rightOperand = std::dynamic_pointer_cast<Literal>(condition->right);
    ASSERT_NE(leftOperand, nullptr);
    ASSERT_NE(rightOperand, nullptr);
    EXPECT_EQ(leftOperand->name, "x");
    EXPECT_EQ(rightOperand->value, "0");

    // Validate the "then" branch
    auto thenStmt = std::dynamic_pointer_cast<BlockStatement>(ifStmt->thenStatement);
    ASSERT_NE(thenStmt, nullptr);
    ASSERT_EQ(thenStmt->statements.size(), 1);
    auto thenReturn = std::dynamic_pointer_cast<ReturnStatement>(thenStmt->statements[0]);
    ASSERT_NE(thenReturn, nullptr);
    auto thenLiteral = std::dynamic_pointer_cast<Literal>(thenReturn->expression);
    ASSERT_NE(thenLiteral, nullptr);
    EXPECT_EQ(thenLiteral->value, "1");

    // Validate the "else" branch
    auto elseStmt = std::dynamic_pointer_cast<BlockStatement>(ifStmt->elseStatement);
    ASSERT_NE(elseStmt, nullptr);
    ASSERT_EQ(elseStmt->statements.size(), 1);
    auto elseReturn = std::dynamic_pointer_cast<ReturnStatement>(elseStmt->statements[0]);
    ASSERT_NE(elseReturn, nullptr);
    auto elseLiteral = std::dynamic_pointer_cast<Literal>(elseReturn->expression);
    ASSERT_NE(elseLiteral, nullptr);
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
