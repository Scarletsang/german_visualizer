#pragma once

#include <iostream>

// atom
class Character;
class Word;
class Number;

// composite
class Sentence;
class Title;

// collection
class Paragraph;
class Section;
class Document;

class Encoder
{
  public:
    Encoder operator=(const Encoder& encoder) = delete;
    virtual ~Encoder() {};

    virtual int encode(const Character& object, std::istream& input) = 0;
    virtual int encode(const Word& object, std::istream& input) = 0;
    virtual int encode(const Number& object, std::istream& input) = 0;

    // group of atoms
    virtual int encode(const Sentence& object, std::istream& input) = 0;
    // level and one line
    virtual int encode(const Title& object, std::istream& input) = 0;

    // group of lines
    virtual int encode(const Paragraph& object, std::istream& input) = 0;
    // group of a title, and a group of elements with the same level
    virtual int encode(const Section& object, std::istream& input) = 0;
    // group of sections
    virtual int encode(const Document& object, std::istream& input) = 0;
};
