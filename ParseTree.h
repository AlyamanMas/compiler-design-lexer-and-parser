#ifndef PARSETREE_H
#define PARSETREE_H

#include <string>
#include <vector>
#include <memory>
#include <fstream>

/* Parse tree node base class */
class ParseTreeNode {
public:
    std::string label;
    std::vector<std::shared_ptr<ParseTreeNode>> children;

    ParseTreeNode(const std::string& lbl) : label(lbl) {}

    virtual ~ParseTreeNode() {}

    void addChild(std::shared_ptr<ParseTreeNode> child) {
        if (child) {
            children.push_back(child);
        }
    }

    // Generate unique node ID for Graphviz
    static int nodeCounter;
    int nodeId;

    void assignIds() {
        nodeId = nodeCounter++;
        for (auto& child : children) {
            if (child) {
                child->assignIds();
            }
        }
    }

    // Output to Graphviz format
    void toGraphviz(std::ofstream& out) {
        // Output this node
        out << "  node" << nodeId << " [label=\"" << escapeLabel(label) << "\"];\n";

        // Output edges to children
        for (auto& child : children) {
            if (child) {
                out << "  node" << nodeId << " -> node" << child->nodeId << ";\n";
            }
        }

        // Recursively output children
        for (auto& child : children) {
            if (child) {
                child->toGraphviz(out);
            }
        }
    }

private:
    std::string escapeLabel(const std::string& str) {
        std::string result;
        for (char c : str) {
            if (c == '"' || c == '\\') {
                result += '\\';
            }
            result += c;
        }
        return result;
    }
};

/* Terminal node (leaf) */
class TerminalNode : public ParseTreeNode {
public:
    std::string lexeme;

    TerminalNode(const std::string& tokenType, const std::string& lex)
        : ParseTreeNode(tokenType + ": " + lex), lexeme(lex) {}
};

/* Non-terminal node */
class NonTerminalNode : public ParseTreeNode {
public:
    NonTerminalNode(const std::string& ruleName)
        : ParseTreeNode(ruleName) {}
};

/* Empty/Epsilon node */
class EpsilonNode : public ParseTreeNode {
public:
    EpsilonNode() : ParseTreeNode("ε") {}
};

#endif /* PARSETREE_H */
