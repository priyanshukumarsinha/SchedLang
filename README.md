# Lexer and Recursive Descent Parser for SchedLang

Real-time embedded systems often require deterministic task scheduling to ensure timely and predictable task execution. To facilitate this, we introduce **SchedLang**, a simple domain-specific language (DSL) designed to define task schedules, priorities, and deadlines in an expressive and structured way.

This project implements both a **lexical analyzer (lexer)** and a **recursive descent parser** for SchedLang. The lexer converts raw input text into a sequence of tokens, while the parser verifies the syntactic structure of the input and constructs a parse tree. Together, they form the foundation of a small compiler front end for real-time scheduling configurations.

---

## **Language Overview**

### **Purpose**

SchedLang is designed to describe the configuration of periodic or aperiodic tasks in real-time embedded systems. Each task specifies:

* **Task name** (identifier)
* **Priority level**
* **Deadline**
* (Optionally) other parameters like execution time or period in extended versions

--- 

### **Program Structure**

A SchedLang program consists of one or more **task declarations**.
Each task declaration defines the task name and its parameters within braces `{}`.

**General Syntax:**

```
task <identifier> {
    priority = <integer>;
    deadline = <integer>;
}
```

---

## **Lexical Specification (Lexer)**

### **Tokens**

The lexical analyzer breaks the source code into the following token categories:

| Token Type    | Example       | Description              |
| ------------- | ------------- | ------------------------ |
| `KW_TASK`     | `task`        | Declares a new task      |
| `KW_PRIORITY` | `priority`    | Declares task priority   |
| `KW_DEADLINE` | `deadline`    | Declares task deadline   |
| `IDENT`       | `controlLoop` | Task name (user-defined) |
| `INT`         | `100`, `5`    | Integer literal value    |
| `EQ`          | `=`           | Assignment operator      |
| `SEMI`        | `;`           | Statement terminator     |
| `LBRACE`      | `{`           | Start of task block      |
| `RBRACE`      | `}`           | End of task block        |
| `END_OF_FILE` | —             | End of input marker      |
---
### **Lexical Rules**

1. **Keywords** (`task`, `priority`, `deadline`) are matched exactly.
2. **Identifiers** begin with a letter and may contain letters or digits.
3. **Integers** are sequences of digits (`[0-9]+`).
4. **Whitespace** (spaces, tabs, newlines) is ignored.
5. **Symbols** (`=`, `;`, `{`, `}`) are treated as individual tokens.
---
### **Example Token Stream**

For:

```schedlang
task controlLoop {
    priority = 5;
    deadline = 100;
}
```

Lexer Output:

```
KW_TASK (task)
IDENT (controlLoop)
LBRACE ({)
KW_PRIORITY (priority)
EQ (=)
INT (5)
SEMI (;)
KW_DEADLINE (deadline)
EQ (=)
INT (100)
SEMI (;)
RBRACE (})
END_OF_FILE
```

---

## **Grammar Specification (Parser)**

SchedLang’s grammar defines the valid syntactic structure for programs.
---
### **Grammar Rules**

```
Program        ::= { TaskDecl }
TaskDecl       ::= "task" IDENT "{" TaskBody "}"
TaskBody       ::= { (PriorityDecl | DeadlineDecl) }
PriorityDecl   ::= "priority" "=" INT ";"
DeadlineDecl   ::= "deadline" "=" INT ";"
```
---

### **Characteristics**

* Grammar is **LL(1)** (non-left-recursive and suitable for recursive descent parsing).
* Parsing is **deterministic** and operates in linear time relative to input size.
* Syntax errors are detected early and reported clearly.

---

## **Parser Implementation**

### **Approach**

A **recursive descent parser** is implemented, where each non-terminal is represented by a dedicated C++ function:

* `parseProgram()` → parses the full program.
* `parseTaskDecl()` → parses a single task.
* `parseTaskBody()` → parses its internal declarations.
* `parsePriorityDecl()` / `parseDeadlineDecl()` → handle specific parameter rules.

Each function calls others recursively as per grammar hierarchy.

### **Error Handling**
---

If a token does not match the expected one, the parser throws a **syntax error** with the offending token and a descriptive message, e.g.:

```
Syntax Error: Expected ';' but found 'deadline'
```


## **Directory Structure**

```
priyanshukumarsinha-schedlang/
├── include/
│   ├── lexer.h
│   ├── parser.h
│   └── token.h
├── src/
│   ├── lexer.cpp
│   ├── parser.cpp
│   └── main.cpp
├── examples/
│   └── example.sched
├── Makefile
└── README.md
```


## **Example Execution**

### **Input**

```schedlang
task controlLoop {
    priority = 5;
    deadline = 100;
}

task sensorTask {
    priority = 3;
    deadline = 200;
}
```

### **Output**

```
Parsed task: controlLoop
  priority = 5
  deadline = 100
Parsed task: sensorTask
  priority = 3
  deadline = 200
Parsing completed successfully!
```

If an error occurs:

```
Syntax Error: Expected ';' but found 'deadline'
```


## **Design Choices and Features**

* **Readable structure:** Grammar maps directly to parser functions.
* **Extendable:** New keywords (like `period`, `wcet`) can be added easily.
* **Low latency:** One-pass, deterministic parsing suitable for embedded systems.
* **Modularity:** Separate lexer and parser modules for clarity and reuse.


## **Future Enhancements**

* Add new attributes (`wcet`, `period`, `start_time`).
* Generate an **Abstract Syntax Tree (AST)** for code generation or scheduling simulation.
* Implement **semantic analysis** for constraint validation (e.g., deadline ≥ WCET).
* Build a simple interpreter or compiler backend to simulate scheduling.


## **Conclusion**

The **SchedLang lexer and recursive descent parser** provide a solid foundation for defining and validating real-time scheduling tasks. The system ensures deterministic parsing, clear syntax error detection, and a modular structure suitable for further expansion into a full compiler or scheduling simulator for embedded applications.

---

