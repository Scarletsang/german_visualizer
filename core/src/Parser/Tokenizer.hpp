#pragma once

#include <memory>
#include <istream>

#include "Parser/Tokens.hpp"

class Tokenizer
{
  public:
    static const char  kNoCriteria = -1;
    Tokenizer(std::istream& stream);
    Tokenizer(const Tokenizer& tokenizer) = delete;
    virtual ~Tokenizer() {}
    Tokenizer& operator=(const Tokenizer& tokenizer) = delete;

    virtual std::unique_ptr<TokenCharacter> tokenize_character(char c = kNoCriteria) = 0;
    virtual std::unique_ptr<TokenCharacter> tokenize_punctuation(char c = kNoCriteria) = 0;
    virtual std::unique_ptr<TokenCharacter> tokenize_sentence_end() = 0;

    virtual std::unique_ptr<TokenNumber> tokenize_number() = 0;
    virtual std::unique_ptr<TokenSpace> tokenize_space() = 0;
    virtual std::unique_ptr<TokenSymbol> tokenize_symbol() = 0;
    virtual std::unique_ptr<TokenWord> tokenize_word() = 0;
    virtual int ignore_dominant_space() = 0;
    virtual int ignore_newline() = 0;

    virtual const std::istream& stream() const;

    virtual std::streampos snapshot() const;
    virtual void  rollback(std::streampos position);
    virtual void  clear();
    virtual void  debug();
  
  protected:
    std::istream& stream_;
};
