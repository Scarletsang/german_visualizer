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
  while (is_healthy())
  {
    std::shared_ptr<MarkdownElement> element = parse_element_delimiter();
    if (element)
      elements.push_back(element);
  }
  return elements;
}

std::shared_ptr<Title>  Parser::parse_title()
{
  std::istream::pos_type original_state = tokenizer_.snapshot();
  std::unique_ptr<TokenSymbol> hashes = tokenizer_.tokenize_symbol();
  if (hashes->data() != '#')
  {
    tokenizer_.rollback(original_state);
    return nullptr;
  }
  std::shared_ptr<Paragraph> paragraph = parse_paragraph();
  if (!paragraph)
  {
    tokenizer_.rollback(original_state);
    return nullptr;
  }
  if (ignore_element_end() == EXIT_FAILURE)
  {
    tokenizer_.rollback(original_state);
    return nullptr;
  }
  return CreateTitle(hashes->level(), paragraph);
}

std::shared_ptr<Paragraph>  Parser::parse_paragraph()
{
  std::shared_ptr<Paragraph>  paragraph = CreateParagraph();
  while (is_healthy())
  {
    std::shared_ptr<Sentence> sentence = parse_sentence();
    if (!sentence)
      break;
    paragraph->add_element(sentence);
  }
  if (paragraph->data().empty())
    return nullptr;
  return paragraph;
}

std::shared_ptr<MarkdownElement> Parser::parse_element()
{
  {
    std::shared_ptr<Paragraph> paragraph = parse_paragraph();
    if (paragraph)
    {
      if (paragraph->data().size() == 1)
        return paragraph->data()[0];
      return paragraph;
    }
  }
  {
    std::shared_ptr<Word> word = parse_word();
    if (word)
    {
      if (word->data().size() == 1)
        return std::make_shared<Character>(word->data()[0]);
      return word;
    }
  }
  {
    std::shared_ptr<Number> number = parse_number();
    if (number)
      return number;
  }
  return nullptr;
}

std::shared_ptr<MarkdownElement> Parser::parse_element_delimiter()
{
  std::istream::pos_type original_state = tokenizer_.snapshot();
  std::shared_ptr<MarkdownElement> element = parse_element();
  if (!element)
    return nullptr;
  if (ignore_element_end() == EXIT_FAILURE)
  {
    tokenizer_.rollback(original_state);
    return nullptr;
  }
  return element;
}

std::shared_ptr<Sentence>   Parser::parse_sentence()
{
  std::istream::pos_type original_state = tokenizer_.snapshot();
  std::shared_ptr<MarkdownElement>  element = parse_sentence_element();
  if (!element)
    return nullptr;
  std::shared_ptr<Sentence> sentence = CreateSentence();
  while (is_healthy())
  {
    std::istream::pos_type second_state = tokenizer_.snapshot();
    std::shared_ptr<Delimiter>  space = parse_space();
    if (!space)
      break;
    element = parse_sentence_element();
    if (!element)
    {
      tokenizer_.rollback(second_state);
      break;
    }
    if (!space->data().empty())
      sentence->add_element(space);
    sentence->add_element(element);
  }
  if (sentence->data().empty())
  {
    tokenizer_.rollback(original_state);
    return nullptr;
  }
  return sentence;
}

std::shared_ptr<MarkdownElement>   Parser::parse_sentence_element()
{
  {
    std::shared_ptr<Word> word = parse_word();
    if (word)
      return word;
  }
  {
    std::shared_ptr<Number> number = parse_number();
    if (number)
      return number;
  }
  {
    std::shared_ptr<Character> punctuation = parse_punctuation();
    if (punctuation)
      return punctuation;
  }
  return nullptr;
}
