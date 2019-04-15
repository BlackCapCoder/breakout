#ifndef TEMP_H
#define TEMP_H

#include <tuple>
#include <type_traits>


template <bool, class, class> struct If;
template <class T, class F> struct If <true,  T, F> { using value = T; };
template <class T, class F> struct If <false, T, F> { using value = F; };


template <class, class> struct Concat;
template <template <class> class C, class...Xs, class...Ys>
struct Concat<C<Xs...>, C<Ys...>>
{
  using value = C<Xs..., Ys...>;
};


template <template <class> class, class> struct Filter;

template <template <class> class F, template <class...> class C, class X, class...Xs>
struct Filter<F, C<X, Xs...>>
{
  using rest  = typename Filter<F, C<Xs...>>::value;
  using value = typename Concat<typename If<F<X>::value, C<>, C<X>>::value, rest>::value;
};
template <template <class> class F, template <class...> class C>
struct Filter<F, C<>>
{
  using value = C<>;
};


template <template <class...> class T, class...Xs>
struct Apply
{
  template <class...Rst>
  using value = T<Xs..., Rst...>;
};


template <typename T, typename Tuple>
struct has_type;

template <typename T>
struct has_type<T, std::tuple<>> : std::false_type {};

template <typename T, typename U, typename... Ts>
struct has_type<T, std::tuple<U, Ts...>> : has_type<T, std::tuple<Ts...>> {};

template <typename T, typename... Ts>
struct has_type<T, std::tuple<T, Ts...>> : std::true_type {};



#endif // ifndef TEMP_H
