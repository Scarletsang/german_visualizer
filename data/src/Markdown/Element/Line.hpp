#pragma once

#include <iostream>
#include <vector>
#include <memory>

#include "Element.hpp"
#include "Encoder.hpp"
#include "Sentence.hpp"

class Line : public MarkdownElement
{
  public:
    using element = std::shared_ptr<Sentence>;
    Line();
    Line(const Line& object);
    virtual ~Line();

    Line& operator=(const Line& other);

    virtual int encode(Encoder& encoder, std::istream& input) const;
    virtual bool  is_atom() const;

    void  add_sentence(std::shared_ptr<Sentence> word);

    const std::vector<element>& data() const;
    std::vector<element>& data();

  private:
    std::vector<element> data_;
};
