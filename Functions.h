#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <map>
#include <vector>
#include <string>
#include <iostream>

template <typename T, typename U>
class Functions {
private:
    std::map<T, U> mapping;
    std::vector<T> domain;
    std::vector<U> codomain;

public:
    Functions();
    void addMapping(const T& input, const U& output);
    void display() const;

    bool isInjective() const;
    bool isSurjective() const;
    bool isBijective() const;
    bool isFunction() const;

    U apply(const T& input) const;
    Functions<U, T> inverse() const;

    template <typename V>
    Functions<T, V> compose(const Functions<U, V>& other) const;

    void displayProperties() const;
    std::vector<T> getDomain() const { return domain; }
    std::vector<U> getCodomain() const { return codomain; }
};

// Template implementations
template <typename T, typename U>
Functions<T, U>::Functions() {}

template <typename T, typename U>
void Functions<T, U>::addMapping(const T& input, const U& output) {
    mapping[input] = output;

    bool inDomain = false;
    for (const T& d : domain) {
        if (d == input) {
            inDomain = true;
            break;
        }
    }
    if (!inDomain) domain.push_back(input);

    bool inCodomain = false;
    for (const U& c : codomain) {
        if (c == output) {
            inCodomain = true;
            break;
        }
    }
    if (!inCodomain) codomain.push_back(output);
}

template <typename T, typename U>
void Functions<T, U>::display() const {
    std::cout << "Function mappings:\n";
    for (const auto& pair : mapping) {
        std::cout << "  " << pair.first << " -> " << pair.second << "\n";
    }
}

template <typename T, typename U>
bool Functions<T, U>::isFunction() const {
    for (const T& d : domain) {
        int count = 0;
        for (const auto& pair : mapping) {
            if (pair.first == d) count++;
        }
        if (count != 1) return false;
    }
    return true;
}

template <typename T, typename U>
bool Functions<T, U>::isInjective() const {
    std::vector<U> images;
    for (const auto& pair : mapping) {
        for (const U& img : images) {
            if (img == pair.second) return false;
        }
        images.push_back(pair.second);
    }
    return true;
}

template <typename T, typename U>
bool Functions<T, U>::isSurjective() const {
    for (const U& c : codomain) {
        bool found = false;
        for (const auto& pair : mapping) {
            if (pair.second == c) {
                found = true;
                break;
            }
        }
        if (!found) return false;
    }
    return true;
}

template <typename T, typename U>
bool Functions<T, U>::isBijective() const {
    return isInjective() && isSurjective();
}

template <typename T, typename U>
U Functions<T, U>::apply(const T& input) const {
    if (mapping.count(input)) {
        return mapping.at(input);
    }
    throw std::runtime_error("Input not in domain");
}

template <typename T, typename U>
Functions<U, T> Functions<T, U>::inverse() const {
    Functions<U, T> inv;
    if (!isBijective()) {
        std::cout << "Warning: Function is not bijective, inverse may not be well-defined\n";
    }

    for (const auto& pair : mapping) {
        inv.addMapping(pair.second, pair.first);
    }
    return inv;
}

template <typename T, typename U>
template <typename V>
Functions<T, V> Functions<T, U>::compose(const Functions<U, V>& other) const {
    Functions<T, V> result;

    for (const auto& pair : mapping) {
        try {
            V output = other.apply(pair.second);
            result.addMapping(pair.first, output);
        }
        catch (...) {
            // Skip if intermediate value not in other's domain
        }
    }

    return result;
}

template <typename T, typename U>
void Functions<T, U>::displayProperties() const {
    std::cout << "\n=== Function Properties ===\n";
    std::cout << "Is Valid Function: " << (isFunction() ? "YES" : "NO") << "\n";
    std::cout << "Injective (One-to-One): " << (isInjective() ? "YES" : "NO") << "\n";
    std::cout << "Surjective (Onto): " << (isSurjective() ? "YES" : "NO") << "\n";
    std::cout << "Bijective: " << (isBijective() ? "YES" : "NO") << "\n";
    std::cout << "Domain size: " << domain.size() << "\n";
    std::cout << "Codomain size: " << codomain.size() << "\n";
}

#endif