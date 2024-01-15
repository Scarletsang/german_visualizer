#pragma once

#include <string>

#include "Parser/Token/Token.hpp"

class TokenWord : public SimpleToken<std::string>
{
  public:
    TokenWord();
    TokenWord(const std::string& token_word);
    TokenWord(const TokenWord& token_word);
    ~TokenWord();
    TokenWord& operator=(const TokenWord& token_word);
};
