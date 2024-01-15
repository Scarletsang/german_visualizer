#include "Space.hpp"

TokenSpace::TokenSpace()
  : SimpleToken<std::string>() {}

TokenSpace::TokenSpace(const std::string& token_space)
  : SimpleToken<std::string>(token_space) {}

TokenSpace::TokenSpace(const TokenSpace& token_space)
  : SimpleToken<std::string>(token_space) {}

TokenSpace::~TokenSpace() {}

TokenSpace& TokenSpace::operator=(const TokenSpace& token_space)
{
  SimpleToken<std::string>::operator=(token_space);
  return *this;
}
