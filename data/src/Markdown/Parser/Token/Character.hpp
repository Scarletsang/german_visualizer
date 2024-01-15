#pragma once

#include "Parser/Token/Token.hpp"

class TokenCharacter : public SimpleToken<char>
{
  public:
    TokenCharacter();
    TokenCharacter(char c);
    TokenCharacter(const TokenCharacter& token_character);
    ~TokenCharacter();
    TokenCharacter& operator=(const TokenCharacter& token_character);
};
