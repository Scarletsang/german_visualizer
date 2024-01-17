#pragma once

#include <string>

#include "Parser/Token/Token.hpp"

class TokenSymbol : public SimpleToken<char>
{
  public:
    TokenSymbol();
    TokenSymbol(char symbol);
    TokenSymbol(char symbol, int level);
    TokenSymbol(const TokenSymbol& token_symbol);
    ~TokenSymbol();
    TokenSymbol& operator=(const TokenSymbol& token_symbol);

    void  set_level(int level);
    int  level() const;
  protected:
    int level_;
};
