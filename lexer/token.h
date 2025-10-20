#ifndef TOKEN_H
#define TOKEN_H
#pragma once
#include<string>

enum class TokenType{
    // UNKOWN TYPE
    UNKNOWN,
    
    // Keywords
    KW_TASK, KW_PRIORITY, KW_DEADLINE,

    // Identifier
    IDENT,

    // Int Literal
    INT,

    // Symbols
    EQ, SEMI, LBRACE, RBRACE,

    // End of file
    EOF_TOKEN

};

struct Token{
    TokenType type;
    std::string lexeme;
    int intVal; // valid if type == INT    
    int line;
    int col;
};

#endif