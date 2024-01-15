#include "Symbol.hpp"

TokenSymbol::TokenSymbol()
  : SimpleToken<char>(), level_(0) {}

TokenSymbol::TokenSymbol(char symbol)
  : SimpleToken<char>(symbol), level_(0) {}

TokenSymbol::TokenSymbol(char symbol, int level)
  : SimpleToken<char>(symbol), level_(level) {}

TokenSymbol::TokenSymbol(const TokenSymbol& token_symbol)
  : SimpleToken<char>(token_symbol), level_(token_symbol.level_) {}

TokenSymbol::~TokenSymbol() {}

TokenSymbol& TokenSymbol::operator=(const TokenSymbol& token_symbol)
{
  SimpleToken<char>::operator=(token_symbol);
  level_ = token_symbol.level_;
  return *this;
}

void TokenSymbol::set_level(int level)
{
  level_ = level;
}

int TokenSymbol::level() const
{
  return level_;
}
