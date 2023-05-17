#include "json.h"
#include "parser.h"
#include <sstream>

using namespace JP::Json;
using namespace std;

// 构造实现：参数赋值 并 修改元素类型------------------------------------
Json::Json() : m_type(json_null){
}
Json::Json(bool value) : m_type(json_bool){
    m_value.m_bool = value;
}
Json::Json(int value) : m_type(json_int){
    m_value.m_int = value;
}
Json::Json(double value) : m_type(json_double){
    m_value.m_double = value;
}
Json::Json(const char* value) : m_type(json_string){
    m_value.m_string = new string(value);
}
Json::Json(const string& value) : m_type(json_string){
    m_value.m_string = new string(value);
}
Json::Json(Type type) : m_type(type){
    switch (m_type)
    {
        case json_null:
            break;
        case json_bool:
            m_value.m_bool = false;
            break;
        case json_int:
            m_value.m_int = 0;
            break;
        case json_double:
            m_value.m_double = 0.0;
            break;
        case json_string:
            m_value.m_string = new string("");
            break;
        case json_array:
            m_value.m_array = new vector<Json>();
            break;
        case json_object:
            m_value.m_object = new map<string, Json>();
            break;
        default:
            break;
    }
}
// 拷贝构造
Json::Json(const Json& other){
    copy(other);
}


// 数据类型转换重载（隐性）------------------------------------------------
Json::operator bool(){      // bool b = v2; 相当于 bool b = bool(v2)
    if(m_type != json_bool)
        throw logic_error("type error, not bool\n");
    return m_value.m_bool;
}
Json::operator int(){
    if(m_type != json_int)
        throw logic_error("type error, not int\n");
    return m_value.m_int;
}
Json::operator double(){
    if(m_type != json_double)
        throw logic_error("type error, not double\n");
    return m_value.m_double;
}
Json::operator string(){
    if(m_type != json_string)
        throw logic_error("type error, not string\n");
    return *(m_value.m_string);
}


// 数据类型转换（显性）--------------------------------------------------
bool Json::asBool() const{   // json.asBool(v2);
    if (m_type == json_bool)
        return m_value.m_bool;
    throw std::logic_error("function Json::asBool value type error");
}
int Json::asInt() const{
    if (m_type == json_int)
        return m_value.m_int;
    throw std::logic_error("function Json::asInt value type error");
}
double Json::asDouble() const{
    if (m_type == json_double)
        return m_value.m_double;
    throw std::logic_error("function Json::asDouble value type error");
}
string Json::asString() const{
    if (m_type == json_string)
        return *(m_value.m_string);
    throw std::logic_error("function Json::asString value type error");
}


// 操作符重载-----------------------------------------------------------
Json & Json::operator [] (int index){
    if(m_type != json_array)
        throw logic_error("type error, not array\n");
    
    if(index < 0 || index >= m_value.m_array->size())
        throw logic_error("index error\n");
    return (*(m_value.m_array))[index];
}

Json & Json::operator [] (const char* key){
    string name(key);   
    return (*(this))[name];             // 调用下方c++格式
}

Json & Json::operator [] (const string& key){
    if(m_type != json_object){      // 如果不是对象
        clear();          // 清除原来的再创建
        m_type = json_object;
        m_value.m_object = new map<string, Json>();
    }
    return (*(m_value.m_object))[key];
}

void Json::operator = (const Json& other){
    clear();
    copy(other);
}

bool Json::operator == (const Json & other){
    if (m_type != other.m_type) return false;
    switch (m_type)
    {
        case json_null:
            return true;
        case json_bool:
            return (m_value.m_bool == other.m_value.m_bool);
        case json_int:
            return (m_value.m_int == other.m_value.m_int);
        case json_double:
            return (m_value.m_double == other.m_value.m_double);
        case json_string:
            return *(m_value.m_string) == *(other.m_value.m_string);
        case json_array:
            return m_value.m_array == other.m_value.m_array;
        case json_object:
            return m_value.m_object == other.m_value.m_object;
        default:
            break;
    }
    return false;
}

bool Json::operator != (const Json & other){
    return !(*this == other);
}


// 动作函数------------------------------------------------------------
void Json::append(const Json& other){   
    if(m_type != json_array){       // 如果不是数组，则创建数组
    clear();
        m_type = json_array;
        m_value.m_array = new vector<Json>();
    }
    (m_value.m_array)->push_back(other);
}

bool Json::has(int index){
    if(m_type != json_array) return false;
    int size = (m_value.m_array)->size();
    return (index >= 0) && (index < size);
}
bool Json::has(const char* key){
    string name(key);
    return has(name);
}
bool Json::has(const string& key){
    if(m_type != json_object) return false;
    return (m_value.m_object)->find(key) != m_value.m_object->end();
}   

void Json::remove(int index){
    if(m_type != json_array) return ;
    if(index < 0 || index >= (m_value.m_array)->size()) return;
    (m_value.m_array)->at(index).clear();
    (m_value.m_array)->erase(m_value.m_array->begin() + index);
}
void Json::remove(const char* key){
    string name(key);
    remove(name);
}
void Json::remove(const string& key){
    auto it = m_value.m_object->find(key);
    if(it == (m_value.m_object)->end()) return;
    (*(m_value.m_object))[key].clear();
    (m_value.m_object)->erase(key);
}

void Json::copy(const Json& other){
    m_type = other.m_type;
    switch (m_type)
    {
        case json_null:
            break;
        case json_bool:
            m_value.m_bool = other.m_value.m_bool;
            break;
        case json_int:
            m_value.m_int = other.m_value.m_int;
            break;
        case json_double:
            m_value.m_double = other.m_value.m_double;
            break;
        case json_string:
            m_value.m_string = other.m_value.m_string;  // 浅拷贝
            break;
        case json_array:
            m_value.m_array = other.m_value.m_array;
            break;
        case json_object:
            m_value.m_object = other.m_value.m_object;
            break;
        default:
            break;
    }
}

void Json::clear(){
    switch (m_type)
    {
        case json_null:
        case json_bool:
        case json_int:
        case json_double:
            break;
        case json_string:
            delete m_value.m_string;
            break;
        case json_array:{
                for (auto it = (m_value.m_array)->begin(); 
                it != (m_value.m_array)->end(); it++){
                    it->clear();
                }
                delete m_value.m_array;
            }
            break;
        case json_object:{
                for (auto it = (m_value.m_object)->begin(); 
                it != (m_value.m_object)->end(); it++){
                    it->second.clear();
                }
                delete m_value.m_object;
            }
            break;
        default:
            break;
    }
    m_type = json_null;     // 清空数据类型
}

string Json::str() const{
    stringstream ss;
    switch (m_type)
    {
        case json_null:
            ss << "null";
            break;
        case json_bool:
            if (m_value.m_bool) ss << "true";
            else ss << "false";
            break;
        case json_int:
            ss << m_value.m_int;
            break;
        case json_double:
            ss << m_value.m_double;
            break;
        case json_string:      
            ss << "\"" << *(m_value.m_string) << "\"";
            break;
        case json_array:{
                ss << "[";
                for (auto it = (m_value.m_array)->begin(); 
                    it != (m_value.m_array)->end(); it++){
                    if (it != (m_value.m_array)->begin())   // 非起点输出前置逗号
                        ss << ",";          
                    ss << (*it).str();
                }
                ss << "]";
            }
            break;
        case json_object:{
                ss << "{";
                for (auto it = (m_value.m_object)->begin(); 
                    it != (m_value.m_object)->end(); it++){
                    if (it != (m_value.m_object)->begin())
                        ss << ",";
                    ss << "\"" << it->first << "\":" << it->second.str();
                }
                ss << "}";
            }
            break;
        default:
            break;
    }
    return ss.str();
}

void Json::parse(const string& str){
    Parser p;
    p.load(str);
    *this = p.parse();
}
