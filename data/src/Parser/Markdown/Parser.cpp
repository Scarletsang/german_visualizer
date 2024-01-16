#include "Parser.hpp"

#include <memory>

#include "Parser/Tokenizer.hpp"
#include "Elements.hpp"

MarkdownParser::MarkdownParser(Tokenizer& tokenizer)
  : tokenizer_(tokenizer) {}

MarkdownParser::~MarkdownParser() {}

std::shared_ptr<Document> MarkdownParser::parse_document()
{
  std::shared_ptr<Document> document = CreateDocument();
  std::shared_ptr<MarkdownElement>  element;
  while (!is_eof())
  {
    std::shared_ptr<MarkdownElement> section = parse_section();
    if (section)
    {
      document->add_element(section);
      continue;
    }
    element = parse_element_delimiter();
    if (element)
    {
      document->add_element(element);
      continue;
    }
    break;
  }
  return document;
}

std::shared_ptr<Section> MarkdownParser::parse_section()
{
  std::cout << "what: ";
  tokenizer_.debug();
  std::istream::pos_type original_state = tokenizer_.snapshot();
  std::shared_ptr<Title> title = parse_title();
  if (!title)
  {
    std::cout << "what failed 1" << std::endl;
    return nullptr;
  }
  MarkdownParser::MarkdownElements body = parse_body(title->level());
  if (body.empty())
  {
    std::cout << "what failed 2" << std::endl;
    tokenizer_.rollback(original_state);
    return nullptr;
  }
  std::shared_ptr<Section> section = CreateSection(title);
  section->add_elements(body);
  std::cout << "what end: ";
  tokenizer_.debug();
  return section;
}

MarkdownParser::MarkdownElements MarkdownParser::parse_body(int level)
{
  MarkdownElements  elements;
  while (!is_eof())
  {
    std::istream::pos_type original_state = tokenizer_.snapshot();
    std::shared_ptr<MarkdownElement> element = parse_element_delimiter();
    if (element)
    {
      std::shared_ptr<Section> section = std::dynamic_pointer_cast<Section>(element);
      if (section && section->title()->level() <= level)
      {
        tokenizer_.rollback(original_state);
        return elements;
      }
      elements.push_back(element);
    }
    else
      break;
  }
  return elements;
}

std::shared_ptr<Title>  MarkdownParser::parse_title()
{
  std::cout << "title: ";
  tokenizer_.debug();
  std::istream::pos_type original_state = tokenizer_.snapshot();
  std::unique_ptr<TokenSymbol> hashes = tokenizer_.tokenize_symbol();
  if (!hashes || hashes->data() != '#')
  {
    std::cout << "title failed 1" << std::endl;
    tokenizer_.rollback(original_state);
    return nullptr;
  }
  if (!parse_space())
  {
    std::cout << "title failed 2" << std::endl;
    tokenizer_.rollback(original_state);
    return nullptr;
  }
  std::shared_ptr<Paragraph> paragraph = parse_paragraph();
  if (!paragraph)
  {
    std::cout << "title failed 3" << std::endl;
    tokenizer_.rollback(original_state);
    return nullptr;
  }
  std::cout << "title end: ";
  tokenizer_.debug();
  return CreateTitle(hashes->level(), paragraph);
}

std::shared_ptr<Paragraph>  MarkdownParser::parse_paragraph()
{
  std::shared_ptr<Paragraph>  paragraph = CreateParagraph();
  while (!is_eof())
  {
    std::shared_ptr<Sentence> sentence = parse_sentence();
    if (!sentence)
      break;
    paragraph->add_element(sentence);
    if (peek_element_end(false) == EXIT_SUCCESS)
    {
      std::cout << "no way" << std::endl;
      break;
    }
  }
  std::cout << "p" << paragraph->data().size() << ": ";
  tokenizer_.debug();
  if (paragraph->data().empty())
    return nullptr;
  return paragraph;
}

std::shared_ptr<MarkdownElement> MarkdownParser::parse_element()
{
  {
    std::shared_ptr<Section> section = parse_section();
    if (section)
      return section;
  }
  {
    std::shared_ptr<Paragraph> paragraph = parse_paragraph();
    if (paragraph)
    {
      if (paragraph->data().size() == 1)
      {
        if (paragraph->data()[0]->data().size() == 1)
          return paragraph->data()[0]->data()[0];
        return paragraph->data()[0];
      }
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

std::shared_ptr<MarkdownElement> MarkdownParser::parse_element_delimiter()
{
  std::istream::pos_type original_state = tokenizer_.snapshot();
  std::shared_ptr<MarkdownElement> element = parse_element();
  if (!element)
    return nullptr;
  if (peek_element_end(true) == EXIT_FAILURE)
  {
    tokenizer_.rollback(original_state);
    return nullptr;
  }
  return element;
}

int MarkdownParser::ignore_eof()
{
  std::istream::pos_type original_state = tokenizer_.snapshot();
  if (!tokenizer_.tokenize_character())
    return EXIT_SUCCESS;
  tokenizer_.rollback(original_state);
  return EXIT_FAILURE;
}

int  MarkdownParser::peek_element_end(bool consume)
{
  std::istream::pos_type original_state = tokenizer_.snapshot();
  if (ignore_eof() == EXIT_SUCCESS)
  {
    if (!consume)
      tokenizer_.rollback(original_state);
    return EXIT_SUCCESS;
  }
  if (ignore_newline() == EXIT_SUCCESS)
  {
    while (ignore_newline() == EXIT_SUCCESS)
      ;
    if (!consume)
      tokenizer_.rollback(original_state);
    return EXIT_SUCCESS;
  }
  tokenizer_.rollback(original_state);
  return EXIT_FAILURE;
}

std::shared_ptr<Sentence>   MarkdownParser::parse_sentence()
{
  std::istream::pos_type original_state = tokenizer_.snapshot();
  std::shared_ptr<Sentence> sentence = CreateSentence();
  std::shared_ptr<MarkdownElement>  element;
  std::istream::pos_type second_state = tokenizer_.snapshot();
  std::shared_ptr<Delimiter>  space;

  while (!is_eof())
  {
    space = parse_space();
    if (space && !space->data().empty())
    {
      sentence->add_element(space);
      second_state = tokenizer_.snapshot();
      continue;
    }
    element = parse_sentence_element();
    if (!element)
    {
      tokenizer_.rollback(second_state);
      break;
    }
    sentence->add_element(element);
    second_state = tokenizer_.snapshot();
  }
  std::shared_ptr<Character> fullstop = parse_fullstop();
  std::cout << "sentence: " << (fullstop ? fullstop->data() : '8') << std::endl;
  tokenizer_.debug();
  if (fullstop)
  {
    sentence->add_element(fullstop);
    return sentence;
  }
  if (!sentence->data().empty())
    return sentence;
  tokenizer_.rollback(original_state);
  return nullptr;
}

std::shared_ptr<MarkdownElement>   MarkdownParser::parse_sentence_element()
{
  {
    std::shared_ptr<Character> punctuation = parse_punctuation();
    if (punctuation)
      return punctuation;
  }
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
  return nullptr;
}

std::shared_ptr<Word> MarkdownParser::parse_word()
{
  std::unique_ptr<TokenWord> token_word = tokenizer_.tokenize_word();
  if (token_word)
    return CreateWord(token_word->data());
  return nullptr;
}

std::shared_ptr<Number> MarkdownParser::parse_number()
{
  std::unique_ptr<TokenNumber> token_number = tokenizer_.tokenize_number();
  if (token_number)
    return CreateNumber(token_number->data());
  return nullptr;
}

std::shared_ptr<Delimiter> MarkdownParser::parse_space()
{
  std::unique_ptr<TokenSpace> token_space = tokenizer_.tokenize_space();
  if (token_space)
  {
    if (token_space->data().empty())
      return std::shared_ptr<Delimiter>(new Delimiter());
    return CreateDelimiter(token_space->data());
  }
  return nullptr;
}

int MarkdownParser::ignore_newline()
{
  return tokenizer_.ignore_newline();
}

std::shared_ptr<Character>  MarkdownParser::parse_punctuation()
{
  std::unique_ptr<TokenCharacter> token_punctuation = tokenizer_.tokenize_punctuation();
  if (token_punctuation)
    return CreateCharacter(token_punctuation->data());
  return nullptr;
}

std::shared_ptr<Character>  MarkdownParser::parse_fullstop()
{
  std::unique_ptr<TokenCharacter> token_punctuation = tokenizer_.tokenize_sentence_end();
  if (token_punctuation)
    return CreateCharacter(token_punctuation->data());
  std::cout << "fullstop failed" << std::endl;
  return nullptr;
}

std::shared_ptr<Character>  MarkdownParser::parse_character()
{
  std::unique_ptr<TokenCharacter> token_character = tokenizer_.tokenize_character();
  if (token_character)
    return CreateCharacter(token_character->data());
  return nullptr;
}

bool  MarkdownParser::is_healthy() const
{
  return tokenizer_.stream().good();
}

bool  MarkdownParser::is_eof() const
{
  return tokenizer_.stream().eof();
}
