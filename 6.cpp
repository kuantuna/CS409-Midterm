// ...
// Fill in all necessary extra code in this space to satisfy all questions below.
// You cannot change any other content other than the places marked as red.
// ...
// StudentID: S018474, Name: Tuna Tuncer
#include <iostream>
#include <tuple>
#include <vector>
#include <string>
#include <map>
#include <variant>
using namespace std;
template<typename ...> struct TD;
// Solve Q1a (10pts), Q1b (5pts) here:
// LIMITATION! For Q1a and Q1b in total, you can use at most 6 semicolon (;) characters
// including the struct ending semicolon

template<typename T>
struct Mat
{
    size_t rows, cols;
    T initial_value;
    vector<vector<T>> data;

    Mat(size_t rows, size_t cols, T initial_value) : rows(rows), cols(cols), initial_value(initial_value), data(rows, vector<T>(cols, initial_value))
    {
    }

    template<typename U>
    auto operator[] (U&& row_index) -> vector<T>&
    {
        return data[forward<U>(row_index)];
    }
};

// Solve Q1c here (5pts)

template<typename T>
auto print(const Mat<T>& matrix) -> void
{
    for(const auto& row_vectors: matrix.data)
    {
        for(const auto& item: row_vectors)
        {
            if constexpr(is_same_v<T, string>) // check later seems to be working fine
                cout << "\"" << item << "\" ";
            else
                cout << item << " ";
        }
        cout << endl;
    }
    cout << endl;
}

// Solve Q2 here (15 pts)

template<typename T>
[[nodiscard]] auto transform(const Mat<T>& mat_src, auto&& func)
{
    using NewType = decltype(func((mat_src.data.at(0)).at(0))); // Note this type is just for the [0][0] check if other indexes have another type
    auto transformed_mat = Mat(mat_src.rows, mat_src.cols, NewType{});
    for(size_t row_index=0; row_index<mat_src.rows; ++row_index)
        for(size_t col_index=0; col_index<mat_src.cols; ++col_index)
            transformed_mat[row_index][col_index] = func((mat_src.data.at(row_index)).at(col_index));
    return transformed_mat;
}

// Solve Q3a (10pts) and Q3b (10pts) here

template<typename T>
concept IsMatT = requires(T t)
{
    t.cols;
    t.rows;
    t.initial_value;
    t.data;
};

template<typename T>
concept Incrementable = requires(T t)
{
    is_same_v<decltype(t.initial_value), int>;
    is_same_v<decltype(t.initial_value), double>;
    is_same_v<decltype(t.initial_value), float>;
    is_same_v<decltype(t.initial_value), short>;
    is_same_v<decltype(t.initial_value), long>;
};

template<typename T>
[[nodiscard]] auto inc(T&& matrix)
requires requires
{
    IsMatT<remove_reference_t<T>>; // check this
    Incrementable<remove_reference_t<T>>; // check this
}
{
    if constexpr(std::is_same_v<remove_reference_t<T>, T>)
        cout << "Incrementing r-value matrix" << endl;
    else
        cout << "Incrementing l-value matrix" << endl;
    return transform(forward<T>(matrix), [](auto&& item){ return item+1; });
}

// Solve Q4 here (10 pts)

template<typename ... T>                 struct Concat;
template<typename ... T, typename ... U> struct Concat<tuple<T...>, tuple<U...>> { using type = tuple<T..., U...>; };
template<typename ... T>                 struct Concat<tuple<T...>, void>        { using type = tuple<T...>; };
template<typename ... T>                 struct Concat<tuple<T...>, tuple<void>> { using type = tuple<T...>; }; // bura eklendi
template<typename ... T>                 struct Concat<void, tuple<T...>>        { using type = tuple<T...>; };
template<typename ... T>                 struct Concat<tuple<void>, tuple<T...>> { using type = tuple<T...>; }; // bura eklendi
template<>                               struct Concat<tuple<void>, tuple<void>> { using type = tuple<void>; }; // bura eklendi
template<typename T, typename U> using concat_t = typename Concat<T, U>::type;

// Solve Q5 here (5 pts)

template<typename ... T> struct IsIntegral        { static const auto value = false; };
template<>               struct IsIntegral<int>   { static const auto value = true; };
template<>               struct IsIntegral<char>  { static const auto value = true; };
template<>               struct IsIntegral<short> { static const auto value = true; };
template<>               struct IsIntegral<long>  { static const auto value = true; };

// Solve Q6 here (15 pts)

template<typename ... T>                                                                     struct FilterTypes;
template<template<typename...> typename T, typename ...U, typename V>                        struct FilterTypes<T<U...>, V>
{ using type = tuple<conditional_t<T<V>::value, V, void>>; };
template<template<typename...> typename T, typename ...U, typename First, typename ... Rest> struct FilterTypes<T<U...>, First, Rest...>
{ using type = concat_t<tuple<conditional_t<T<First>::value, First, void>>, typename FilterTypes<T<U...>, Rest...>::type>; };


template<typename...T>                                                struct HelperFilter;
template<typename T, template<typename...> typename U, typename ...V> struct HelperFilter<T, U<V...>>
{ using type = typename FilterTypes<T, V...>::type; };


template<typename T, typename U> using filter_types_t = typename HelperFilter<T,U>::type;

// Solve Q7 here (15 pts)
// A String class to hide the underlying details of std::string (i.e. std::basic_string<char,...>) This shows as "String" when type-debugging, and facilitates
struct String : string { using string::string; };
// The transformer required by Q7 is supplied here
template<typename ...> struct Transformer;
template<typename T> struct Transformer<T> { using type = T; };
template<> struct Transformer<char> { using type = int; };
template<> struct Transformer<short> { using type = int; };
template<> struct Transformer<long> { using type = int; };
template<> struct Transformer<double> { using type = float; };
template<> struct Transformer<string> { using type = String; };
// Solve Q8 here (10 pts)
int main()
{
    // LIMITATION! For Q1a and Q1b in total, you can use at most 6 semicolon (;) characters
    // including the struct ending semicolon. More than 6 semicolons, you get zero points.
    // Q1a - Create a Mat<T> class with a constructor of Mat(rows, cols, initial_value)
    // T is automatically deduced from the initial_value's type
    auto m1 = Mat(2, 3, 9.9); // 2 row, 3 column matrix with double values is initialized to 9.9 for each cell
    print(m1);
    // Q1b - Make below assignment style work (i.e. m1[rowindex][colindex])
    for(size_t i=0; i<m1.rows; ++i)
        m1[i][i] = 1.1;
    // Q1c - write a print free-function that prints an instance of Mat<T>
    // if T is string, it puts " around the value, otherwise it directly prints the value
    print(m1);
    // Q2 - Write a "auto transform(const Mat<T>& mat_src, auto&& func)" free-function
    // that can take a source matrix and transform its contents globally.
    // At the end it returns the transformed matrix. Original matrix stays untouched.
    // Returned matrix can be of a different type depending on the "func"'s return value.
    // i.e. Mat<int> after transformation can be Mat<double> for instance.
    auto m2 = transform(
                Mat{2, 1, map<string, variant<string, double, int>>{ {"pi", 3.14}, {"CS", "409/509"}, {"year", 2021}}},
                []<typename T>(T&& map_) {
                    auto s = string{};
                    for(const auto& [key, value] : map_)
                    {
                        auto value_str = string{};
                        if(holds_alternative<string>(value))
                            value_str = get<string>(value);
                        else if(holds_alternative<double>(value))
                            value_str = to_string(get<double>(value));
                        else if(holds_alternative<int>(value))
                            value_str = to_string(get<int>(value));
                        s += key + ": " + value_str + " ";
                    }
                    return s;
                }
                );
    print(m1);
    print(m2);
    // Q3a - Write a SINGLE (overloading inc is not allowed) free-function named "inc".
    // writing overloads for inc function will get you zero points
    // This function returns a new Mat<T> whose contents are incremented by 1.
    // It also writes l-value or r-value to the console based on its parameter's situation at the call site.
    // Q3b - Make the SINGLE "inc" function available only for Mat<T> types using concepts
    // writing overloads for "inc" function will get you zero points
    // For instance, Mat<string> cannot be incremented. Mat<int>, Mat<float>, ... can be  incremented.
    print(inc(m1));
    print(inc(Mat(1, 4, 1)));
    // Q4 - Write a concat struct that can concatenate tuples at least as described below
    // concat omits the void at the end
    // concat concatenates types of two tuples into one tuple
//     using T1 = tuple<int, double, float>;
//     TD< concat_t<T1, void> > q4a; // ---> tuple<int, double, float>
//     TD< concat_t<T1, T1> > q4b; // ---> tuple<int, double, float, int, double, float>
    // Q5 - Write IsIntegral value-trait which is similar to std::is_integral.
    // But your implementation must also accept IsIntegral<> as a valid entry.
    // i.e. <> means an empty parameter-pack
    // Q6 - Write "filter_types" type-trait
    // that accepts a value-trait and many types
    // as a value trait you must support at least both of IsIntegral<> and is_integral<void>
    // a value-trait can be, for instance, IsIntegral that checks if a type is suitable or not
    // in the end, filter_types struct supplies the types filtered according to the value-trait in its "type" attribute
    // Do not write templated classes in the main() function block. Leave them in the global namespace.
//    using TUPLE = tuple<int, float, string, char, short, double, string, double, float>;
//    using TUPLE_INTEGRAL = filter_types_t<IsIntegral<>, TUPLE>;
    // TUPLE_INTEGRAL --> tuple<int, char, short>
//    TD< TUPLE_INTEGRAL > q6a;
//    using TUPLE_FLOATING = filter_types_t<is_floating_point<void>, TUPLE>;
    // TUPLE_FLOATING --> tuple<float, double, double, float>
//    TD< TUPLE_FLOATING > q6b;
    // Q7 - Write a "transform_types" type trait
    // that accepts convertion type-trait and many types
    // a type-trait specialized for your scenario is already supplied. Its called "Transformer".
    // in the end, transform_types struct supplies the transformed types according to the typetrait in its "type" attribute
    // Do not write templated classes in the main() function block. Leave them in the global namespace.
//    using TUPLE_TRANSFORMED = transform_types_t<Transformer<>, TUPLE>;
    // TUPLE_TRANSFORMED --> tuple<int, float, String, int, int, float, String, float, float>
    // TD< TUPLE_TRANSFORMED > q7a;
    // Q8 - Write a constexpr free-function named count_types
    // when used as shown below it returns the number of types in a tuple that fits to the criterion supplied
//    cout << "Number of integral types in TUPLE is " << count_types<IsIntegral>(TUPLE{}) << endl;
//    cout << "Number of integral types in TUPLE is " << count_types<is_integral>(TUPLE{}) << endl;
//    cout << "Number of floating types in TUPLE is " << count_types<is_floating_point>(TUPLE{}) << endl;
    return 0;
  }
