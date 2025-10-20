#include<bits/stdc++.h>
#include "lexer/lexer.h"
#include "parser/parser.h"
#include "parser/ast.h"

int main(int argc, char*argv[]){
    if(argc!=2){
        std::cerr << "Usage: " << argv[0] << " <filename>\n";
        return 1;
    }

    std::string filename = argv[1];
    std::ifstream file(filename);
    if(!file){
        std::cerr << "ERROR :: FILE NOT FOUND :: " << filename << std::endl;
        return 1;
    }
    std::stringstream buffer;
    buffer << file.rdbuf(); // read entire file
    std::string input = buffer.str();

    Lexer lex(input);

    // we keep finding next tokens until we get an EOF_TOKEN
    // printing the symbol table
    std::cout << "ID\t" << "TokenType\t" << "line[col]\t" << "Symbol\n";
    // size_t idx = 0;
    // while(true){
    //     Token t = lex.nextToken(); 
    //     // this gives token which is to be processed now
    //     // now lets print the token
    //     std::cout << idx << "\t" << static_cast<int>(t.type) << "\t\t" 
    //               << t.line << "[" << t.col << "]\t\t" << 
    //               t.lexeme << "\n";
    //     if (t.type == TokenType::EOF_TOKEN) break;
    //     idx++;
    // }
    try{
        // lets create a parser
        Parser p(lex);

        // parse the program to make an AST Tree
        ProgramNode prog = p.parseProgram();
        std::cout << "Parsed " << prog.tasks.size() << " task(s) successfully.\n";
        for (auto &t : prog.tasks) {
        std::cout << "Task: " << t.name << "\n";
        for (auto &pr : t.props) std::cout << " " << pr.key << " = " << pr.value << "\n";
        }
    }
    catch(const std::exception &e){
        std::cerr << "ERROR:: " << e.what() << "\n";
        return 1;
    }

    return 0;
}