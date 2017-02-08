#pragma once

#include <jni.h>

#include <string>
#include <exception>
#include <typeinfo>
#include <cstdint>
#include <iostream>

namespace hurricane {
namespace java {

class SignagureException : public std::exception {
public:
    SignagureException(const std::string& message) : _message(message) {
    }

#ifdef WIN32
    const char* what() const {
        return _message.c_str();
    }
#else
    const char* what() const noexcept {
        return _message.c_str();
    }
#endif

private:
    std::string _message;
};

namespace signature_impl {

template <class Type>
std::string ForType(const Type*) {
    throw SignagureException(std::string("Unknown java signature type: ") + typeid(Type).name());
}

template <>
inline std::string ForType(const bool*) {
    return "Z";
}

template <>
inline std::string ForType(const jbyte*) {
    return "B";
}

template <>
inline std::string ForType(const jchar*) {
    return "C";
}

template <>
inline std::string ForType(const jshort*) {
    return "S";
}

template <>
inline std::string ForType(const jint*) {
    return "I";
}

template <>
inline std::string ForType(const jlong*) {
    return "J";
}

template <>
inline std::string ForType(const jfloat*) {
    return "F";
}

template <>
inline std::string ForType(const jdouble*) {
    return "D";
}

template <>
inline std::string ForType(const void*) {
    return "V";
}

}

}
}
