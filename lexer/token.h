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
    TOKEN_EOF

};

class Token{
    TokenType type;
    std::string lexeme;
    int intVal; // valid if type == INT    
    int line;
    int col;

    Token(){
        type = TokenType::UNKNOWN;
        lexeme = "";
        line = 0;
        col = 0;
    }

    Token(TokenType type, std::string lexeme, int line, int col){
        this->type = type;
        this->lexeme = lexeme;
        this->intVal = 0;
        this->line = line;
        this->col = col;
    }

    Token(TokenType type, int intVal, int line, int col){
        this->type = type;
        this->intVal = intVal;
        this->lexeme = std::to_string(intVal);
        this->line = line;
        this->col = col;
    }
};

#endif