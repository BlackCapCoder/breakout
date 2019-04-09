#include <utility>

template <char... chars>
using TYPE_STRING = std::integer_sequence<char, chars...>;

template <typename T, T... chars>
constexpr TYPE_STRING<chars...> operator""_tstr() { return { }; }

template <typename>
struct STRING;

template <char... elements>
struct STRING<TYPE_STRING<elements...>> {
  static const char* GetString()
  {
    static constexpr char str[sizeof...(elements) + 1] = { elements..., '\0' };
    return str;
  }
};

#define str(A) STRING<decltype(A##_tstr)>
