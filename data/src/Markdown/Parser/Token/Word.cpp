#include "Word.hpp"

TokenWord::TokenWord()
  : SimpleToken<std::string>() {}

TokenWord::TokenWord(const std::string& token_word)
  : SimpleToken<std::string>(token_word) {}

TokenWord::TokenWord(const TokenWord& token_word)
  : SimpleToken<std::string>(token_word) {}

TokenWord::~TokenWord() {}

TokenWord& TokenWord::operator=(const TokenWord& token_word)
{
  SimpleToken<std::string>::operator=(token_word);
  return *this;
}
