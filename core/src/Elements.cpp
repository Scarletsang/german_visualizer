#include "Elements.hpp"

#include <string>
#include <memory>

#include "Ast/Atom/Character.hpp"
#include "Ast/Atom/Delimiter.hpp"
#include "Ast/Atom/Word.hpp"
#include "Ast/Atom/Number.hpp"
#include "Ast/DomCollection/Paragraph.hpp"
#include "Ast/DomCollection/Document.hpp"
#include "Ast/DomCollection/Section.hpp"
#include "Ast/DomCollection/Sentence.hpp"
#include "Ast/Title.hpp"

//////////////////////////////////
////////////   Atom   ////////////
//////////////////////////////////

std::shared_ptr<Character>  CreateCharacter(char c)
{
  return std::make_shared<Character>(c);
}

std::shared_ptr<Word>       CreateWord(const std::string& word)
{
  return std::make_shared<Word>(word);
}

std::shared_ptr<Delimiter>  CreateDelimiter(const std::string& word)
{
  return std::make_shared<Delimiter>(word);
}

std::shared_ptr<Number>     CreateNumber(int number)
{
  return std::make_shared<Number>(number);
}

std::shared_ptr<Number>     CreateNumber(float number)
{
  return std::make_shared<Number>(number);
}

///////////////////////////////////////////
////////////   DomCollection   ////////////
///////////////////////////////////////////

std::shared_ptr<Title>      CreateTitle(int level, std::shared_ptr<Paragraph> paragraph)
{
  return std::make_shared<Title>(level, paragraph);
}

std::shared_ptr<Sentence>   CreateSentence()
{
  return std::make_shared<Sentence>();
}

std::shared_ptr<Paragraph>  CreateParagraph()
{
  return std::make_shared<Paragraph>();
}

std::shared_ptr<Section>    CreateSection(std::shared_ptr<Title> title)
{
  return std::make_shared<Section>(title);
}

std::shared_ptr<Document>   CreateDocument()
{
  return std::make_shared<Document>();
}
