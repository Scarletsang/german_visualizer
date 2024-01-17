/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Result.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htsang <htsang@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/12 14:35:42 by htsang            #+#    #+#             */
/*   Updated: 2024/01/17 00:34:41 by htsang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>

template <typename T, typename Err>
class Result
{
  public:
    static  Result Ok(T value);
    static  Result Error(Err error);

    Result(T value);
    Result(Err error, int);
    Result(Result const &src);
    ~Result();

    Result &operator=(Result const &src);

    operator bool() const;
    bool  is_ok() const;

    T     value() const;
    Err   error() const;

    ///////////////////////////////////////////
    ////////////   Result monads   ////////////
    ///////////////////////////////////////////

    template <typename T2, typename Err2>
    Result<T2, Err2> chain(Result<T2, Err2> (*f)()) const;

    template <typename T2, typename Err2>
    Result<T2, Err2> chain(Result<T2, Err2> (*f)(const T&)) const;

    template <typename T2, typename Err2, typename Env>
    Result<T2, Err2> chain(Result<T2, Err2> (*f)(Env), Env data) const;

    template <typename T2, typename Err2, typename Env>
    Result<T2, Err2> chain(Result<T2, Err2> (*f)(Env&), Env& data) const;

    template <typename T2, typename Err2, typename Env>
    Result<T2, Err2> chain(Result<T2, Err2> (*f)(const T&, Env), Env data) const;

    template <typename T2, typename Err2, typename Env>
    Result<T2, Err2> chain(Result<T2, Err2> (*f)(const T&, Env&), Env& data) const;

    //////////////////////////////////////////////////////
    ////////////   Result monad for methods   ////////////
    //////////////////////////////////////////////////////

    template <typename T2, typename Err2, typename Class>
    Result<T2, Err2> chain(Result<T2, Err2> (Class::*f)(), Class& obj) const;

    template <typename T2, typename Err2, typename Class>
    Result<T2, Err2> chain(Result<T2, Err2> (Class::*f)(const T&), Class& obj) const;

    template <typename T2, typename Err2, typename Class, typename Env>
    Result<T2, Err2> chain(Result<T2, Err2> (Class::*f)(Env), Class& obj, Env data) const;

    template <typename T2, typename Err2, typename Class, typename Env>
    Result<T2, Err2> chain(Result<T2, Err2> (Class::*f)(Env&), Class& obj, Env& data) const;

    template <typename T2, typename Err2, typename Class, typename Env>
    Result<T2, Err2> chain(Result<T2, Err2> (Class::*f)(const T&, Env), Class& obj, Env data) const;

    template <typename T2, typename Err2, typename Class, typename Env>
    Result<T2, Err2> chain(Result<T2, Err2> (Class::*f)(const T&, Env&), Class& obj, Env& data) const;

    ///////////////////////////////////////////////////////
    ////////////   function to handle errors   ////////////
    ///////////////////////////////////////////////////////

    template <typename T2, typename Err2>
    Result<T2, Err2> otherwise(Result<T2, Err2> (*f)()) const;

    template <typename T2, typename Err2>
    Result<T2, Err2> otherwise(Result<T2, Err2> (*f)(Err)) const;

    template <typename T2, typename Err2>
    Result<T2, Err2> otherwise(Result<T2, Err2> (*f)(Err&)) const;

    template <typename T2, typename Err2, typename Env>
    Result<T2, Err2> otherwise(Result<T2, Err2> (*f)(const Err&, Env), Env data) const;

    template <typename T2, typename Err2, typename Env>
    Result<T2, Err2> otherwise(Result<T2, Err2> (*f)(const Err&, Env&), Env& data) const;

    ///////////////////////////////////////////////////////////////////
    ////////////   function to handle errors for methods   ////////////
    ///////////////////////////////////////////////////////////////////

    template <typename T2, typename Err2, typename Class>
    Result<T2, Err2> otherwise(Result<T2, Err2> (Class::*f)(), Class& obj) const;

    template <typename T2, typename Err2, typename Class>
    Result<T2, Err2> otherwise(Result<T2, Err2> (Class::*f)(Err), Class& obj) const;

    template <typename T2, typename Err2, typename Class>
    Result<T2, Err2> otherwise(Result<T2, Err2> (Class::*f)(Err&), Class& obj) const;

    template <typename T2, typename Err2, typename Class, typename Env>
    Result<T2, Err2> otherwise(Result<T2, Err2> (Class::*f)(const Err&, Env), Class& obj, Env data) const;

    template <typename T2, typename Err2, typename Class, typename Env>
    Result<T2, Err2> otherwise(Result<T2, Err2> (Class::*f)(const Err&, Env&), Class& obj, Env& data) const;

  private:
    bool  is_ok_;
    T     value_;
    Err   error_;
};

/////////////////////////////////////////////////////
////////////   template implementation   ////////////
/////////////////////////////////////////////////////

template <typename T, typename Err>
Result<T,Err> Result<T,Err>::Ok(T value) { return Result<T,Err>(value); }

template <typename T, typename Err>
Result<T,Err> Result<T,Err>::Error(Err error) { return Result<T,Err>(error, 0); }

template <typename T, typename Err>
Result<T,Err>::Result(T value) : is_ok_(true), value_(value) {}

template <typename T, typename Err>
Result<T,Err>::Result(Err error, int) : is_ok_(false), error_(error) {}

template <typename T, typename Err>
Result<T,Err>::Result(Result const &src) : is_ok_(src.is_ok_) {
  if (is_ok_)
    value_ = src.value_;
  else
    error_ = src.error_;
}

template <typename T, typename Err>
Result<T,Err>::~Result() {}

template <typename T, typename Err>
Result<T,Err>&  Result<T,Err>::operator=(Result const &src)
{
  if (this != &src)
  {
    if (src.is_ok_)
      value_ = src.value_;
    else
      error_ = src.error_;
    is_ok_ = src.is_ok_;
  }
  return *this;
}

template <typename T, typename Err>
Result<T,Err>::operator bool() const { return is_ok_; }

template <typename T, typename Err>
bool  Result<T,Err>::is_ok() const { return is_ok_; }

template <typename T, typename Err>
T Result<T,Err>::value() const { return value_; }

template <typename T, typename Err>
Err Result<T,Err>::error() const { return error_; }

///////////////////////////////////////////
////////////   Result monads   ////////////
///////////////////////////////////////////

/**
 * @brief Result monad.
 * @param f a function that takes no argument and returns a Result
 * @note chain:: Result a err -> (() -> Result b err) -> Result b err
*/
template <typename T, typename Err>
template <typename T2, typename Err2>
Result<T2, Err2> Result<T,Err>::chain(Result<T2, Err2> (*f)()) const
{
  if (this->is_ok())
    return f();
  else
    return Result<T2, Err>::Error(error_);
}

/**
 * @brief Result monad.
 * @param f a function that takes the value of this Result and returns a Result
 * @note chain:: Result a err -> (a -> Result b err) -> Result b err
*/
template <typename T, typename Err>
template <typename T2, typename Err2>
Result<T2, Err2> Result<T,Err>::chain(Result<T2, Err2> (*f)(const T&)) const
{
  if (this->is_ok())
    return f(this->value_);
  else
    return Result<T2, Err>::Error(error_);
}

/**
 * @brief Result monad.
 * @param f a function that takes an env and returns a Result
 * @param data the env
 * @note chain:: Result a err -> env -> ( env -> Result b env) -> Result b env
*/
template <typename T, typename Err>
template <typename T2, typename Err2, typename Env>
Result<T2, Err2> Result<T,Err>::chain(Result<T2, Err2> (*f)(Env), Env data) const
{
  if (this->is_ok())
    return f(data);
  else
    return Result<T2, Err>::Error(error_);
}

/**
 * @brief Result monad.
 * @param f a function that takes a reference to env and returns a Result
 * @param data The reference to env
 * @note chain:: Result a err -> env -> ( env -> Result b env) -> Result b env
*/
template <typename T, typename Err>
template <typename T2, typename Err2, typename Env>
Result<T2, Err2> Result<T,Err>::chain(Result<T2, Err2> (*f)(Env&), Env& data) const
{
  if (this->is_ok())
    return f(data);
  else
    return Result<T2, Err>::Error(error_);
}

/**
 * @brief Result monad.
 * @param f a function that takes a value of this Result and an env and returns a Result
 * @param data The env
 * @note chain:: Result a err -> env -> ( a -> env -> Result b env) -> Result b env
*/
template <typename T, typename Err>
template <typename T2, typename Err2, typename Env>
Result<T2, Err2> Result<T,Err>::chain(Result<T2, Err2> (*f)(const T&, Env), Env data) const
{
  if (this->is_ok())
    return f(this->value_, data);
  else
    return Result<T2, Err>::Error(error_);
}

/**
 * @brief Result monad.
 * @param f a function that takes a value of this Result and a reference to env and returns a Result
 * @param data The reference to env
 * @note chain:: Result a err -> env -> ( a -> env -> Result b env) -> Result b env
*/
template <typename T, typename Err>
template <typename T2, typename Err2, typename Env>
Result<T2, Err2> Result<T,Err>::chain(Result<T2, Err2> (*f)(const T&, Env&), Env& data) const
{
  if (this->is_ok())
    return f(this->value_, data);
  else
    return Result<T2, Err>::Error(error_);
}

//////////////////////////////////////////////////////
////////////   Result monad for methods   ////////////
//////////////////////////////////////////////////////

/**
 * @brief Result monad that is compatible with methods.
 * @param f a method that takes no argument and returns a Result
 * @param obj the object on which the method is called
 * @note chain:: Result a err -> (() -> Result b err) -> Result b err
*/
template <typename T, typename Err>
template <typename T2, typename Err2, typename Class>
Result<T2, Err2> Result<T,Err>::chain(Result<T2, Err2> (Class::*f)(), Class& obj) const
{
  if (this->is_ok())
    return (obj.*f)();
  else
    return Result<T2, Err>::Error(error_);
}

/**
 * @brief Result monad that is compatible with methods.
 * @param f a method that takes the value of this Result and returns a Result
 * @param obj the object on which the method is called
 * @note chain:: Result a err -> (a -> Result b err) -> Result b err
*/
template <typename T, typename Err>
template <typename T2, typename Err2, typename Class>
Result<T2, Err2> Result<T,Err>::chain(Result<T2, Err2> (Class::*f)(const T&), Class& obj) const
{
  if (this->is_ok())
    return (obj.*f)(this->value_);
  else
    return Result<T2, Err>::Error(error_);
}

/**
 * @brief Result monad that is compatible with methods.
 * @param f a method that takes an env and returns a Result
 * @param obj the object on which the method is called
 * @param data the env
 * @note chain:: Result a err -> env -> ( env -> Result b env) -> Result b env
*/
template <typename T, typename Err>
template <typename T2, typename Err2, typename Class, typename Env>
Result<T2, Err2> Result<T,Err>::chain(Result<T2, Err2> (Class::*f)(Env), Class& obj, Env data) const
{
  if (this->is_ok())
    return (obj.*f)(data);
  else
    return Result<T2, Err>::Error(error_);
}

/**
 * @brief Result monad that is compatible with methods.
 * @param f a method that takes a reference to env and returns a Result
 * @param obj the object on which the method is called
 * @param data The reference to env
 * @note chain:: Result a err -> env -> ( env -> Result b env) -> Result b env
*/
template <typename T, typename Err>
template <typename T2, typename Err2, typename Class, typename Env>
Result<T2, Err2> Result<T,Err>::chain(Result<T2, Err2> (Class::*f)(Env&), Class& obj, Env& data) const
{
  if (this->is_ok())
    return (obj.*f)(data);
  else
    return Result<T2, Err>::Error(error_);
}

/**
 * @brief Result monad that is compatible with methods.
 * @param f a method that takes a value of this Result and an env and returns a Result
 * @param obj the object on which the method is called
 * @param data The env
 * @note chain:: Result a err -> env -> ( a -> env -> Result b env) -> Result b env
*/
template <typename T, typename Err>
template <typename T2, typename Err2, typename Class, typename Env>
Result<T2, Err2> Result<T,Err>::chain(Result<T2, Err2> (Class::*f)(const T&, Env), Class& obj, Env data) const
{
  if (this->is_ok())
    return (obj.*f)(this->value_, data);
  else
    return Result<T2, Err>::Error(error_);
}

/**
 * @brief Result monad that is compatible with methods.
 * @param f a method that takes a value of this Result and a reference to env and returns a Result
 * @param obj the object on which the method is called
 * @param data The reference to env
 * @note chain:: Result a err -> env -> ( a -> env -> Result b env) -> Result b env
*/
template <typename T, typename Err>
template <typename T2, typename Err2, typename Class, typename Env>
Result<T2, Err2> Result<T,Err>::chain(Result<T2, Err2> (Class::*f)(const T&, Env&), Class& obj, Env& data) const
{
  if (this->is_ok())
    return (obj.*f)(this->value_, data);
  else
    return Result<T2, Err>::Error(error_);
}

///////////////////////////////////////////////////////
////////////   function to handle errors   ////////////
///////////////////////////////////////////////////////

template <typename T, typename Err>
template <typename T2, typename Err2>
Result<T2, Err2> Result<T,Err>::otherwise(Result<T2, Err2> (*f)()) const
{
  if (this->is_ok())
    return Result<T2, Err2>::Ok(this->value_);
  else
    return f();
}

template <typename T, typename Err>
template <typename T2, typename Err2>
Result<T2, Err2> Result<T,Err>::otherwise(Result<T2, Err2> (*f)(Err)) const
{
  if (this->is_ok())
    return Result<T2, Err2>::Ok(this->value_);
  else
    return f(this->error_);
}

template <typename T, typename Err>
template <typename T2, typename Err2>
Result<T2, Err2> Result<T,Err>::otherwise(Result<T2, Err2> (*f)(Err&)) const
{
  if (this->is_ok())
    return Result<T2, Err2>::Ok(this->value_);
  else
    return f(this->error_);
}

template <typename T, typename Err>
template <typename T2, typename Err2, typename Env>
Result<T2, Err2> Result<T,Err>::otherwise(Result<T2, Err2> (*f)(const Err&, Env), Env data) const
{
  if (this->is_ok())
    return Result<T2, Err2>::Ok(this->value_);
  else
    return f(this->error_, data);
}

template <typename T, typename Err>
template <typename T2, typename Err2, typename Env>
Result<T2, Err2> Result<T,Err>::otherwise(Result<T2, Err2> (*f)(const Err&, Env&), Env& data) const
{
  if (this->is_ok())
    return Result<T2, Err2>::Ok(this->value_);
  else
    return f(this->error_, data);
}

///////////////////////////////////////////////////////////////////
////////////   function to handle errors for methods   ////////////
///////////////////////////////////////////////////////////////////

template <typename T, typename Err>
template <typename T2, typename Err2, typename Class>
Result<T2, Err2> Result<T,Err>::otherwise(Result<T2, Err2> (Class::*f)(), Class& obj) const
{
  if (this->is_ok())
    return Result<T2, Err2>::Ok(this->value_);
  else
    return (obj.*f)();
}

template <typename T, typename Err>
template <typename T2, typename Err2, typename Class>
Result<T2, Err2> Result<T,Err>::otherwise(Result<T2, Err2> (Class::*f)(Err), Class& obj) const
{
  if (this->is_ok())
    return Result<T2, Err2>::Ok(this->value_);
  else
    return (obj.*f)(this->error_);
}

template <typename T, typename Err>
template <typename T2, typename Err2, typename Class>
Result<T2, Err2> Result<T,Err>::otherwise(Result<T2, Err2> (Class::*f)(Err&), Class& obj) const
{
  if (this->is_ok())
    return Result<T2, Err2>::Ok(this->value_);
  else
    return (obj.*f)(this->error_);
}

template <typename T, typename Err>
template <typename T2, typename Err2, typename Class, typename Env>
Result<T2, Err2> Result<T,Err>::otherwise(Result<T2, Err2> (Class::*f)(const Err&, Env), Class& obj, Env data) const
{
  if (this->is_ok())
    return Result<T2, Err2>::Ok(this->value_);
  else
    return (obj.*f)(this->error_, data);
}

template <typename T, typename Err>
template <typename T2, typename Err2, typename Class, typename Env>
Result<T2, Err2> Result<T,Err>::otherwise(Result<T2, Err2> (Class::*f)(const Err&, Env&), Class& obj, Env& data) const
{
  if (this->is_ok())
    return Result<T2, Err2>::Ok(this->value_);
  else
    return (obj.*f)(this->error_, data);
}

template<typename T, typename Err>
std::ostream& operator<<(std::ostream& os, Result<T,Err> const& result)
{
  if (result.is_ok())
    os << result.value();
  else
    os << result.error();
  return os;
}
