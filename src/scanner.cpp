#include "scanner.h"
#include <fstream>
#include <sstream>
#include <cctype>
#include <cstdlib>
#include <iostream>

std::string Scanner::read_file_contents(const std::string& filename) {
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

std::vector<Token> Scanner::scan_tokens(const std::string& source) {
    std::vector<Token> tokens;
    int currentLine = 1;
    size_t i = 0;
    while(i < source.size()){
        char c = source[i];
        
        // Handle newlines.
        if(c == '\n'){
            currentLine++;
            i++;
            continue;
        }
        
        // Number literal.
        if(std::isdigit(c)){
            size_t start = i;
            while(i < source.size() && std::isdigit(source[i])){
                i++;
            }
            // Check for a fractional part.
            if(i < source.size() && source[i] == '.' &&
               (i + 1 < source.size() && std::isdigit(source[i+1]))){
                i++; // consume '.'
                while(i < source.size() && std::isdigit(source[i])){
                    i++;
                }
            }
            std::string lexeme = source.substr(start, i - start);
            double num = std::stod(lexeme);
            std::ostringstream oss;
            oss << num;
            std::string literal = oss.str();
            if(literal.find('.') == std::string::npos) {
                literal += ".0";
            }
            tokens.push_back(Token{TOKEN_NUMBER, lexeme, literal, currentLine});
            continue;
        }
        
        // Plus operator.
        if(c == '+'){
            tokens.push_back(Token{TOKEN_PLUS, "+", "null", currentLine});
            i++;
            continue;
        }
        
        // Skip any whitespace.
        if(isspace(static_cast<unsigned char>(c))){
            i++;
            continue;
        }
        
        // If we reach here, it's an unexpected character.
        std::cerr << "[line " << currentLine << "] Error: Unexpected character: " << c << std::endl;
        has_error = true;
        i++;
    }
    
    tokens.push_back(Token{TOKEN_EOF, "", "null", currentLine});
    return tokens;
}
