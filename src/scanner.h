#ifndef SCANNER_H
#define SCANNER_H

#include <string>
#include <vector>

// Define token type constants (adjust these as needed).
const std::string TOKEN_NUMBER = "NUMBER";
const std::string TOKEN_PLUS   = "PLUS";
const std::string TOKEN_EOF    = "EOF";

// Token structure.
struct Token {
    std::string type;
    std::string lexeme;
    std::string literal;
    int line;
};

// A helper to convert token types to string for printing.
inline std::string tokenTypeToString(const std::string& type) {
    return type;
}

// Scanner class declaration.
class Scanner {
public:
    bool has_error = false;
    
    // Reads the contents of a file into a string.
    std::string read_file_contents(const std::string& filename);
    
    // Scans the given source and returns a vector of tokens.
    std::vector<Token> scan_tokens(const std::string& source);
};

#endif // SCANNER_H
