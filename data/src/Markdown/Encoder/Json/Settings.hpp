#pragma once

class JsonEncoderSettings
{
  public:
    JsonEncoderSettings(\
      int level = 0, int indentation = 2, char indentation_character = ' ');
    JsonEncoderSettings(const JsonEncoderSettings& object);
    virtual ~JsonEncoderSettings();

    JsonEncoderSettings& operator=(const JsonEncoderSettings& other);

    int level() const;
    int indentation() const;
    char indentation_character() const;

    bool  is_compact() const;

    void  toggle_compact();
    void  set_level(int level);
    void  increase_level();
    void  decrease_level();
    void  set_indentation(int indentation);
    void  set_indentation_character(char indentation_character);

  private:
    int level_;
    int indentation_;
    char  indentation_character_;
    bool  is_compact_;
};
