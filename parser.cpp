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

#include "parser.hpp"

using val_t = std::variant<int, double>;

Parser::Parser() {
    next_ = std::cin.get();
}

void Parser::parse() {
    std::size_t n;
    try{
        n = get_num_lines();
    }
    catch(std::runtime_error &e){
        std::cout << e.what() << std::endl;
        exit(1);
    }
    for (std::size_t i = 0; i < n; i++) {
        try {
            if (std::isalpha(next())) {     // variable assignment
                std::string var_name = get_var_name();
                if (vars_.count(var_name) != 0) {    // variable reassignment
                    throw std::runtime_error("variable reassignment");
                }
                if (next() != '=') {    // variable assignment wrong format
                    throw std::runtime_error("variable assignment wrong format");
                }
                eat();
                val_t expr = A();   // evaluate and store variable assignment
                vars_[var_name] = expr;
                if (next() != '\n'){
                    throw std::runtime_error("Something wrong, not \\n at the end of expression");
                }
                print_variant_val(expr);
            }
            else {
                val_t expr = A();
                if (next() != '\n'){
                    throw std::runtime_error("Something wrong, not \\n at the end of expression");
                }
                print_variant_val(expr);
            }
        }
        catch (std::runtime_error &e) {     // skip the rest of the invalid expr
            std::cout << "invalid" << std::endl;
            while (next() != '\n'){
                next_ = std::cin.get();
            }
        }

        next_ = std::cin.get();
    }
}

val_t Parser::A() {
    val_t x = M();
    while (next() == '+' or next() == '-')
    {
        bool m = next() == '-';
        eat();
        val_t y = M();
        if (m)
            x = minus(x, y);
        else
            x = add(x, y);
    }
    return x;
}

val_t Parser::M() {
    val_t x = T();
    while (next() == '*' or next() == '/' or next() == '%')
    {
        char op = next();
        eat();
        val_t y = T();
        if (op == '*'){
            x = mul(x, y);
        }
        else if (op == '/'){
            x = div(x, y);
        }
        else if(op == '%'){
            x = mod(x, y);
        }
    }
    return x;
}

val_t Parser::T() {
    val_t x = 0;
    bool d = false;
    int mult = 1;
    if (next() == '-'){     // handles unary minus
        mult = -1;
        eat();
    }
    if (next() == '(')      // handles subexpressions in brackets
    {
        eat();
        x = A();
        if (next() != ')')
            throw std::runtime_error("')' expected.");
        eat();
    }
    else if (next() == '('){
        throw std::runtime_error("Random ) detected!!!");
    }
    else if (std::isdigit(next()))      // handles multidigit terms
    {
        std::string num_str;
        while(std::isdigit(next()) || next() == '.'){
            if (next() == '.'){
                d = true;
            }
            num_str += next();
            eat();
        }
        if (d){
            x = std::stod(num_str);
        }
        else{
            x = std::stoi(num_str);
        }
    }
    else if (next() == '$'){
        next_ = std::cin.get();     // eat the '$'
        std::string var_name;
        while (std::isalpha(next())){
            var_name.push_back(std::tolower(next()));
            next_ = std::cin.get();
        }
        if (next() == ' '){
            eat();
        }
        if (vars_.count(var_name) == 0){
            throw std::runtime_error("Variable does not exist!");
        }
        x = vars_[var_name];
    }
    else{
        throw std::runtime_error("T did not get an integer");
    }
    if (std::holds_alternative<int>(x)){
        return std::get<int>(x) * mult;
    }
    else if (std::holds_alternative<double>(x)){
        return std::get<double>(x) * mult;
    }
    else {
        throw std::runtime_error("Wrong value stored in variant!!!");
    }
}

std::size_t Parser::get_num_lines() {
    if (!std::isdigit(next())){
        exit(1);
        //throw std::runtime_error("Integer expected as number of lines!!!");
    }
    auto var_n = T();
    std::size_t n;
    if (std::holds_alternative<int>(var_n)){
        n = (size_t) std::get<int>(var_n);
    }
    else {
        throw std::runtime_error("Wrong format of number of lines!!!");
    }
    while (next() != '\n'){
        next_ = std::cin.get();
    }
    next_ = std::cin.get();
    return n;
}

std::string Parser::get_var_name() {
    std::string var_name;
    while (std::isalpha(next())){
        var_name.push_back(std::tolower(next()));
        next_ = std::cin.get();
    }
    return var_name;
}

void Parser::print_variant_val(std::variant<int, double> v) {
    if (std::holds_alternative<int>(v)){
        std::cout << std::get<int>(v) << std::endl;
    }
    else{
        std::cout << std::get<double>(v) << std::endl;
    }
}

char Parser::next() {
    return next_;
}

void Parser::eat() {
    do{
        next_ = std::cin.get();
    } while (next_ == ' ' && next_ != EOF);
}

std::variant<int, double> Parser::mod(std::variant<int, double> &a, std::variant<int, double> &b) {
    if (std::holds_alternative<int>(a)){
        if (std::holds_alternative<int>(b)){
            return std::get<int>(a) % std::get<int>(b);
        }
        else {
            throw std::runtime_error("invalid operands for %!!!");
        }
    }
    throw std::runtime_error("invalid operands for %!!!");
}

std::variant<int, double> Parser::mul(std::variant<int, double> &a, std::variant<int, double> &b) {
    if (std::holds_alternative<int>(a)){
        if (std::holds_alternative<int>(b)){
            return std::get<int>(a) * std::get<int>(b);
        }
        else if (std::holds_alternative<double>(b)){
            return std::get<int>(a) * std::get<double>(b);
        }
    }
    else if (std::holds_alternative<double>(a)){
        if (std::holds_alternative<int>(b)){
            return std::get<double>(a) * std::get<int>(b);
        }
        else if (std::holds_alternative<double>(b)){
            return std::get<double>(a) * std::get<double>(b);
        }
    }
    throw std::runtime_error("Empty variant!");
}

std::variant<int, double> Parser::minus(std::variant<int, double> &a, std::variant<int, double> &b) {
    if (std::holds_alternative<int>(a)){
        if (std::holds_alternative<int>(b)){
            return std::get<int>(a) - std::get<int>(b);
        }
        else if (std::holds_alternative<double>(b)){
            return std::get<int>(a) - std::get<double>(b);
        }
    }
    else if (std::holds_alternative<double>(a)){
        if (std::holds_alternative<int>(b)){
            return std::get<double>(a) - std::get<int>(b);
        }
        else if (std::holds_alternative<double>(b)){
            return std::get<double>(a) - std::get<double>(b);
        }
    }

    throw std::runtime_error("Empty variant!");
}

std::variant<int, double> Parser::add(std::variant<int, double> &a, std::variant<int, double> &b) {
    if (std::holds_alternative<int>(a)){
        if (std::holds_alternative<int>(b)){
            return std::get<int>(a) + std::get<int>(b);
        }
        else if (std::holds_alternative<double>(b)){
            return std::get<int>(a) + std::get<double>(b);
        }
    }
    else if (std::holds_alternative<double>(a)){
        if (std::holds_alternative<int>(b)){
            return std::get<double>(a) + std::get<int>(b);
        }
        else if (std::holds_alternative<double>(b)){
            return std::get<double>(a) + std::get<double>(b);
        }
    }
    throw std::runtime_error("Empty variant!");
}

std::variant<int, double> Parser::div(std::variant<int, double> &a, std::variant<int, double> &b) {
    if (std::holds_alternative<int>(a)){
        if (std::holds_alternative<int>(b)){
            return std::get<int>(a) / std::get<int>(b);
        }
        else if (std::holds_alternative<double>(b)){
            return std::get<int>(a) / std::get<double>(b);
        }
    }
    else if (std::holds_alternative<double>(a)){
        if (std::holds_alternative<int>(b)){
            return std::get<double>(a) / std::get<int>(b);
        }
        else if (std::holds_alternative<double>(b)){
            return std::get<double>(a) / std::get<double>(b);
        }
    }
    throw std::runtime_error("Empty variant!");
}