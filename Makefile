# Makefile for C- Language Parser

# Compiler and flags
CXX = g++
CC = gcc
CXXFLAGS = -std=c++11 -Wall -Wextra
CFLAGS = -Wall -Wextra
LEX = flex
LEXFLAGS =

# Target executable
TARGET = parser

# Source files
LEXER_SOURCE = lexer_parser.l
LEXER_OUTPUT = lex.yy.c
CPP_SOURCES = main.cpp Parser.cpp ParseTree.cpp
HEADERS = token.h ParseTree.h Parser.h

# Object files
OBJECTS = main.o Parser.o ParseTree.o lex.yy.o

# Default target
all: $(TARGET)

# Generate lexer
$(LEXER_OUTPUT): $(LEXER_SOURCE)
	$(LEX) $(LEXFLAGS) $(LEXER_SOURCE)

# Compile lexer (C code) - compile as C to maintain C linkage
lex.yy.o: $(LEXER_OUTPUT)
	$(CC) $(CFLAGS) -Wno-unused-function -c $(LEXER_OUTPUT) -o lex.yy.o

# Compile C++ sources
main.o: main.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c main.cpp -o main.o

Parser.o: Parser.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c Parser.cpp -o Parser.o

ParseTree.o: ParseTree.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c ParseTree.cpp -o ParseTree.o

# Link all objects
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(TARGET)

# Clean build files
clean:
	rm -f $(OBJECTS) $(LEXER_OUTPUT) $(TARGET) *.dot *.png

# Run with test file
test: $(TARGET)
	./$(TARGET) tests/test_parser.c

# Run and generate PNG
test-png: $(TARGET)
	./$(TARGET) tests/test_parser.c parse_tree.dot
	dot -Tpng parse_tree.dot -o parse_tree.png
	@echo "Parse tree visualization saved to parse_tree.png"

# Generate PDF report
report:
	pandoc REPORT.md -o REPORT.pdf --pdf-engine=xelatex -V geometry:margin=1in -V fontsize=11pt --toc --toc-depth=3
	@echo "Report generated: REPORT.pdf"

report-typst:
	pandoc REPORT.md -o REPORT.typ.pdf --pdf-engine=typst --toc --toc-depth=3
	@echo "Report generated: REPORT.typ.pdf"

.PHONY: all clean test test-png report report-typst
