#include "parser.h"
#include "ast.h"
#include <stdexcept>
#include <iostream>
#include <set>
#include <string>

Parser::Parser(Lexer &lexer){
    this->lexer = &lexer;
    cur = lexer.peek(0);
}

void Parser::advance(){
    if(lexer->peek(0).type == cur.type && lexer->peek(0).lexeme == cur.lexeme){
        lexer->nextToken();

    }
    cur = lexer->peek(0);
}

// except checks if the current token is the expected token
void Parser::expect(TokenType t, const std::string &msg){
    if(cur.type != t){
        throw std::runtime_error("Syntax error at line " + std::to_string(cur.line) + ":" + std::to_string(cur.col) + ", " + msg + ", got '" + cur.lexeme + "'");
    }
}

void Parser::parseTaskDecl(ProgramNode &prog){
    int startLine = cur.line, startCol = cur.col;
    // currently cur == KW_TASK
    expect(TokenType::KW_TASK, "expected 'task'");
    advance();

    // next there should be a task name i.e identifier
    expect(TokenType::IDENT, "expected 'taskname <identifier>'");
    // if identifier found lets create a TaskNode then
    
    TaskNode t;
    t.name = cur.lexeme;
    t.line = cur.line;
    t.col = cur.col;
    
    advance();
    // now we need to give t.props
    // for that we need to scan further
    // next we expect a LBRACE {
    expect(TokenType::LBRACE, "expected '{'");
    advance();
    // if { present then we advance forward
    // and check TaskBody

    parseTaskBody(t);

    // if the nextToken gives KW_DEADLINE || KW_PRIORITY
    // these are consumed inside parseTaskBody()
    // but if } RBRACE it needs to be consumed here

    expect(TokenType::RBRACE, "expected '}'");
    advance();

    // add this task to ProgramNode
    prog.tasks.push_back(std::move(t));
    // move transfers all contents of t to tasks
    // doesn't copy but directly gives ownership 

}

void Parser::parseTaskBody(TaskNode &task){
    // the task body consists of various properties
    // so we need to parse properties here
    while(true){
        if(cur.type == TokenType::KW_DEADLINE || cur.type==TokenType::KW_PRIORITY){
            parseProperty(task);
            continue;
        }
        else if(cur.type == TokenType::RBRACE){
            // break out
            // parseTaskDecl will consume RBRACE
            break;
        }
        else{
            // unexpected token
            throw std::runtime_error("Unexpected token in task body at line " 
            + std::to_string(cur.line) + ":" + std::to_string(cur.col)
            + " ('" + cur.lexeme + "'). Expected 'priority', 'deadline', or '}'.");
        }
    }

    // only parse if its priority or deadline
}

void Parser::parseProperty(TaskNode &task){
    Property p;

    if(cur.type == TokenType::KW_PRIORITY){
        p.key = "priority";
    }
    else{
        // cur.type == TokenType::KW_DEADLINE
        p.key = "deadline";
    }
    p.line = cur.line;
    p.col = cur.col;

    advance();
    // next we expect assignment operator 
    expect(TokenType::EQ, "expected '=' after property name");
    advance();

    // next we expect an INT Token
    expect(TokenType::INT, "expected integer literal for property value");
    advance();

    // next we expect a semicolon
    expect(TokenType::SEMI, "expected ';' after property");
    advance();

    // now current is either at nextProperty or }
    // which will be handled by praseTaskDecl or parseTaskBody

    // our current property must not be already present 
    // in the task we parsing right now
    for(auto &existing : task.props){
        // check with new Property key before 
        // pushing back to task.props
        if(existing.key == p.key){
            throw std::runtime_error("Duplicate property '" + p.key + "' in task '" + task.name + "' at line " + std::to_string(p.line));
        }
    }

    // push unique task
    task.props.push_back(p);
    
}

void Parser::runSemanticChecks(ProgramNode &prog){
    // now lets check as per our rules
    // Every task should have both these properties
    // i.e priority and deadline
    // both of them should have a value >=0
    // no duplicate task name

    std::set<std::string> names;
    // set of name of all tasks seen till now
    for(auto &t:prog.tasks){
        if(names.count(t.name)){
            // if name already seen
            // that means its duplicate name
            // throw error 
            throw std::runtime_error("Duplicate task name: '" + t.name + "' at line " + std::to_string(t.line));
        }
        // if not seen till now, put it in names now
        names.insert(t.name);
        // check if it has priority
        bool hasPr = false, hasDl = false;
        for(auto &p:t.props){
            if(p.key == "priority") hasPr = true;
            else if(p.key == "deadline") hasDl = true;

            // for values
            if (p.key == "priority" && (p.value < 0)) {
                throw std::runtime_error("Invalid priority value for task '" + t.name + "' at line " + std::to_string(p.line) + ": must be >= 0");
            }
            else if (p.key == "deadline" && (p.value <= 0)) {
                throw std::runtime_error("Invalid deadline value for task '" + t.name + "' at line " + std::to_string(p.line) + ": must be > 0");
            }
        }
        
        if (!hasPr) throw std::runtime_error("Task '" + t.name + "' missing 'priority' property at line " + std::to_string(t.line));
        if (!hasDl) throw std::runtime_error("Task '" + t.name + "' missing 'deadline' property at line " + std::to_string(t.line));

    }
}

ProgramNode Parser::parseProgram(){
    ProgramNode prog;
    // task <identifier> {
    //     priority = <integer>;
    //     deadline = <integer>;
    // }

    // 1. find task keyword KW_TASK

    while(cur.type == TokenType::KW_TASK){
        // 2. Simplify TaskDecl
        // std::cout << "got task keyword" << std::endl;
        parseTaskDecl(prog);
    }

    // if task keyword not present at start
    // then it must be EOF else error
    if(cur.type != TokenType::EOF_TOKEN){
        throw std::runtime_error("Unexpected token at end of program: '" + cur.lexeme + "'");
    }

    // now lets check as per our rules
    // Every task should have both these properties
    // i.e priority and deadline
    // both of them should have a value >=0
    // no duplicate task name

    // for that we define runSemanticChecks();
    runSemanticChecks(prog);
    return prog;
}