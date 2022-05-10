#ifndef PROJECT_META_HPP
#define PROJECT_META_HPP

#include <iterator>
#include <type_traits>
#include <ctime>

template <typename T>
constexpr auto is_bool_v = std::is_same_v<bool, std::remove_cv_t<std::remove_reference_t<T>>>;

template <typename T>
constexpr auto is_tm_v = std::is_same_v<std::tm, std::remove_cv_t<std::remove_reference_t<T>>>;

template <typename T>
constexpr auto is_number_v = std::is_arithmetic_v<std::remove_cv_t<std::remove_reference_t<T>>> && !is_bool_v<T>;

template <typename T>
constexpr auto is_character_v = std::is_same_v<char, std::remove_cv_t<std::remove_reference_t<T>>>;

template <typename C>
using get_iterator = decltype(std::begin(std::declval<C&>()));

template <typename C, typename = void>
struct is_container : std::false_type
{};  // primary declaration to fall back to

template <typename C>  // specialization for the iterable types
struct is_container<C, std::void_t<get_iterator<C>>> : std::true_type
{};

template <typename T>
concept NotChar = !is_character_v<T>;

template <typename C>
concept Container = requires(C&& c)
{
    {
    *std::begin(c)
    }
    ->NotChar;
    {std::end(c)};
};

template <typename C>
constexpr auto is_container_v = is_container<C>::value;

template <typename S, typename = void>
struct is_string : std::false_type
{};

template <typename S>  // container of characters
struct is_string<S, std::void_t<get_iterator<S>>>
        : std::conditional_t<is_character_v<typename std::iterator_traits<get_iterator<S>>::value_type>, std::true_type,
                std::false_type>
{};

template <>  // c-string
struct is_string<char*, void> : std::true_type
{};

template <>  // const c-string
struct is_string<const char*, void> : std::true_type
{};

template <typename S>
constexpr auto is_string_v = is_string<std::remove_cv_t<std::remove_reference_t<S>>>::value;

template <typename... Ts>
struct is_tuple : std::false_type
{};

template <typename... Ts>
struct is_tuple<std::tuple<Ts...>> : std::true_type
{};

template <typename T>
constexpr auto is_tuple_v = is_tuple<std::decay_t<T>>::value;

/** support for visitor pattern: */
template <typename Data, typename Reader, typename = void>
struct accepts_reader : std::false_type
{};

template <typename Data, typename Reader>
struct accepts_reader<Data, Reader,
        std::void_t<decltype(std::declval<const Data&>().accept_reader(std::declval<Reader&>()))>>
        : std::true_type
{};

template <typename Data, typename Reader>
constexpr auto accepts_reader_v = accepts_reader<Data, Reader>::value;

template <typename Data, typename Reader, typename = void>
struct accepts_writer : std::false_type
{};

template <typename Data, typename Writer>
struct accepts_writer<Data, Writer, std::void_t<decltype(std::declval<Data&>().accept_writer(std::declval<Writer&>()))>>
        : std::true_type
{};

template <typename Data, typename Writer>
constexpr auto accepts_writer_v = accepts_writer<Data, Writer>::value;

/** type-displayer for compile-time type debugging */
template <typename T>
struct TD;  // intentionally has no implementation to reveal the type in the error message

/** dependent_false is used for static_asserts inside constexpr-if else-branch.
 * The design is very similar to TD, except it has bool value.
 * See: https://devblogs.microsoft.com/oldnewthing/20200311-00/?p=103553
 * Also: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2019/p1830r1.pdf */
template <typename... Args>
constexpr auto dependent_false = false;

#endif //PROJECT_META_HPP
