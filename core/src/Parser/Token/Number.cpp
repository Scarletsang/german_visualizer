#include "Number.hpp"

TokenNumber::TokenNumber()
  : SimpleToken<int>() {}

TokenNumber::TokenNumber(int num)
  : SimpleToken<int>(num) {}

TokenNumber::TokenNumber(const TokenNumber& token_space)
  : SimpleToken<int>(token_space) {}

TokenNumber::~TokenNumber() {}

TokenNumber& TokenNumber::operator=(const TokenNumber& token_space)
{
  SimpleToken<int>::operator=(token_space);
  return *this;
}
