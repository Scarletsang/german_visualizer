#pragma once

#include <string>

#include "Parser/Token/Token.hpp"

class TokenSpace : public SimpleToken<std::string>
{
  public:
    TokenSpace();
    TokenSpace(const std::string& token_space);
    TokenSpace(const TokenSpace& token_space);
    ~TokenSpace();
    TokenSpace& operator=(const TokenSpace& token_space);
};
