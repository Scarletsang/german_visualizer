#pragma once

#include <iostream>

#include "libcpp/Maybe.hpp"

class JsonEncoderSettings
{
  public:
    JsonEncoderSettings(\
      int level = 0, int indentation = 2, libcpp::Maybe<char> indentation_character = ' ');
    JsonEncoderSettings(const JsonEncoderSettings& object);
    virtual ~JsonEncoderSettings();

    JsonEncoderSettings& operator=(const JsonEncoderSettings& other);

    int level() const;
    int indentation() const;
    libcpp::Maybe<char> indentation_character() const;

    bool  is_compact() const;

    void  set_level(int level);
    void  increase_level();
    void  decrease_level();
    void  set_indentation(int indentation);
    void  set_indentation_character(libcpp::Maybe<char> indentation_character);

  private:
    int level_;
    int indentation_;
    libcpp::Maybe<char> indentation_character_;
};
