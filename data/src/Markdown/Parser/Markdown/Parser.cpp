#include "Parser.hpp"

#include <memory>

#include "Parser/Tokenizer.hpp"
#include "Elements.hpp"

Parser::Parser(Tokenizer& tokenizer)
  : tokenizer_(tokenizer) {}

Parser::~Parser() {}

std::shared_ptr<Document> Parser::parse_document()
{
  std::shared_ptr<Document> document = CreateDocument();
  MarkdownElements  elements;
  while (is_healthy())
  {
    std::shared_ptr<MarkdownElement> section = parse_section();
    if (section)
    {
      document->add_element(section);
      continue;
    }
    elements = parse_body();
    if (!elements.empty())
    {
      document->add_elements(elements);
      continue;
    }
  }
  return document;
}

std::shared_ptr<Section> Parser::parse_section()
{
  std::istream::pos_type original_state = tokenizer_.snapshot();
  std::shared_ptr<Title> title = parse_title();
  if (!title)
    return nullptr;
  Parser::MarkdownElements body = parse_body();
  if (body.empty())
  {
    tokenizer_.rollback(original_state);
    return nullptr;
  }
  std::shared_ptr<Section> section = CreateSection(title);
  section->add_elements(body);
  return section;
}

Parser::MarkdownElements Parser::parse_body()
{
  MarkdownElements  elements;
  // TODO: implementation
  return elements;
}
