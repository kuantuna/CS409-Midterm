#include <iostream>
#include <vector>
#include <tuple>
using namespace std;

template<typename... T> struct TD;

template<typename ... T>                 struct Concat                           { using Type = tuple<T...>; };
template<typename ... T, typename ... U> struct Concat<tuple<T...>, tuple<U...>> { using Type = tuple<T..., U...>; };
template<typename ... T>                 struct Concat<tuple<T...>, void>        { using Type = tuple<T...>; };
template<typename ... T>                 struct Concat<tuple<T...>, tuple<void>> { using Type = tuple<T...>; }; // bura eklendi
template<typename ... T>                 struct Concat<void, tuple<T...>>        { using Type = tuple<T...>; };
template<typename ... T>                 struct Concat<tuple<void>, tuple<T...>> { using Type = tuple<T...>; }; // bura eklendi
template<>                               struct Concat<tuple<void>, tuple<void>> { using Type = tuple<void>; }; // bura eklendi
template<typename T, typename U> using concat_t = typename Concat<T, U>::Type;

template<typename ... T> struct IsIntegral        { static const auto value = false; };
template<>               struct IsIntegral<int>   { static const auto value = true; };
template<>               struct IsIntegral<char>  { static const auto value = true; };
template<>               struct IsIntegral<short> { static const auto value = true; };
template<>               struct IsIntegral<long>  { static const auto value = true; };


template<typename ... T>                                                                     struct FilterTypes;
template<template<typename...> typename A, typename ...B, typename V>                        struct FilterTypes<A<B...>, V>
{ using Type = tuple<conditional_t<A<V>::value, V, void>>; };
template<template<typename...> typename A, typename ...B, typename First, typename ... Rest> struct FilterTypes<A<B...>, First, Rest...>
{ using Type = concat_t<tuple<conditional_t<A<First>::value, First, void>>, typename FilterTypes<A<B...>, Rest...>::Type>; };


template<typename...T>                                                struct InnerFilter;
template<typename T, template<typename...> typename U, typename ...V> struct InnerFilter<T, U<V...>>
{ using Type = typename FilterTypes<T, V...>::Type; };


template<typename T, typename U> using filter_types_t = typename InnerFilter<T,U>::Type;


int main()
{
    using TUPLE = tuple<int, float, string, char, short, double, string, double, float>;
    using TUPLE_INTEGRAL = filter_types_t<IsIntegral<>, TUPLE>;
    // TUPLE_INTEGRAL --> tuple<int, char, short>
     TD< TUPLE_INTEGRAL > q6a;
    using TUPLE_FLOATING = filter_types_t<is_floating_point<void>, TUPLE>;
    // TUPLE_FLOATING --> tuple<float, double, double, float>
     TD< TUPLE_FLOATING > q6b;

     return 0;
}
