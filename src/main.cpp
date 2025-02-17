#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <cctype>
#include <vector>

std::string read_file_contents(const std::string& filename);

int main(int argc, char *argv[]) {
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;
    
    if (argc < 3) {
        std::cerr << "Usage: ./your_program tokenize <filename>" << std::endl;
        return 1;
    }
    
    const std::string command = argv[1];
    
    if (command != "tokenize") {
        std::cerr << "Unknown command: " << command << std::endl;
        return 1;
    }
    
    std::string file_contents = read_file_contents(argv[2]);
    
    // Vectors to hold error messages and token output lines.
    std::vector<std::string> errors;
    std::vector<std::string> tokens;
    
    int currentLine = 1;
    size_t i = 0;
    while (i < file_contents.size()) {
        char c = file_contents[i];
        
        // Handle newline: update line number and move on.
        if (c == '\n') {
            currentLine++;
            i++;
            continue;
        }
        
        // Skip over comments starting with "//"
        if (c == '/' && i + 1 < file_contents.size() && file_contents[i+1] == '/') {
            i += 2; // Skip the "//"
            // Skip until the end of line or file.
            while (i < file_contents.size() && file_contents[i] != '\n') {
                i++;
            }
            continue; // Newline will be processed in the next iteration.
        }
        
        // Process tokens and possible two-character operators.
        switch (c) {
            case '=':
                if (i + 1 < file_contents.size() && file_contents[i+1] == '=') {
                    tokens.push_back("EQUAL_EQUAL == null");
                    i += 2;
                } else {
                    tokens.push_back("EQUAL = null");
                    i++;
                }
                break;
            case '!':
                if (i + 1 < file_contents.size() && file_contents[i+1] == '=') {
                    tokens.push_back("BANG_EQUAL != null");
                    i += 2;
                } else {
                    tokens.push_back("BANG ! null");
                    i++;
                }
                break;
            case '<':
                if (i + 1 < file_contents.size() && file_contents[i+1] == '=') {
                    tokens.push_back("LESS_EQUAL <= null");
                    i += 2;
                } else {
                    tokens.push_back("LESS < null");
                    i++;
                }
                break;
            case '>':
                if (i + 1 < file_contents.size() && file_contents[i+1] == '=') {
                    tokens.push_back("GREATER_EQUAL >= null");
                    i += 2;
                } else {
                    tokens.push_back("GREATER > null");
                    i++;
                }
                break;
            case '/':
                // Already handled comment case above.
                tokens.push_back("SLASH / null");
                i++;
                break;
            case '(':
                tokens.push_back("LEFT_PAREN ( null");
                i++;
                break;
            case ')':
                tokens.push_back("RIGHT_PAREN ) null");
                i++;
                break;
            case '{':
                tokens.push_back("LEFT_BRACE { null");
                i++;
                break;
            case '}':
                tokens.push_back("RIGHT_BRACE } null");
                i++;
                break;
            case '*':
                tokens.push_back("STAR * null");
                i++;
                break;
            case '+':
                tokens.push_back("PLUS + null");
                i++;
                break;
            case '.':
                tokens.push_back("DOT . null");
                i++;
                break;
            case ',':
                tokens.push_back("COMMA , null");
                i++;
                break;
            case '-':
                tokens.push_back("MINUS - null");
                i++;
                break;
            case ';':
                tokens.push_back("SEMICOLON ; null");
                i++;
                break;
            default:
                // Skip whitespace characters.
                if (isspace(static_cast<unsigned char>(c))) {
                    i++;
                } else {
                    // Record unexpected character error with the current line number.
                    errors.push_back("[line " + std::to_string(currentLine) + "] Error: Unexpected character: " + c);
                    i++;
                }
                break;
        }
    }
    
    for (const auto& err : errors) {
        std::cout << err << std::endl;
    }

    for (const auto& tok : tokens) {
        std::cout << tok << std::endl;
    }
    
    // Finally print the EOF token with two spaces.
    std::cout << "EOF  null" << std::endl;
    
    // Return 0 if no fatal errors occurred.
    return 0;
}

std::string read_file_contents(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error reading file: " << filename << std::endl;
        std::exit(1);
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    return buffer.str();
}
