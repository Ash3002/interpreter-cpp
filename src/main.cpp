#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <cctype>
#include <vector>

std::string read_file_contents(const std::string& filename);

int main(int argc, char* argv[]) {
    // Disable buffering.
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;
    
    if (argc < 3) {
        std::cerr << "Usage: ./your_program tokenize <filename>" << std::endl;
        return 1;
    }
    
    std::string command = argv[1];
    if (command != "tokenize") {
        std::cerr << "Unknown command: " << command << std::endl;
        return 1;
    }
    
    std::string file_contents = read_file_contents(argv[2]);
    
    // Vectors to hold errors and token messages.
    std::vector<std::string> errors;
    std::vector<std::string> tokens;
    
    int currentLine = 1;
    size_t i = 0;
    while (i < file_contents.size()) {
        char c = file_contents[i];
        
        // Handle newlines.
        if (c == '\n') {
            currentLine++;
            i++;
            continue;
        }
        
        // Skip line comments (starting with "//").
        if (c == '/' && i + 1 < file_contents.size() && file_contents[i + 1] == '/') {
            i += 2; // Skip the "//"
            while (i < file_contents.size() && file_contents[i] != '\n') {
                i++;
            }
            continue;
        }
        
        // Process tokens.
        switch (c) {
            case '"': {
                // Start of a string literal.
                size_t start = i; // record the starting quote index.
                i++; // Skip the opening quote.
                std::string literalContent;
                bool terminated = false;
                while (i < file_contents.size()) {
                    char ch = file_contents[i];
                    if (ch == '"') {
                        terminated = true;
                        break;
                    }
                    if (ch == '\n') {
                        currentLine++;
                    }
                    literalContent.push_back(ch);
                    i++;
                }
                if (!terminated) {
                    errors.push_back("[line " + std::to_string(currentLine) + "] Error: Unterminated string.");
                } else {
                    // The closing quote is at file_contents[i].
                    std::string lexeme = file_contents.substr(start, i - start + 1);
                    tokens.push_back("STRING " + lexeme + " " + literalContent);
                    i++; // Skip the closing quote.
                }
                break;
            }
            case '=':
                if (i + 1 < file_contents.size() && file_contents[i + 1] == '=') {
                    tokens.push_back("EQUAL_EQUAL == null");
                    i += 2;
                } else {
                    tokens.push_back("EQUAL = null");
                    i++;
                }
                break;
            case '!':
                if (i + 1 < file_contents.size() && file_contents[i + 1] == '=') {
                    tokens.push_back("BANG_EQUAL != null");
                    i += 2;
                } else {
                    tokens.push_back("BANG ! null");
                    i++;
                }
                break;
            case '<':
                if (i + 1 < file_contents.size() && file_contents[i + 1] == '=') {
                    tokens.push_back("LESS_EQUAL <= null");
                    i += 2;
                } else {
                    tokens.push_back("LESS < null");
                    i++;
                }
                break;
            case '>':
                if (i + 1 < file_contents.size() && file_contents[i + 1] == '=') {
                    tokens.push_back("GREATER_EQUAL >= null");
                    i += 2;
                } else {
                    tokens.push_back("GREATER > null");
                    i++;
                }
                break;
            case '/':
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
                // Skip whitespace.
                if (isspace(static_cast<unsigned char>(c))) {
                    i++;
                } else {
                    errors.push_back("[line " + std::to_string(currentLine) + "] Error: Unexpected character: " + c);
                    i++;
                }
                break;
        }
    }
    
    // Print errors to stderr.
    for (const auto& error : errors) {
        std::cerr << error << std::endl;
    }
    
    // Print tokens to stdout.
    for (const auto& token : tokens) {
        std::cout << token << std::endl;
    }
    
    // Always print the EOF token.
    std::cout << "EOF  null" << std::endl;
    
    // Return exit code 65 if there were any lexical errors.
    return errors.empty() ? 0 : 65;
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
