#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <iostream>
#include <string_view>
#include "type_converter.hpp"
#include "validation_module.hpp"
#include "error_handler_module.hpp"

namespace clipp {

using SetterFunc = void (*)(void*, const std::string&);

struct ArgumentBase {
    std::string short_name;
    std::string long_name;
    std::string description;
    bool required = false;
    bool parsed = false;
    SetterFunc setter = nullptr;
    virtual ~ArgumentBase() = default;
    void set_value(const std::string& str) {
        if (setter) setter(this, str);
    }
};

template<typename T>
struct Argument : ArgumentBase {
    T value;
    T* default_value = nullptr;
    static void set_value_impl(void* self, const std::string& str) {
        auto* arg = static_cast<Argument<T>*>(static_cast<ArgumentBase*>(self));
        validate_argument(str);
        arg->value = TypeConverter<T>::convert(str);
        arg->parsed = true;
    }
};

class ArgumentParser {
public:
    ArgumentParser(const std::string& program_name, const std::string& description = "")
        : program_name_(program_name), description_(description) {}

    template<typename T>
    void add_option(const std::string& short_name, const std::string& long_name, const std::string& description, bool required = false, T* default_value = nullptr) {
        static_assert(TypeConverter<T>::is_supported, "Type T is not supported for argument conversion");
        auto arg = std::make_unique<Argument<T>>();
        arg->short_name = short_name;
        arg->long_name = long_name;
        arg->description = description;
        arg->required = required;
        arg->default_value = default_value;
        arg->setter = &Argument<T>::set_value_impl;
        if (default_value) arg->value = *default_value;
        arguments_.push_back(std::move(arg));
        ArgumentBase* ptr = arguments_.back().get();
        if (!short_name.empty()) name_to_arg_[short_name] = ptr;
        if (!long_name.empty()) name_to_arg_[long_name] = ptr;
    }

    ParseResult parse(int argc, char* argv[]) {
        args_.clear();
        for (int i = 0; i < argc; ++i) {
            args_.push_back(argv[i]);
        }
        // Parse options
        int i = 1; // skip program name
        while (i < argc) {
            std::string arg = argv[i];
            validate_argument(arg); // basic validation
            if (arg.size() > 1 && arg[0] == '-') {
                std::string name;
                if (arg[1] == '-') {
                    // long option
                    std::string long_part = arg.substr(2);
                    name = "--" + long_part;
                } else {
                    // short option
                    std::string short_part = arg.substr(1);
                    name = "-" + short_part;
                }
                auto it = name_to_arg_.find(name);
                if (it != name_to_arg_.end()) {
                    validate_index(++i, argc);
                    try {
                        it->second->set_value(argv[i]);
                    } catch (const std::exception& e) {
                        return {false, format_error_message("parsing", "Error parsing " + name + ": " + e.what())};
                    }
                } else {
                    return {false, format_error_message("unknown_option", "Unknown option: " + std::string(arg))};
                }
            }
            ++i;
        }
        return {true, ""};
    }

    template<typename T>
    T get(const std::string& name) const {
        auto it = name_to_arg_.find(name);
        if (it == name_to_arg_.end()) throw std::runtime_error("Unknown argument: " + name);
        auto arg = dynamic_cast<Argument<T>*>(it->second);
        if (!arg) throw std::runtime_error("Type mismatch for argument: " + name);
        return arg->value;
    }

    const std::vector<std::string>& get_args() const { return args_; }

    void print_usage(std::ostream& os = std::cout) {
        os << "Usage:\n  " << program_name_ << " [options]\n\n";
        if (!description_.empty()) os << description_ << "\n\n";
        os << "Options:\n";
        for (auto& arg : arguments_) {
            os << "  ";
            if (!arg->short_name.empty()) os << arg->short_name;
            if (!arg->short_name.empty() && !arg->long_name.empty()) os << ", ";
            if (!arg->long_name.empty()) os << arg->long_name;
            os << " <value> " << arg->description;
            if (arg->required) os << " (required)";
            os << "\n";
        }
    }

private:
    std::string program_name_;
    std::string description_;
    std::vector<std::string> args_;
    std::vector<std::unique_ptr<ArgumentBase>> arguments_;
    std::map<std::string, ArgumentBase*> name_to_arg_;
};

} // namespace clipp