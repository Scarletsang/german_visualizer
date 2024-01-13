#include "Elements.hpp"

#include <string>
#include <memory>

#include "Element/Atom/Character.hpp"
#include "Element/Atom/Word.hpp"
#include "Element/Atom/Number.hpp"
#include "Element/Collection/Paragraph.hpp"
#include "Element/Collection/Document.hpp"
#include "Element/Collection/Section.hpp"
#include "Element/Collection/Sentence.hpp"
#include "Element/Title.hpp"

std::shared_ptr<Character>  CreateCharacter(char c)
{
  return std::make_shared<Character>(c);
}

std::shared_ptr<Word>       CreateWord(const std::string& word)
{
  return std::make_shared<Word>(word);
}

std::shared_ptr<Number>     CreateNumber(int number)
{
  return std::make_shared<Number>(number);
}

std::shared_ptr<Number>     CreateNumber(float number)
{
  return std::make_shared<Number>(number);
}

// // composite
std::shared_ptr<Sentence>   CreateSentence()
{
  return std::make_shared<Sentence>();
}

std::shared_ptr<Title>      CreateTitle(int level, std::shared_ptr<Paragraph> paragraph)
{
  return std::make_shared<Title>(level, paragraph);
}

// // collection
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
