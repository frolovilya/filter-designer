#include "ListSelectorValues.hpp"
#include <stdexcept>

template <typename E, typename C>
std::string toString(E value, C converter, size_t converterSize) {
    for (unsigned int i = 0; i < converterSize; i++) {
        if (converter[i].val == value) {
            return converter[i].str;
        }
    }

    throw std::logic_error("Unable to convert enum to string");
}

template <typename E, typename C>
E toValue(std::string str, C converter, size_t converterSize) {
    for (unsigned int i = 0; i < converterSize; i++) {
        if (converter[i].str == str) {
            return converter[i].val;
        }
    }

    throw std::invalid_argument("Unable to convert string to enum");
}

std::string toString(WindowType value) {
    return toString(value, windowTypes, sizeof(windowTypes) / sizeof(windowTypes[0]));
}

WindowType toWindowType(std::string str) {
    return toValue<WindowType>(str, windowTypes, sizeof(windowTypes) / sizeof(windowTypes[0]));
}

std::string toString(FilterType value) {
    return toString(value, filterTypes, sizeof(filterTypes) / sizeof(filterTypes[0]));
}

FilterType toFilterType(std::string str) {
    return toValue<FilterType>(str, filterTypes, sizeof(filterTypes) / sizeof(filterTypes[0]));
}

std::string toString(FilterPass value) {
    return toString(value, passTypes, sizeof(passTypes) / sizeof(passTypes[0]));
}

FilterPass toPassType(std::string str) {
    return toValue<FilterPass>(str, passTypes, sizeof(passTypes) / sizeof(passTypes[0]));
}

