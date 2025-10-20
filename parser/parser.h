#ifndef PARSER_H
#define PARSER_H

#include "../lexer/lexer.h"
#include "../lexer/token.h"
#include "ast.h"

class Parser{
private:
    Lexer *lexer;
    Token cur;
    void parseTaskDecl(ProgramNode &prog);
    void runSemanticChecks(ProgramNode &prog);
    void expect(TokenType t, const std:: string &msg);
    void advance();
    void parseTaskBody(TaskNode &task);
    void parseProperty(TaskNode &task);
public: 
    Parser(Lexer &lexer);
    ProgramNode parseProgram();
};

#endif
