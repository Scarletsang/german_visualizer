#include "Concret.hpp"
#include "Types.hpp"

///////////////////////////////////////
////////////   Character   ////////////
///////////////////////////////////////

Character::Character() {}

Character::Character(const char character) : character_(character) {}

Character::Character(const Character& other) : character_(other.character_) {}

Character::~Character() {}

MarkdownType  Character::get_type() const
{
  return kCharacter;
}

///////////////////////////////////////
////////////   Word   /////////////////
///////////////////////////////////////

Word::Word() {}

Word::Word(const std::string& word) : word_(word) {}

Word::Word(const Word& other) : word_(other.word_) {}

Word::~Word() {}

MarkdownType  Word::get_type() const
{
  return kWord;
}

///////////////////////////////////////
////////////   Title   ////////////////
///////////////////////////////////////

Title::Title() {}

Title::Title(const std::string& word) : title_(word), level_(1) {}

Title::Title(const std::string& word, int level) : title_(word), level_(level) {}

Title::Title(const Title& other)
  : title_(other.title_), level_(other.level_) {}

Title::~Title() {}

MarkdownType  Title::get_type() const
{
  return kTitle;
}
