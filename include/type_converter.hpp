#pragma once

#include <string>
#include <vector>
#include <sstream>

namespace clipp {

template<typename T>
struct TypeConverter {
    static T convert(const std::string& str);
};

template<>
struct TypeConverter<int> {
    static int convert(const std::string& str) { return std::stoi(str); }
};

template<>
struct TypeConverter<float> {
    static float convert(const std::string& str) { return std::stof(str); }
};

template<>
struct TypeConverter<double> {
    static double convert(const std::string& str) { return std::stod(str); }
};

template<>
struct TypeConverter<std::string> {
    static std::string convert(const std::string& str) { return str; }
};

template<>
struct TypeConverter<bool> {
    static bool convert(const std::string& str) { return str == "true" || str == "1"; }
};

template<typename T>
struct TypeConverter<std::vector<T>> {
    static std::vector<T> convert(const std::string& str) {
        std::vector<T> v;
        std::stringstream ss(str);
        std::string item;
        while (std::getline(ss, item, ',')) {
            v.push_back(TypeConverter<T>::convert(item));
        }
        return v;
    }
};

} // namespace clipp