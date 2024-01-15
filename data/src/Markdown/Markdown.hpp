// #pragma once

// #include <string>
// #include <istream>
// #include <vector>
// #include <memory>

// #include "Element/Element.hpp"
// #include "Elements.hpp"
// #include "Parser/Error.hpp"

// #include "libcpp/Result.hpp"
// #include "libcpp/Parser.hpp"
// #include "libcpp/parser/ErrorTrace.hpp"

// class MarkdownParser : public libcpp::Parser
// {
//   public:
//     using ErrorTrace = libcpp::ErrorTrace<Error>;
//     using ParseTarget = std::shared_ptr<MarkdownElement>;
//     template <typename T>
//     using ParseResult = libcpp::Result<std::shared_ptr<T>, ErrorTrace>;

//     MarkdownParser(std::istream& input);
//     ~MarkdownParser();

//     ParseResult<Document> parse_document();

//   private:
//     ParseResult<Title>  parse_title(ParseTarget markdown);
//     ParseResult<Paragraph>  parse_paragraph(ParseTarget markdown);
//     ParseResult<Sentence> parse_sentence(ParseTarget markdown);
//     ParseResult<Word> parse_word(ParseTarget markdown);
//     ParseResult<Character>  parse_character(ParseTarget markdown);
//     ParseResult<Number> parse_number(ParseTarget markdown);
// };
