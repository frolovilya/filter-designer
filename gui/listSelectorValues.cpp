#include "listSelectorValues.hpp"
#include <stdexcept>


std::string toString(WindowType value) {
    for (unsigned int i = 0; i < sizeof(windowTypes) / sizeof(windowTypes[0]); i++) {
        if (windowTypes[i].val == value) {
            return windowTypes[i].str;
        }
    }

    throw std::logic_error("Unable to convert window type to string");
}

WindowType toWindowType(std::string str) {
    for (unsigned int i = 0; i < sizeof(windowTypes) / sizeof(windowTypes[0]); i++) {
        if (windowTypes[i].str == str) {
            return windowTypes[i].val;
        }
    }

    throw std::invalid_argument("Invalid window type value");
}

std::string toString(FilterType value) {
    for (unsigned int i = 0; i < sizeof(filterTypes) / sizeof(filterTypes[0]); i++) {
        if (filterTypes[i].val == value) {
            return filterTypes[i].str;
        }
    }

    throw std::logic_error("Unable to convert filter type to string");
}

FilterType toFilterType(std::string str) {
    for (unsigned int i = 0; i < sizeof(filterTypes) / sizeof(filterTypes[0]); i++) {
        if (filterTypes[i].str == str) {
            return filterTypes[i].val;
        }
    }

    throw std::invalid_argument("Invalid filter type value");
}

std::string toString(PassType value) {
    for (unsigned int i = 0; i < sizeof(passTypes) / sizeof(passTypes[0]); i++) {
        if (passTypes[i].val == value) {
            return passTypes[i].str;
        }
    }

    throw std::logic_error("Unable to convert pass type to string");
}

PassType toPassType(std::string str) {
    for (unsigned int i = 0; i < sizeof(passTypes) / sizeof(passTypes[0]); i++) {
        if (passTypes[i].str == str) {
            return passTypes[i].val;
        }
    }

    throw std::invalid_argument("Invalid pass type value");
}

