{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
  buildInputs = with pkgs; [
    gcc
    gnumake
    flex
    bison
    graphviz
    pandoc
    texliveSmall
  ];

  shellHook = ''
    echo "C- Parser Development Environment"
    echo "=================================="
    echo "Available tools:"
    echo "  - gcc/g++: C/C++ compiler"
    echo "  - make: Build automation"
    echo "  - flex: Lexical analyzer generator"
    echo "  - graphviz: Visualization tool for parse trees"
    echo ""
    echo "To build the parser, run: make"
    echo "To test the parser, run: make test"
    echo ""
  '';
}
