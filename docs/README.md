# C- Language Parser

A recursive descent parser for the C- programming language with left recursion removal and left factoring applied to the grammar.

## Project Structure

```
.
├── grammar_normal.ebnf          # Original grammar with left recursion
├── grammar_enhanced.ebnf        # Enhanced grammar (left recursion removed, left factored)
├── lexer_parser.l              # Flex lexer specification for parser integration
├── token.h                     # Token type definitions
├── ParseTree.h                 # Parse tree node structures
├── Parser.h                    # Parser class declaration
├── Parser.cpp                  # Parser implementation (recursive descent)
├── main.cpp                    # Main program
├── Makefile                    # Build configuration
├── shell.nix                   # NixOS development environment
└── tests/
    └── test_parser.c           # Sample test program
```

## Grammar Transformations

The enhanced grammar (`grammar_enhanced.ebnf`) has been transformed from the original grammar with:

### Left Recursion Removed
- `declaration-list` → `declaration-list'`
- `param-list` → `param-list'`
- `statement-list` → `statement-list'`
- `expression` → `expression'`
- `additive-expression` → `additive-expression'`
- `term` → `term'`

### Left Factoring Applied
- `var-declaration` → `var-declaration'`
- `param` → `param'`
- `selection-stmt` → `selection-stmt'`
- `var` → `var'`

## Setup and Building

### Option 1: Using Nix Shell (Recommended for NixOS)

```bash
# Enter the development environment
nix-shell

# Build the parser
make

# Test the parser
make test
```

### Option 2: Manual Installation on NixOS

```bash
# Install required packages
nix-env -iA nixpkgs.gcc nixpkgs.gnumake nixpkgs.flex nixpkgs.graphviz

# Build the parser
make

# Test the parser
make test
```

### Option 3: Ubuntu/Debian

```bash
# Install dependencies
sudo apt-get install build-essential flex graphviz

# Build the parser
make

# Test the parser
make test
```

## Usage

### Basic Usage

```bash
./parser <input_file> [output_dot_file]
```

Example:
```bash
./parser tests/test_parser.c parse_tree.dot
```

### Generate Parse Tree Visualization

```bash
# Parse and generate .dot file
./parser tests/test_parser.c parse_tree.dot

# Convert to PNG image
dot -Tpng parse_tree.dot -o parse_tree.png

# Or use the make target
make test-png
```

## Input File Format

Input programs must follow the C- grammar syntax:

```c
Program ProgramName {
    // Variable declarations
    int x;
    int arr[10];
    float y;

    // Statements
    x = 5;

    if (x < 10) {
        x = x + 1;
    }

    while (x < 100) {
        x = x * 2;
    }
}.
```

**Important:**
- Programs must start with `Program` keyword followed by an identifier
- Programs must end with `}.` (closing brace and dot)
- Only variable declarations are supported (no function declarations in this grammar)

## Parser Output

### Success Case
```
=============================================================
           Parser for C- Language (Enhanced Grammar)
=============================================================

Input file: tests/test_parser.c
Output file: parse_tree.dot

=============================================================
                  PARSING SUCCESSFUL
=============================================================

Parse tree saved to: parse_tree.dot
To visualize: dot -Tpng parse_tree.dot -o parse_tree.png
```

### Error Case
```
=============================================================
           Parser for C- Language (Enhanced Grammar)
=============================================================

Input file: tests/bad_input.c
Output file: parse_tree.dot

=============================================================
                    PARSING FAILED
=============================================================

SYNTAX ERROR at Line 5, Col 12: Expected ';' but found '{'
```

## Implementation Details

### Parser Architecture
- **Type:** Recursive Descent Parser
- **Parsing Method:** Top-down, LL(1)-style
- **Grammar:** Enhanced grammar with no left recursion
- **Error Recovery:** First-error reporting (stops at first syntax error)

### Parse Tree
- **Format:** Graphviz DOT format
- **Node Types:**
  - Non-terminals: Grammar rule names (e.g., "expression", "statement")
  - Terminals: Token type + lexeme (e.g., "ID: x", "NUM: 42")
  - Epsilon: Empty productions marked as "ε"

### Files Generated
- `parse_tree.dot`: Graphviz representation of the parse tree
- `parse_tree.png`: Visual representation (when using `make test-png`)

## Makefile Targets

- `make` or `make all`: Build the parser
- `make clean`: Remove all generated files
- `make test`: Run parser on test file
- `make test-png`: Run parser and generate PNG visualization

## Error Handling

The parser reports:
1. **Lexical Errors:** Invalid characters, unclosed comments, malformed numbers/identifiers
2. **Syntax Errors:** Unexpected tokens, missing required tokens, malformed grammar structures

Errors include line and column numbers to help locate the problem in the source code.

## Supported Language Features

Based on the enhanced grammar, the parser supports:

- **Data Types:** `int`, `float`
- **Variables:** Simple variables and arrays
- **Operators:**
  - Arithmetic: `+`, `-`, `*`, `/`
  - Relational: `<`, `<=`, `>`, `>=`, `==`, `!=`
  - Assignment: `=`
- **Control Flow:**
  - `if` statements (with optional `else`)
  - `while` loops
- **Expressions:** Arithmetic and relational expressions with proper precedence
- **Arrays:** Array declarations and indexed access

## Assignment Compliance

This parser implements the requirements from Assignment 2:
- ✅ Grammar with left recursion removed
- ✅ Left factoring applied to eliminate common prefixes
- ✅ Recursive descent parser implementation
- ✅ Parse tree generation
- ✅ Graphviz output for visualization
- ✅ Error reporting with line/column information
- ✅ Integration with existing lexer

## Troubleshooting

### Parser won't compile
- Ensure you have gcc/g++, make, and flex installed
- On NixOS, use `nix-shell` or install packages with `nix-env`

### Graphviz visualization fails
- Install graphviz: `nix-env -iA nixpkgs.graphviz` (NixOS) or `sudo apt-get install graphviz` (Ubuntu)

### Input file parsing fails
- Verify your input follows the C- grammar syntax
- Check that the program starts with `Program ID {` and ends with `}.`
- Ensure all statements are within the program block
