#ifndef MARKDOWN_PARSER_HPP
#define MARKDOWN_PARSER_HPP

#include <string>
#include <vector>

#include "Markdown.hpp"
#include "libcpp/Result.hpp"
#include "libcpp/Parser.hpp"
#include "libcpp/parser/ErrorTrace.hpp"

class Error
{

};

class MarkdownParser : public libcpp::Parser
{
  public:
    typedef libcpp::ErrorTrace<Error> ErrorTrace;
    typedef libcpp::Result<Markdown*, ErrorTrace> ParseResult;

    MarkdownParser(std::istream& input);
    ~MarkdownParser();

    ParseResult parse();
  
  private:
    ParseResult parseBlock(Markdown* markdown);
    ParseResult parseHeader(Markdown* markdown);
    ParseResult parseParagraph(Markdown* markdown);
    ParseResult parseLine(Markdown* markdown);
    ParseResult parseWord(Markdown* markdown);
};

struct Json {};

#endif
