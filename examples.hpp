#pragma once

#ifdef __GNUG__
constexpr bool GCC_COMPILER = true;
#else
constexpr bool GCC_COMPILER = false;
#endif

#include <iostream>
#include <type_traits>

namespace util {
  template<typename T>
  constexpr std::string_view typestr(){
    // for now
    static_assert(GCC_COMPILER);
    std::string_view out = __PRETTY_FUNCTION__;
    out = out.substr(6+47);
    out = out.substr(0, out.size() - 50);
    return out;
  }
}

namespace example1 {

  // this is your type
  struct S {
    int x;
  };

  // S& -> int&
  // S&& -> int&&
  // const S& -> const int&
  template<typename T> requires std::is_same_v<std::remove_cvref_t<T>, S>
  auto&& getX(T&& t){
    return std::forward<T>(t).x;
  }

  void demonstration(){
#define SHOW(T) do {std::cout << #T << " --> " << util::typestr<decltype(getX(std::declval<T>()))>() << std::endl;} while (0)
    SHOW(S&);
    SHOW(const S&);
    SHOW(S&&);
#undef SHOW
  }
  
} // namespace example1

namespace example2 {

  // this is your type template
  template<typename T>
  struct Stemplate {
    using type = T;
    T x;
  };

  // boilerplate you only need to write once
  template<typename T>
  constexpr bool is_Stemplate_v = false;
  template<typename T>
  constexpr bool is_Stemplate_v<Stemplate<T>> = true;

  // S& -> S::type&
  // S&& -> S::type&&
  // const S& -> const S::type&
  template<typename T> requires is_Stemplate_v<std::remove_cvref_t<T>>
  auto&& getX(T&& t){
    return std::forward<T>(t).x;
  }

  void demonstration(){
#define SHOW(T) do {std::cout << #T << " --> " << util::typestr<decltype(getX(std::declval<T>()))>() << std::endl;} while (0)
    SHOW(Stemplate<float>&);
    SHOW(const Stemplate<bool>&);
    SHOW(Stemplate<char>&&);
#undef SHOW    
  }
  
} // namespace example2
