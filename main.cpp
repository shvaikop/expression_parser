#include <fstream>
#include <istream>
#include <string>
#include <iostream>
#include <stdexcept>
#include <map>

class parser {
public:
    using val_t = double;

    parser(std::istream * isp)
            : isp_(isp)
    {
    }

    val_t parse(std::size_t n)
    {
        for (std::size_t i = 0; i < n; i++) {
            try {
                if (std::isalpha(next())) {     // variable assignment
                    std::string var_name = get_var_name();
                    if (vars.count(var_name) != 0) {
                        throw std::runtime_error("Variable reassignment!");
                    }
                    if (next() != '=') {
                        throw std::runtime_error("Variable name must be followed by \"=\" variable assignement");
                    }
                    eat();
                    val_t expr = A();
                    vars[var_name] = expr;
                    std::cout << var_name << " = " << expr << std::endl;
                }
                else {
                    val_t expr = A();
                    std::cout << expr << std::endl;
                }
            }
            catch (std::runtime_error e) {
                std::cout << e.what() << std::endl;
                while (next() != '\n'){
                    next_ = is().get();
                }
            }
            eat(); // Might need to change to just eating new line character    !!!!!
        }
        return 0;
    }
private:
    val_t parse_line() {
        if (std::isalpha(next())){     // variable assignment
            std::string var_name = get_var_name();
            if (next() != '='){
                throw std::runtime_error("Variable name must be followed by \"=\" variable assignement");
            }
            eat();
            val_t expr = A();

        }
    }

    std::string get_var_name(){
        std::string var_name;
        while (std::isalpha(next())){
            var_name.push_back(std::tolower(next()));
            next_ = is().get();
        }
        return var_name;
    }

    val_t A()
    {
        val_t x = M();
        while (next() == '+' or next() == '-')
        {
            bool m = next() == '-';
            eat();
            val_t y = M();
            if (m)
                x -= y;
            else
                x += y;
        }
        return x;
    }

    val_t M()
    {
        val_t x = T();
        while (next() == '*' or next() == '/')
        {
            bool m = next() == '*';
            eat();
            val_t y = T();
            if (m)
                x *= y;
            else
                x /= y;
        }
        return x;
    }

    val_t T()
    {
        val_t x = 0;
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
        else if (std::isdigit(next()))      // handles multidigit integers
        {
            x = int(next() - '0');
            eat();
            while (std::isdigit(next())){
                x = (x * 10) + int(next() - '0');
                eat();
            }
        }
        else{
            throw std::runtime_error("T did not get an integer");
        }
        return x * mult;
    }

    char next()
    {
        return next_;
    }

    // goes to next character, ignores while space
    void eat()
    {
        do{
            next_ = is().get();
        } while (next_ == ' ' && next_ != EOF);

    }

    std::istream& is()
    {
        return *isp_;
    }

    std::istream* isp_;
    char next_ = is().get();
    std::map<std::string, double> vars;
};

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cout << "Missing file name" << std::endl;
        return 8;
    }

    std::string filename = argv[1];

    // read the file:
    {
        std::ifstream is(filename);

        if (!is.good())
        {
            std::cout << "Cannot open file \"" << filename << "\"" << std::endl;
            return 8;
        }

        parser p(&is);

        parser::val_t x = p.parse(5);

        std::cout << x << std::endl;
    }

    return 0;
}