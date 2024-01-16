#pragma once

#include "Parser/Token/Token.hpp"

class TokenNumber : public SimpleToken<int>
{
  public:
    TokenNumber();
    TokenNumber(int num);
    TokenNumber(const TokenNumber& token_space);
    ~TokenNumber();
    TokenNumber& operator=(const TokenNumber& token_space);
};
