#include "main.cpp"
#include <vector>
#include <iostream>

void Parser::parse(std::vector<Token>& tokens){

    current = 0;

    start = 0;

    tokensPtr = &tokens;

    while(!is_end()){

        Token token = advance();

        switch (token.type)

        {

        case TOKEN_TRUE:

            std::cout << "true" << std::endl;

            break;

        case TOKEN_FALSE:

            std::cout << "false" << std::endl;

            break;

        case TOKEN_NIL:

            std::cout << "nil" << std::endl;

            break;

        default: return;

        }

    }

}

const Token& Parser::advance(){

    return (*tokensPtr)[current++];

}

bool Parser::is_end(){

    return current > (*tokensPtr).size();

}