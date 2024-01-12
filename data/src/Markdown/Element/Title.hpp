#pragma once

#include <iostream>
#include <memory>

#include "Element.hpp"
#include "Encoder.hpp"
#include "Line.hpp"

class Title
{
  public:
    Title();
    Title(int level);
    Title(int level, std::shared_ptr<Line> line);
    Title(const Title& object);
    virtual ~Title();

    Title& operator=(const Title& other);

    virtual int encode(Encoder& encoder, std::istream& input) const;
    virtual bool  is_atom() const;

    int level() const;
    std::shared_ptr<Line> line() const;

    void  set_level(int level);
    void  set_line(std::shared_ptr<Line> line);

  private:
    int level_;
    std::shared_ptr<Line> line_;
};
