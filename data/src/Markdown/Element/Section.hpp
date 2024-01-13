#pragma once

#include <iostream>
#include <vector>
#include <memory>

#include "Element.hpp"
#include "Encoder.hpp"
#include "Collection.hpp"
#include "Title.hpp"

class Section : public Collection<MarkdownElement>
{
  public:
    Section();
    Section(std::shared_ptr<Title> title);
    Section(const Section& object);
    virtual ~Section();

    Section& operator=(const Section& other);

    virtual int encode(Encoder& encoder, std::istream& input) const;
    virtual bool  is_atom() const;

    void  set_title(std::shared_ptr<Title> title);

    std::shared_ptr<Title> title() const;

  protected:
    std::shared_ptr<Title> title_;
};
