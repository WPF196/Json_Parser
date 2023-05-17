#pragma once

#include <string>
#include <vector>
#include <map>

using namespace std;

namespace JP{
namespace Json{

// Json的数据类型
enum Type{
    json_null = 0,
    json_bool,
    json_int,
    json_double,
    json_string,
    json_array,
    json_object
};

class Json{
public:
    // 构造(多种数据类型)
    Json();
    Json(bool);
    Json(int);
    Json(double);
    Json(const char*);
    Json(const string&);
    Json(Type);
    // 拷贝构造
    Json(const Json&);

    // 数据类型转换重载（隐性）
    operator bool();
    operator int();
    operator double();
    operator string();

    // 数据类型转换（显性）
    bool asBool() const;
    int asInt() const;
    double asDouble() const;
    string asString() const;

    // 操作符重载
    Json & operator [] (int);           // 数组下标
    Json & operator [] (const char*);   // object的key（c格式）
    Json & operator [] (const string&); // object的key（c++格式）
    void operator = (const Json&);      // 重载等号，用于对象赋值
    bool operator == (const Json&);     // 相等判断
    bool operator != (const Json&);     // 不等判断
    
    // 类型判断
    bool isNull() const { return m_type == json_null; };
    bool isBool() const { return m_type == json_bool; };
    bool isInt() const { return m_type == json_int; };
    bool isDouble() const { return m_type == json_double; };
    bool isString() const { return m_type == json_string; };
    bool isArray() const { return m_type == json_array; };
    bool isObject() const { return m_type == json_object; };

    // 动作函数
    void append(const Json&);           // 追加
    bool has(int);                      // 有无当前索引
    bool has(const char*);              
    bool has(const string&);            
    void remove(int);                   // 删除索引元素
    void remove(const char*);
    void remove(const string&);
    void copy(const Json&);             // type的switch
    void clear();                       // 释放内存
    string str() const;                 // 返回json的内容
    void parse(const string&);          // 调用解析器
    
    // 数组迭代器
    typedef vector<Json>::iterator iterator;
    iterator begin() { return m_value.m_array->begin(); }
    iterator end() { return m_value.m_array->end(); }
    

private:
    // 元素联合体（某时刻只有一个元素，节约空间）
    union Value{
        bool m_bool;
        int m_int;
        double m_double;
        string *m_string;
        vector<Json>* m_array;
        map<string, Json>* m_object;
    };
    
    // Json所包含的字段
    Type m_type;       // 类型
    Value m_value;     // 值
}; 

}
}