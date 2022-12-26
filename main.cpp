#include <fstream>
#include <istream>
#include <string>
#include <iostream>
#include <stdexcept>

class parser {
public:
    using val_t = double;

    parser(std::istream * isp)
            : isp_(isp)
    {
    }

    val_t parse()
    {
        try {
            val_t x = A();
            return x;
        }
        catch(std::runtime_error e) {
            std::cout << e.what() << std::endl;
            return -1;
        }
    }
private:
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

        parser::val_t x = p.parse();

        std::cout << x << std::endl;
    }

    return 0;
}