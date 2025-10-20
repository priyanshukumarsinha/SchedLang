#ifndef AST_H
#define AST_H
#include <vector>
#include<string>

struct Property{
    // Property — a single key/value pair inside a task 
    // (e.g., "priority" -> 5), with source position for diagnostics.
    std::string key;
    int value;
    int line;
    int col;
};

struct TaskNode{
    // represents one task declaration;
    // name, list of properties, and source position
    // task <identifier> {
    //     priority = <integer>;
    //     deadline = <integer>;
    // }
    std::string name; //identifier
    std::vector<Property> props;
    // properties such as priority or deadline
    int line;
    int col;

};

// top level AST, a list of tasks
struct ProgramNode{
    std::vector<TaskNode> tasks;
};


#endif