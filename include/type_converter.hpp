#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <type_traits>

namespace clipp {

template<typename T>
struct TypeConverter {
    static constexpr bool is_supported = false;
    static T convert(const std::string& str);
};

template<>
struct TypeConverter<int> {
    static constexpr bool is_supported = true;
    static int convert(const std::string& str) { return std::stoi(str); }
};

template<>
struct TypeConverter<float> {
    static constexpr bool is_supported = true;
    static float convert(const std::string& str) { return std::stof(str); }
};

template<>
struct TypeConverter<double> {
    static constexpr bool is_supported = true;
    static double convert(const std::string& str) { return std::stod(str); }
};

template<>
struct TypeConverter<std::string> {
    static constexpr bool is_supported = true;
    static std::string convert(const std::string& str) { return str; }
};

template<>
struct TypeConverter<bool> {
    static constexpr bool is_supported = true;
    static bool convert(const std::string& str) { return str == "true" || str == "1"; }
};

template<typename T>
struct TypeConverter<std::vector<T>> {
    static constexpr bool is_supported = TypeConverter<T>::is_supported;
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