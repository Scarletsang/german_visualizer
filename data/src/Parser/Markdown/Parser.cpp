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
    ParserResult<Section> section = parse_section(0);
    if (section)
    {
      document->add_element(section.value());
      continue ;
    }
    element = parse_element_delimiter(0);
    if (element)
    {
      document->add_element(element);
      continue;
    }
    break;
  }
  return document;
}

MarkdownParser::ParserResult<Section> MarkdownParser::parse_section(Level min_level)
{
  tokenizer_.clear();
  ParserResult<Title> title = parse_title(min_level);
  if (!title)
    return ParserResult<Section>::Error(title.error());
  peek_element_end(true);
  MarkdownParser::MarkdownElements body;
  while (!is_eof())
  {
    std::shared_ptr<MarkdownElement> element = parse_element_delimiter(title.value()->level());
    if (element)
      body.push_back(element);
    else
      break;
  }
  std::shared_ptr<Section> section = CreateSection(title.value());
  section->add_elements(body);
  return section;
}

MarkdownParser::ParserResult<Title>  MarkdownParser::parse_title(Level min_level)
{
  std::istream::pos_type original_state = tokenizer_.snapshot();
  std::unique_ptr<TokenSymbol> hashes = tokenizer_.tokenize_symbol();
  if (!hashes || (hashes->data() != '#'))
  {
    tokenizer_.rollback(original_state);
    return ParserResult<Title>::Error(MarkdownParserError::kSyntaxError);
  }
  else if ((hashes->level() <= min_level))
  {
    tokenizer_.rollback(original_state);
    return ParserResult<Title>::Error(MarkdownParserError::kTooLowLevel);
  }
  std::shared_ptr<Paragraph> paragraph = parse_paragraph();
  if (!paragraph)
  {
    tokenizer_.rollback(original_state);
    return ParserResult<Title>::Error(MarkdownParserError::kSyntaxError);
  }
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
      break;
  }
  if (paragraph->data().empty())
    return nullptr;
  return paragraph;
}

std::shared_ptr<MarkdownElement> MarkdownParser::parse_element(Level min_level)
{
  tokenizer_.clear();
  {
    ParserResult<Section> section = parse_section(min_level);
    if (section)
      return section.value();
    else if (section.error() == MarkdownParserError::kTooLowLevel)
      return nullptr;
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

std::shared_ptr<MarkdownElement> MarkdownParser::parse_element_delimiter(Level min_level)
{
  std::istream::pos_type original_state = tokenizer_.snapshot();
  std::shared_ptr<MarkdownElement> element = parse_element(min_level);
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
  if (fullstop)
  {
    sentence->add_element(fullstop);
    return sentence;
  }
  else if (!sentence->data().empty())
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
