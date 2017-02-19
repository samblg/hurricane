#include "hurricane/multilang/java/String.h"
#include "hurricane/multilang/java/VirtualMachine.h"

namespace hurricane {
namespace java {

const std::string String::ClassName = "java/lang/String";

Class& String::GetDefaultClass()
{
    static Class stringClass = VirtualMachine::GetDefault()->FindClass(String::ClassName);

    return stringClass;
}

String::String(const Class& clazz) : _clazz(clazz)
{
}

String::String(const Class& clazz, const std::string& data) : _clazz(clazz), _data(data)
{
}

String::String() : String(GetDefaultClass()) {
}

String::String(const std::string& data) :
    String(GetDefaultClass(), data) {
}

String::String(jstring data) :
    String(GetDefaultClass())
{
    JNIEnv* env = VirtualMachine::GetDefault()->GetEnv();
    const char* buffer = env->GetStringUTFChars(data, nullptr);
    jsize stringLength = env->GetStringUTFLength(data);

    _data.resize(stringLength);
    _data.assign(buffer, buffer + stringLength);

    env->ReleaseStringUTFChars(data, buffer);
}

String::String(jobject data) : String(jstring(data))
{
}

jstring String::ToJavaString() const
{
    JNIEnv* env = VirtualMachine::GetDefault()->GetEnv();
    return env->NewStringUTF(_data.c_str());
}

}
}
