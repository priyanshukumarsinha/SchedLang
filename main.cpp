#include<bits/stdc++.h>
#include "lexer/lexer.h"
int main(){
    std::string source = R"(
        task controlLoop {
            priority = 5;
            deadline = 100;
        }
    )";

    Lexer lex(source);

    // we keep finding next tokens until we get an EOF_TOKEN
    // printing the symbol table
    std::cout << "ID\t" << "TokenType\t" << "line[col]\t" << "Symbol\n";
    size_t idx = 0;
    while(true){
        Token t = lex.nextToken(); 
        // this gives token which is to be processed now
        // now lets print the token
        std::cout << idx << "\t" << static_cast<int>(t.type) << "\t" 
                  << t.line << "[" << t.col << "]\t" << 
                  t.lexeme << "\n";
        if (t.type == TokenType::EOF_TOKEN) break;
        idx++;

    }

    return 0;
}