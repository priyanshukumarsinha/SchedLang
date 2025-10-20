#include"lexer.h"
#include<iostream>

Lexer::Lexer(const std::string& input){
    this->src = input;
    this->pos = 0;
    this->line = 1;
    this->col = 0;
}

char Lexer::peekChar(int ahead=0){
    if(pos+ahead >= src.size()) return '\0';
    return src[pos+ahead];
}

char Lexer::advance(){
    if(pos>=src.size()) return '\0';
    if(src[pos] == '\n'){
        line++;
        col = 1;
    }
    else{
        col++;
    }
    return src[pos++];

    // we can also return current value here
    // null if pos>=src.size()
    // else src[pos]
}

void Lexer::skipWhitespaceAndComments(){
    while(pos<src.size()){
        char c = peekChar();
        // get the current character, since ahead = 0
        
        // isspace() function to check whitespace
        if(isspace(c)){
            // we need to move forward
            // but here its not as easy as a pos++
            // here we also need to move col and line
            // so we will make a function for advancing advance()
            advance();
            continue;
        }
        // for comments : comments start with #
        if(c == '#'){
            advance();
            while(pos<src.size() && src[pos]!='\n'){
                advance();
            }
            continue;
        }

        // break when anything other than space or comments is found
        break;

    }
}

Token Lexer::lexIdentifierOrKeyword(){
    size_t startLine = line, startCol = col;
    size_t start;
    std::string value;
    while(isalnum(peekChar()) || peekChar() == '_'){
        value += advance();
    }

    // now lets make a token
    Token tok;
    tok.line = startLine;
    tok.col = startCol;
    tok.lexeme = value;
    
    // now check for keywords
    if (value == "task"){
        tok.type = TokenType::KW_TASK;
    }
    else if(value == "deadline"){
        tok.type = TokenType::KW_DEADLINE;
    }
    else if(value == "priority"){
        tok.type = TokenType::KW_PRIORITY;
    }
    else{
        // it must be an identifier
        tok.type = TokenType::IDENT;
    }

    return tok;

}

Token Lexer::nextToken(){
    // A recursive descent parser works by trying to match grammar rules one token at a time.
    // Sometimes, to decide which rule to apply, the parser must see more than one token ahead.
    // Example:
    // When parsing propertyStmt, the parser looks at the first token to decide:
    // 'priority' → parse a priority statement
    // 'deadline' → parse a deadline statement
    // something else → stop parsing the task body
    // hence we need the buffer array which processes all the tokens till the lookahead
    // this array only stores the tokens which are yet not been given to the parser
    // this helps in resolving or matching the grammar
    // because some grammars are ambiguous unless you look 1 or 2 tokens ahead
    if(!buffer.empty()){
        Token t = buffer.front();
        buffer.erase(buffer.begin());
        return t;
    }



    // now if we don't have already processed tokens
    // Lets process them then

    // 1. skip whitespaces and comments
    skipWhitespaceAndComments();
    // 2. if EOF return EOF_TOKEN
    if(pos>=src.size()){
        // this means we reached EOF
        return Token({TokenType::EOF_TOKEN, "", 0, line, col});
    }

    char c = peekChar();
    // now it could either be an identifier or an iteger
    // based on start
    if(isalpha(c)) return lexIdentifierOrKeyword();
    // if(isdigit(c)) return lexNumber();

    pos++;
    Token t1;
    return t1;

}