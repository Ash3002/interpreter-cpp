#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <cctype>
#include <vector>
#include <unordered_map>

std::string read_file_contents(const std::string& filename);

int main(int argc, char* argv[]) {
    // Disable output buffering.
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
        

        if (std::isdigit(c)) {
            size_t start = i;
            // Consume the integer part.
            while (i < file_contents.size() && std::isdigit(file_contents[i])) {
                i++;
            }
            // If a fractional part exists, consume it.
            if (i < file_contents.size() && file_contents[i] == '.' &&
                (i + 1 < file_contents.size() && std::isdigit(file_contents[i+1]))) {
                i++; // Consume the dot.
                while (i < file_contents.size() && std::isdigit(file_contents[i])) {
                    i++;
                }
            }
            std::string lexeme = file_contents.substr(start, i - start);
            
            // Convert the lexeme to a double and then back to a string to normalize it.
            double num = std::stod(lexeme);
            std::ostringstream oss;
            oss << num;
            std::string literal = oss.str();
            // Ensure there's a decimal point in the literal.
            if (literal.find('.') == std::string::npos) {
                literal += ".0";
            }
            
            tokens.push_back("NUMBER " + lexeme + " " + literal);
            continue;
        }
        
        // Skip line comments that start with "//".
        if (c == '/' && i + 1 < file_contents.size() && file_contents[i+1] == '/') {
            i += 2;
            while (i < file_contents.size() && file_contents[i] != '\n') {
                i++;
            }
            continue;
        }
        
        // Handle string literals.
        if (c == '"') {
            size_t start = i;  // Starting index of the lexeme (including the opening quote).
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
                // Include the closing quote in the lexeme.
                std::string lexeme = file_contents.substr(start, i - start + 1);
                tokens.push_back("STRING " + lexeme + " " + literalContent);
                i++; // Skip the closing quote.
            }
            continue;
        }
        // Check for identifiers (start with a letter or underscore).
        if (std::isalpha(c) || c == '_') {
            size_t start = i;
            while (i < file_contents.size() &&
                   (std::isalnum(file_contents[i]) || file_contents[i] == '_')) {
                i++;
            }
            std::string lexeme = file_contents.substr(start, i - start);
        
            // Map of reserved words and their corresponding token names.
            static const std::unordered_map<std::string, std::string> reservedWords = {
                {"and", "AND"},
                {"class", "CLASS"},
                {"else", "ELSE"},
                {"false", "FALSE"},
                {"for", "FOR"},
                {"fun", "FUN"},
                {"if", "IF"},       // Reserved word "if"
                {"nil", "NIL"},
                {"or", "OR"},
                {"print", "PRINT"},
                {"return", "RETURN"},
                {"super", "SUPER"},
                {"this", "THIS"},
                {"true", "TRUE"},
                {"var", "VAR"},
                {"while", "WHILE"}
            };
        
            // Check if the lexeme is a reserved word.
            auto it = reservedWords.find(lexeme);
            if (it != reservedWords.end()) {
                tokens.push_back(it->second + " " + lexeme + " null");
            } else {
                tokens.push_back("IDENTIFIER " + lexeme + " null");
            }
            continue;
        }
            
        
        // Process other tokens.
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
    
    // If any lexical errors occurred, return exit code 65.
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
