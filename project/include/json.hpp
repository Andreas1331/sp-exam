//
// Created by andre on 06-05-2022.
//

#ifndef PROJECT_JSON_HPP
#define PROJECT_JSON_HPP

#include <iostream>
#include <iomanip>  // quoted
#include "meta.hpp"

template <typename T>
struct json_t
{
    T&& data;
};

template <typename T>
json_t(T&&) -> json_t<T>;  // Class Template Argument Deduction guide

struct json_reader_t
{
    std::ostream& os;
    bool first = true;  // first field is being visited
    template <typename Data>
    void operator()(const std::string& name, const Data& value)
    {
        if (!first)
            os << ',';
        else
            first = false;
        os << std::quoted(name) << ':' << json_t{value};
    }
    /** handle tag-dispatch for tuples: */
    template <typename Tuple, std::size_t... Is>
    void print_tuple(const Tuple& tuple, std::index_sequence<Is...>)
    {
        // parameter pack fold over the comma operator:
        (..., operator()(std::to_string(Is + 1), std::get<Is>(tuple)));
    }
    /** support for tuples: */
    template <typename... Ts>
    void operator()(const std::tuple<Ts...>& data)
    {
        print_tuple(data, std::index_sequence_for<Ts...>{});
    }
};

char peek_non_ws(std::istream& is)
{
    char c;
    while (is && std::isspace(is.peek()))
        is.get(c);
    return static_cast<char>(is.peek());
}

char read_non_ws(std::istream& is)
{
    char c;
    while (is.get(c) && std::isspace(c))
        ;
    return c;
}

void expect(std::istream& is, const char symbol)
{
    using namespace std::literals::string_literals;
    const auto c = read_non_ws(is);
    if (symbol != c)
        throw std::logic_error("Expected "s + symbol + " but got '" + c + "'");
}

struct json_writer_t
{
    std::istream& is;
    bool first = true;
    template <typename Data>
    void operator()(const std::string& name, Data& value)
    {
        using namespace std::literals::string_literals;
        if (!first) {
            expect(is, ',');
        } else
            first = false;
        std::string name_read;
        auto c = peek_non_ws(is);
        if (c != '"')
            throw std::logic_error("Expected '\"' but got '"s + c + "'");
        is >> std::quoted(name_read);
        if (name_read != name)
            throw std::logic_error("Expected field \""s + name + "\" but got " + name_read);
        expect(is, ':');
        is >> json_t{value};
    }
    /** handle tag-dispatch for tuples: */
    template <typename Tuple, std::size_t... Is>
    void fill_tuple(Tuple& tuple, std::index_sequence<Is...>)
    {
        (..., operator()(std::to_string(Is + 1), std::get<Is>(tuple)));
    }
    /** support for tuples: */
    template <typename... Ts>
    void operator()(std::tuple<Ts...>& data)
    {
        fill_tuple(data, std::index_sequence_for<Ts...>{});
    }
};

template <typename T>
std::ostream& operator<<(std::ostream& os, const json_t<T>& json)
{
    if constexpr (is_bool_v<T>) {
        auto flags = os.flags();  // save formatting flags
        os << std::boolalpha << json.data;
        os.setf(flags);  // recover the old flags
    } else if constexpr (is_number_v<T>)
        os << json.data;
    else if constexpr (is_string_v<T>)
        os << std::quoted(json.data);
    else if constexpr (is_container_v<T>) {
        auto b = std::begin(json.data), e = std::end(json.data);
        os << '[';
        if (b != e) {
            os << json_t{*b};
            while (++b != e)
                os << ',' << json_t{*b};
        }
        os << ']';
    } else if constexpr (is_tuple_v<T>) {
        os << '{';
        json_reader_t{os}(json.data);
        os << '}';
    } else if constexpr (accepts_reader_v<T, json_reader_t>) {
        os << '{';
        json.data.accept_reader(json_reader_t{os});
        os << '}';
    } else {
        static_assert(dependent_false<T>, "unsupported type");
    }
    return os;
}

template <typename T>
std::istream& operator>>(std::istream& is, json_t<T>& json)
{
    using namespace std::literals::string_literals;
    if constexpr (is_bool_v<T>) {
        auto flags = is.flags();  // save formatting flags
        is >> std::boolalpha >> json.data;
        is.setf(flags);  // restore the old flags
    } else if constexpr (is_number_v<T>)
        is >> json.data;
    else if constexpr (is_string_v<T>)
        is >> std::quoted(json.data);
    else if constexpr (is_container_v<T>) {
        // here support only STD containers, as arrays cannot change size
        // alternatively one may implement filling of the pre-allocated container
        using E = typename std::remove_reference_t<T>::value_type;
        expect(is, '[');
        auto first = true;
        while (is) {
            auto c = peek_non_ws(is);
            if (c == ']') {
                is.get(c);
                break;
            }
            if (!first) {
                if (c != ',')
                    throw std::logic_error("Expected ',' but got "s + c);
                is.get(c);
            } else
                first = false;
            E value{};
            is >> json_t{value};
            json.data.push_back(std::move(value));
        }
    } else if constexpr (is_tuple_v<T>) {
        expect(is, '{');
        json_writer_t{is}(json.data);
        expect(is, '}');
    } else if constexpr (accepts_writer_v<T, json_reader_t>) {
        expect(is, '{');
        json.data.accept_writer(json_writer_t{is});
        expect(is, '}');
    } else {
        TD<T> unknown;
    }
    return is;
}

template <typename T>  // binds to r-value references and calls >> on the l-value reference
std::istream& operator>>(std::istream& is, json_t<T>&& json)
{
    return is >> json;
}

#endif //PROJECT_JSON_HPP
