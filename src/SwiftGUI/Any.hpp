////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of SwiftGUI.                                             //
//                                                                            //
// Copyright: (c) 2011-2015 Simon Schneegans & Felix Lauer                    //
//                                                                            //
// This software may be modified and distributed under the terms              //
// of the MIT license.  See the LICENSE file for details.                     //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// This class is based on the implementation from
// http://codereview.stackexchange.com/questions/20058/c11-any-class

#ifndef SWIFT_ANY_HPP
#define SWIFT_ANY_HPP

#include <utility>
#include <typeinfo>
#include <string>

namespace swift {

class Any
{
  template<typename T>
  using decay = typename std::decay<T>::type;

  template<typename T>
  using none = typename std::enable_if<!std::is_same<Any, T>::value>::type;

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:
   Any() { }
  ~Any() { delete p; }

  Any(Any &&s)      : p{s.p} { s.p = nullptr; }
  Any(Any const &s) : p{s.p->copy()} { }

  template<typename T, typename U = decay<T>, typename = none<U>>
  Any(T &&x) : p{new data<U>{std::forward<T>(x)}} { }

  Any &operator=(Any s) { swap(*this, s); return *this; }

  friend void swap(Any &s, Any &r) { std::swap(s.p, r.p); }

  void clear() { delete p; p = nullptr; }

  bool empty() const { return p; }

  template<typename T>
  bool is() const { return p ? p->is(typeid(T)) : false; }

  template<typename T> T      &&_()     && { return std::move(stat<T>()); }
  template<typename T> T       &_()      & { return stat<T>(); }
  template<typename T> T const &_() const& { return stat<T>(); }

  template<typename T> T      &&cast()     && { return std::move(dyn<T>()); }
  template<typename T> T       &cast()      & { return dyn<T>(); }
  template<typename T> T const &cast() const& { return dyn<T>(); }

  template<typename T> operator T     &&()     && { return std::move(_<T>()); }
  template<typename T> operator T      &()      & { return _<T>(); }
  template<typename T> operator T const&() const& { return _<T>(); }

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:

  struct base
  {
    virtual ~base() { }
    virtual bool is(std::type_info const&) const = 0;
    virtual base *copy() const = 0;
  } *p = nullptr;

  template<typename T>
  struct data : base, std::tuple<T>
  {
    using std::tuple<T>::tuple;

    T       &get()      & { return std::get<0>(*this); }
    T const &get() const& { return std::get<0>(*this); }

    bool is(std::type_info const& i) const override { return i == typeid(T); }
    base *copy()  const override { return new data{get()}; }
  };

  template<typename T>
  T &stat() { return static_cast<data<T>&>(*p).get(); }

  template<typename T>
  T const &stat() const { return static_cast<data<T> const&>(*p).get(); }

  template<typename T>
  T &dyn() { return dynamic_cast<data<T>&>(*p).get(); }

  template<typename T>
  T const &dyn() const { return dynamic_cast<data<T> const&>(*p).get(); }
};

}

#endif // SWIFT_ANY_HPP
