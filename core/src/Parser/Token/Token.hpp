#pragma once

class Token
{
  public:
      virtual ~Token() {}
};

template <typename T>
class SimpleToken : public Token
{
  public:
    SimpleToken();
    SimpleToken(const T& data);
    SimpleToken(const SimpleToken& token);
    virtual ~SimpleToken();
    SimpleToken& operator=(const SimpleToken& token);

    void  set_data(const T& data);
    const T&  data() const;
  protected:
    T data_;
};

/////////////////////////////////////////////////////
////////////   template implementation   ////////////
/////////////////////////////////////////////////////

template <typename T>
SimpleToken<T>::SimpleToken()
  : data_() {}

template <typename T>
SimpleToken<T>::SimpleToken(const T& data)
  : data_(data) {}

template <typename T>
SimpleToken<T>::SimpleToken(const SimpleToken& token)
  : data_(token.data_) {}

template <typename T>
SimpleToken<T>::~SimpleToken() {}

template <typename T>
SimpleToken<T>& SimpleToken<T>::operator=(const SimpleToken& token)
{
  if (this != &token)
    data_ = token.data_;
  return *this;
}

template <typename T>
void SimpleToken<T>::set_data(const T& data)
{
  data_ = data;
}

template <typename T>
const T& SimpleToken<T>::data() const
{
  return data_;
}
