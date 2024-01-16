#pragma once

#include <memory>
#include <vector>

#include "Parser/Tokenizer.hpp"
#include "Elements.hpp"

class Parser
{
  public:
    using MarkdownElements = std::vector<std::shared_ptr<MarkdownElement> >;
    Parser(Tokenizer& tokenizer);
    Parser(const Parser& object) = delete;
    virtual ~Parser();
    Parser& operator=(const Parser& other) = delete;

    std::shared_ptr<Document> parse_document();
    std::shared_ptr<Section>  parse_section();
    MarkdownElements parse_body();
    std::shared_ptr<Title>    parse_title();
    std::shared_ptr<Paragraph>  parse_paragraph();
    std::shared_ptr<Sentence>   parse_sentence();

    std::shared_ptr<Word>       parse_word();
    std::shared_ptr<Number>     parse_number();
    std::shared_ptr<Delimiter>  parse_space();

    std::shared_ptr<Character>  parse_eof();
    std::shared_ptr<Character>  parse_newline();
    std::shared_ptr<Character>  parse_punctuation();
    std::shared_ptr<Character>  parse_fullstop();
    std::shared_ptr<Character>  parse_character();

    bool  is_healthy() const;
    bool  is_eof() const;

  private:
    Tokenizer& tokenizer_;
};
