#pragma once
#include <iostream>
#include <exception>

class Arithmetical_Exception : public std::exception {
    const double value;

  public:
    Arithmetical_Exception(double doubleValue) : value(doubleValue)
    {
    }

    virtual const char* what() const throw()
    {
        return std::string("Arithmetical Exception discovered. Value is : ")
          .append(show_classification(value))
          .c_str();
    }

    const double getDoubleValue() const
    {
        return value;
    }

    const char* show_classification(double x) const
    {
        switch (std::fpclassify(x)) {
            case FP_INFINITE:
                return "Inf";
            case FP_NAN:
                return "NaN";
            case FP_NORMAL:
                return "normal";
            case FP_SUBNORMAL:
                return "subnormal";
            case FP_ZERO:
                return "zero";
            default:
                return "unknown";
        }
    }
};
