SRC_DIR = .
LEXER_DIR = lexer
PARSER_DIR = parser
BUILD_DIR = build

SRCS = $(SRC_DIR)/main.cpp \
	   $(LEXER_DIR)/lexer.cpp \
	   $(PARSER_DIR)/parser.cpp 

OBJS = $(BUILD_DIR)/main.o \
	   $(BUILD_DIR)/lexer.o \
	   $(BUILD_DIR)/parser.o 

TARGET = $(BUILD_DIR)/slangparser

all: $(TARGET)

# Build Executable
$(TARGET): $(OBJS)
	@mkdir -p $(BUILD_DIR)
	g++ -o $(TARGET) $(OBJS)

# create main.o as in OBJS
$(BUILD_DIR)/main.o: main.cpp lexer/lexer.h lexer lexer/token.h parser/parser.h
	@mkdir -p $(BUILD_DIR)
	g++ -I. -c main.cpp -o $(BUILD_DIR)/main.o

# create lexer.o as in OBJS
$(BUILD_DIR)/lexer.o: lexer/lexer.cpp lexer/lexer.h lexer lexer/token.h
	@mkdir -p $(BUILD_DIR)
	g++ -I. -c $(LEXER_DIR)/lexer.cpp -o $(BUILD_DIR)/lexer.o

# create parser.o as in OBJS
$(BUILD_DIR)/parser.o: parser/parser.cpp lexer/lexer.h lexer lexer/token.h parser/parser.h
	@mkdir -p $(BUILD_DIR)
	g++ -I. -c $(PARSER_DIR)/parser.cpp -o $(BUILD_DIR)/parser.o

clean:
	rm -rf $(BUILD_DIR)