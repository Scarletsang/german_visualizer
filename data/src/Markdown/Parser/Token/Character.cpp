#include "Character.hpp"

TokenCharacter::TokenCharacter()
  : SimpleToken<char>() {}

TokenCharacter::TokenCharacter(char c)
  : SimpleToken<char>(c) {}

TokenCharacter::TokenCharacter(const TokenCharacter& token_character)
  : SimpleToken<char>(token_character) {}

TokenCharacter::~TokenCharacter() {}

TokenCharacter& TokenCharacter::operator=(const TokenCharacter& token_character)
{
  SimpleToken<char>::operator=(token_character);
  return *this;
}
