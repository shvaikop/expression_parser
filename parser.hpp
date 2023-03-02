#ifndef FSTREAM
#define FSTREAM
#include <iostream>
#endif

#ifndef IOSTREAM
#define IOSTREAM
#include <iostream>
#endif

#ifndef MAP
#define MAP
#include <map>
#endif

#ifndef VARIANT
#define VARIANT
#include <variant>
#endif

#ifndef PARSER_PARSER_HPP
#define PARSER_PARSER_HPP

class Parser {
public:
    using val_t = std::variant<int, double>;
    Parser();

    void parse();

private:
    char next_;
    std::map<std::string, val_t> vars_;

    // Helper functions to perform an operation on 2 variants holding either int or double
    std::variant<int, double> add(std::variant<int, double> &a, std::variant<int, double> &b);

    std::variant<int, double> mul(std::variant<int, double> &a, std::variant<int, double> &b);

    std::variant<int, double> div(std::variant<int, double> &a, std::variant<int, double> &b);

    std::variant<int, double> mod(std::variant<int, double> &a, std::variant<int, double> &b);

    std::variant<int, double> minus(std::variant<int, double> &a, std::variant<int, double> &b);

    // helper method to print the value of a variant holding either int or double
    void print_variant_val(std::variant<int, double> v);

    // helper method to parse the first line of input
    std::size_t get_num_lines();

    // helper method to parse variable name
    std::string get_var_name();

    // Parse +- epression
    val_t A();

    // Parse */% expression
    val_t M();

    // Parse term of an expression
    val_t T();

    // Returns the char stored in the _next variable
    char next();

    // Return the next character in the expression
    void eat();

};

#endif //PARSER_PARSER_HPP
