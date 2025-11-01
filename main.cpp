#include "Parser.h"
#include <iostream>
#include <fstream>
#include <cstdio>

using namespace std;

void generateGraphviz(shared_ptr<ParseTreeNode> root, const string& filename) {
    ofstream out(filename);
    if (!out.is_open()) {
        cerr << "Error: Could not open file '" << filename << "' for writing\n";
        return;
    }

    // Assign unique IDs to all nodes
    ParseTreeNode::nodeCounter = 0;
    root->assignIds();

    // Write Graphviz header
    out << "digraph ParseTree {\n";
    out << "  node [shape=box, fontname=\"Arial\"];\n";
    out << "  edge [fontname=\"Arial\"];\n\n";

    // Generate nodes and edges
    root->toGraphviz(out);

    // Write footer
    out << "}\n";

    out.close();
    cout << "Parse tree saved to: " << filename << endl;
    cout << "To visualize: dot -Tpng " << filename << " -o parse_tree.png" << endl;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <input_file> [output_dot_file]\n";
        cerr << "Example: " << argv[0] << " tests/test_input.c parse_tree.dot\n";
        return 1;
    }

    string inputFile = argv[1];
    string outputFile = (argc >= 3) ? argv[2] : "parse_tree.dot";

    // Open input file
    FILE* file = fopen(inputFile.c_str(), "r");
    if (!file) {
        cerr << "Error: Cannot open file '" << inputFile << "'\n";
        return 1;
    }

    cout << "=============================================================\n";
    cout << "           Parser for C- Language (Enhanced Grammar)\n";
    cout << "=============================================================\n\n";
    cout << "Input file: " << inputFile << "\n";
    cout << "Output file: " << outputFile << "\n\n";

    // Set lexer input
    yyin = file;

    // Create parser and parse
    Parser parser;
    auto parseTree = parser.parse();

    fclose(file);

    // Check for errors
    if (parser.hadError() || !parseTree) {
        cout << "\n=============================================================\n";
        cout << "                    PARSING FAILED\n";
        cout << "=============================================================\n\n";
        cerr << parser.getErrorMessage() << endl;
        return 1;
    }

    // Success!
    cout << "=============================================================\n";
    cout << "                  PARSING SUCCESSFUL\n";
    cout << "=============================================================\n\n";

    // Generate Graphviz output
    generateGraphviz(parseTree, outputFile);

    return 0;
}
