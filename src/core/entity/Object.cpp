
#include "Object.h"
#include <random>
#include <sstream>

Object::Object()
{
    // 鐢熸垚鍞竴ID
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 9999);
    std::stringstream ss;
    ss << "object_" << dis(gen);
    m_id = ss.str();
}

