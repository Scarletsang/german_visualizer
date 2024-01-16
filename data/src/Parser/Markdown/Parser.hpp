#pragma once

#include <memory>
#include <vector>

#include "Parser/Tokenizer.hpp"
#include "Elements.hpp"

class MarkdownParser
{
  public:
    using MarkdownElements = std::vector<std::shared_ptr<MarkdownElement> >;
    MarkdownParser(Tokenizer& tokenizer);
    MarkdownParser(const MarkdownParser& object) = delete;
    virtual ~MarkdownParser();
    MarkdownParser& operator=(const MarkdownParser& other) = delete;

    std::shared_ptr<Document> parse_document();
    std::shared_ptr<Section>  parse_section();
    MarkdownElements parse_body(int level);
    std::shared_ptr<Title>    parse_title();
    std::shared_ptr<Paragraph>  parse_paragraph();

    std::shared_ptr<MarkdownElement> parse_element();
    std::shared_ptr<MarkdownElement> parse_element_delimiter();
    int  ignore_element_end();

    std::shared_ptr<Sentence>   parse_sentence();
    std::shared_ptr<MarkdownElement>  parse_sentence_element();

    std::shared_ptr<Word>       parse_word();
    std::shared_ptr<Number>     parse_number();
    // handle dominant space. A dominant space is an empty Delimiter
    std::shared_ptr<Delimiter>  parse_space();

    int  ignore_newline();
    int  ignore_eof();
    std::shared_ptr<Character>  parse_punctuation();
    std::shared_ptr<Character>  parse_fullstop();
    std::shared_ptr<Character>  parse_character();

    bool  is_healthy() const;
    bool  is_eof() const;

  private:
    Tokenizer& tokenizer_;
};
