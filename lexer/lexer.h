#ifndef LEXER_H
#define LEXER_H
#include<string>
#include<vector>
#include "token.h"

class Lexer{
private:
    std::string src;
    size_t pos;
    int line;
    int col;
    std::vector<Token> buffer; //lookahead cache
    void skipWhitespaceAndComments();
    char peekChar(int ahead);
    char advance();
    Token lexIdentifierOrKeyword();
    Token lexNumber();

public:
    Lexer(const std::string& input);
    Token nextToken();
};

#endif