#pragma once

#include "json.h"
#include <string>

using namespace std;
//using namespace JP::Json;

namespace JP{
namespace Json{

class Parser
{
public:
    Parser();

    void load(const string&);   // 将传入的json数据流存于Parser对象
    Json parse();               // 解析json

private:
    void skip_white_space();    // 跳过空白符、回车、制表符等
    char get_next_token();      // 获取下一个字符

    // 解析不同类型的Json数据
    Json parse_null();
    Json parse_bool();
    Json parse_number();
    string parse_string();
    Json parse_array();
    Json parse_object();

private:
    string m_str;       // 获取的数据串
    int m_idx;          // 数据串的下标
};

}
}