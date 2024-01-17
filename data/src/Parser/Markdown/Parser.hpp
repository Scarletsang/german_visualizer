#pragma once

#include <memory>
#include <vector>
#include <utility>

#include "Common/Result.hpp"
#include "Elements.hpp"
#include "Parser/Tokenizer.hpp"
#include "Parser/Markdown/Parser/Error.h"

class MarkdownParser
{
  public:
    using MarkdownElements = std::vector<std::shared_ptr<MarkdownElement> >;
    using Level = int;
    
    template <typename T>
    using ParserResult = Result<std::shared_ptr<T>, MarkdownParserError>;

    MarkdownParser(Tokenizer& tokenizer);
    MarkdownParser(const MarkdownParser& object) = delete;
    virtual ~MarkdownParser();
    MarkdownParser& operator=(const MarkdownParser& other) = delete;

    std::shared_ptr<Document> parse_document();
    ParserResult<Section>  parse_section(Level min_level);
    ParserResult<Title>  parse_title(Level min_level);

    std::shared_ptr<MarkdownElement> parse_element(Level min_level);
    std::shared_ptr<MarkdownElement> parse_element_delimiter(Level min_level);
    int peek_element_end(bool consume);

    std::shared_ptr<Paragraph>  parse_paragraph();
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
