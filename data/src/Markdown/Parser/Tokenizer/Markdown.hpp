#pragma once

#include <memory>
#include <istream>

#include "Parser/Tokens.hpp"
#include "Parser/Tokenizer.hpp"

class MarkdownTokenizer : public Tokenizer
{
  public:
    MarkdownTokenizer(std::istream& input_stream);
    MarkdownTokenizer(const MarkdownTokenizer& markdown_tokenizer) = delete;
    ~MarkdownTokenizer();
    MarkdownTokenizer& operator=(const MarkdownTokenizer& markdown_tokenizer) = delete;

    virtual std::unique_ptr<TokenCharacter> tokenize_character() override;
    virtual std::unique_ptr<TokenNumber> tokenize_number() override;
    virtual std::unique_ptr<TokenSpace> tokenize_space() override;
    virtual std::unique_ptr<TokenSymbol> tokenize_symbol() override;
    virtual std::unique_ptr<TokenWord> tokenize_word() override;
    virtual std::unique_ptr<Token> tokenize() override;
};
