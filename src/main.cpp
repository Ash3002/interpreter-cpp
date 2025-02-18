#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "scanner.h"
#include "parser.h"
#include "ast_printer.h"

int main(int argc, char* argv[]) {
    // Disable output buffering.
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;
    
    if (argc < 3) {
        std::cerr << "Usage: ./your_program <command> <filename>" << std::endl;
        return 1;
    }
    
    std::string command = argv[1];
    
    // Create a Scanner instance.
    Scanner scanner;
    std::string file_contents = scanner.read_file_contents(argv[2]); 
    std::vector<Token> tokens = scanner.scan_tokens(file_contents);
    
    if (command == "tokenize") {
        // Print tokens.
        for (size_t i = 0; i < tokens.size(); i++){
            std::string literal = tokens[i].literal.empty() ? "null" : tokens[i].literal;
            std::cout << tokenTypeToString(tokens[i].type) 
                      << " " << tokens[i].lexeme 
                      << " " << literal << std::endl;
        }
        return scanner.has_error ? 65 : 0;
    }
    else if (command == "parse"){
        if (scanner.has_error) {
            return 65;
        }
        // Parse tokens into an AST.
        Parser parser(tokens);
        auto ast = parser.parse();
        
        // Print the AST in prefix notation.
        AstPrinter printer;
        std::string astStr = printer.print(ast.get());
        std::cout << astStr << std::endl;
        return 0;
    } 
    else {
        std::cerr << "Unknown command: " << command << std::endl;
        return 1;
    }
}
