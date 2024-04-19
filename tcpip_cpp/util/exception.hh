#pragma once

#include <memory>
#include <string>
#include <string_view>
#include <system_error>

class tagged_error : public std::system_error
{
private:
    std::string attempt_and_error_;
    int error_code_;
public:
    tagged_error(const std::error_category& category, const std::string_view s_attempt, const int error_code)
        : system_error(error_code, category),
        attempt_and_error_(std::string(s_attempt) + ": " + std::system_error::what()),
        error_code_(error_code){}

    // const char* what() const override {return attempt_and_error_.c_str();}
};
