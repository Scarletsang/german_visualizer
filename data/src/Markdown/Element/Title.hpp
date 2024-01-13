#pragma once

#include <memory>

#include "Element.hpp"
#include "Encoder.hpp"
#include "Collection.hpp"

class Title
{
  public:
    Title();
    Title(int level);
    Title(int level, std::shared_ptr<Paragraph> line);
    Title(const Title& object);
    virtual ~Title();

    Title& operator=(const Title& other);

    virtual int encode(Encoder& encoder) const;
    virtual bool  is_atom() const;

    int level() const;
    std::shared_ptr<Paragraph> data() const;

    void  set_level(int level);
    void  set_data(std::shared_ptr<Paragraph> line);

  protected:
    int level_;
    std::shared_ptr<Paragraph> data_;
};
