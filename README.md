# Lexer and Recursive Descent Parser for SchedLang

Real-time embedded systems require deterministic task scheduling to ensure timely and predictable task execution. To facilitate this, we introduce **SchedLang**, a domain-specific language (DSL) designed to define task schedules, priorities, and deadlines in a structured way.

This project implements both a **lexical analyzer (lexer)** and a **recursive descent parser** for SchedLang. The lexer converts raw input into tokens, while the parser verifies syntax and constructs an Abstract Syntax Tree (AST). Semantic checks ensure tasks are valid and non-conflicting.

---

## **Language Overview**

### **Purpose**

SchedLang describes configurations of periodic or aperiodic tasks in real-time systems. Each task specifies:

* **Task name** (identifier)
* **Priority level**
* **Deadline**
* Optionally, other parameters like execution time or period in extended versions

---

### **Program Structure**

A SchedLang program consists of one or more **task declarations**, each defining the task name and its properties within braces `{}`.

**Syntax:**

```
task <identifier> {
    priority = <integer>;
    deadline = <integer>;
}
```

---

## **Lexical Specification (Lexer)**

### **Tokens**

The lexer breaks source code into these categories:

| Token Type    | Example       | Description              |
| ------------- | ------------- | ------------------------ |
| `KW_TASK`     | `task`        | Task declaration keyword |
| `KW_PRIORITY` | `priority`    | Task priority keyword    |
| `KW_DEADLINE` | `deadline`    | Task deadline keyword    |
| `IDENT`       | `controlLoop` | Task name (user-defined) |
| `INT`         | `5`, `100`    | Integer literals         |
| `EQ`          | `=`           | Assignment operator      |
| `SEMI`        | `;`           | Statement terminator     |
| `LBRACE`      | `{`           | Start of task block      |
| `RBRACE`      | `}`           | End of task block        |
| `EOF_TOKEN`   | —             | End-of-file marker       |

---

### **Lexical Rules**

1. **Keywords** (`task`, `priority`, `deadline`) are matched exactly.
2. **Identifiers** start with a letter and may contain letters or digits.
3. **Integers** are sequences of digits (`[0-9]+`).
4. **Whitespace** is ignored.
5. **Symbols** (`=`, `;`, `{`, `}`) are treated as individual tokens.

---

### **Example Token Stream**

Input:

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
EOF_TOKEN
```

---

## **Grammar Specification (Parser)**

SchedLang’s grammar defines valid program structure:

```
Program        ::= { TaskDecl }
TaskDecl       ::= "task" IDENT "{" TaskBody "}"
TaskBody       ::= { (PriorityDecl | DeadlineDecl) }
PriorityDecl   ::= "priority" "=" INT ";"
DeadlineDecl   ::= "deadline" "=" INT ";"
```

**Characteristics:**

* Grammar is **LL(1)**, suitable for recursive descent parsing.
* Parsing is **deterministic** and operates in linear time.
* Syntax errors are detected and reported clearly.

---

## **Parser Implementation**

Each grammar non-terminal maps to a dedicated C++ function:

* `parseProgram()` → parses the full program
* `parseTaskDecl()` → parses a single task
* `parseTaskBody()` → parses task properties
* `parseProperty()` → handles priority/deadline parsing
* `runSemanticChecks()` → validates task names, required properties, and value ranges

**Error Handling:**

* If an unexpected token occurs, the parser throws a descriptive syntax error:

```
Syntax Error: Expected ';' but found 'deadline'
```

* Semantic errors are also reported with line numbers:

```
Semantic Error: Duplicate task name 'controlLoop' at line 1
Semantic Error: Invalid priority value for task 'sensorTask' at line 2: must be >= 0
```

---

## **Directory Structure**

```
priyanshukumarsinha-schedlang/
├── include/
│   ├── lexer.h
│   ├── parser.h
│   ├── ast.h
│   └── token.h
├── src/
│   ├── lexer.cpp
│   ├── parser.cpp
│   └── main.cpp
├── examples/
│   └── example.slang
├── Makefile
└── README.md
```

---

## **How to Build & Run**

### **Using Make**

```bash
make
```

Generates executable:

```
schedlang
```

### **Manual Compilation**

```bash
g++ -std=c++17 -Iinclude src/*.cpp -o schedlang
```

### **Run Example**

```bash
./schedlang examples/example.slang
```

---

## **Example Execution**

### Input (`example.slang`)

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

### Output

```
Parsed task: controlLoop
  priority = 5
  deadline = 100
Parsed task: sensorTask
  priority = 3
  deadline = 200
Parsing completed successfully!
```

**Error Example**

```
Syntax Error: Unexpected token 'prio' at line 2:5. Expected 'priority', 'deadline', or '}'.
```

---

## **Design Features**

* **Readable structure:** Grammar maps directly to parser functions
* **Extendable:** Easily add new keywords (`period`, `wcet`)
* **Low latency:** One-pass deterministic parsing for embedded systems
* **Modular:** Lexer, parser, and semantic checker are separate components

---

## **Conclusion**

The **SchedLang lexer and recursive descent parser** provide a solid foundation for defining and validating real-time scheduling tasks. The system ensures deterministic parsing, early error detection, and a modular structure suitable for expansion into a full scheduling compiler or simulator.


