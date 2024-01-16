#pragma once

#include <memory>
#include <istream>

#include "Parser/Tokens.hpp"

class Tokenizer
{
  public:
    Tokenizer(std::istream& stream);
    Tokenizer(const Tokenizer& tokenizer) = delete;
    virtual ~Tokenizer() {}
    Tokenizer& operator=(const Tokenizer& tokenizer) = delete;

    virtual std::unique_ptr<TokenCharacter> tokenize_character() = 0;
    virtual std::unique_ptr<TokenNumber> tokenize_number() = 0;
    virtual std::unique_ptr<TokenSpace> tokenize_space() = 0;
    virtual std::unique_ptr<TokenSymbol> tokenize_symbol() = 0;
    virtual std::unique_ptr<TokenWord> tokenize_word() = 0;

    const std::istream& stream() const;

    std::istream::pos_type snapshot() const;
    void  rollback(std::istream::pos_type position);
  
  protected:
    std::istream& stream_;
};
