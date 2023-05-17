#include "parser.h"

using namespace JP::Json;

Parser::Parser() : m_str(""), m_idx(0){
}

void Parser::load(const string& str){
    m_str = str;
    m_idx = 0;
}

Json Parser::parse(){
    char ch = get_next_token();
    switch (ch)
    {
        case 'n':           // 空值 null
            --m_idx;
            return parse_null();
        case 't':           // bool值
        case 'f':      
            --m_idx;
            return parse_bool();
        case '-':           // number值
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            --m_idx;
            return parse_number();
        case '"':           // 字符串
            return Json(parse_string());
        case '[':
            return parse_array();
        case '{':
            return parse_object();
        default:
            break;
    }
    throw logic_error("unexpected char");
}

void Parser::skip_white_space(){
    while(m_str[m_idx] == ' ' || m_str[m_idx] == '\n' 
        || m_str[m_idx] == '\r' || m_str[m_idx] == '\t'){
            m_idx++;
        }
}

char Parser::get_next_token(){
    skip_white_space();
    return m_str[m_idx++];
}


// 解析不同类型的Json数据-------------------------------------------------
Json Parser::parse_null(){
    if(m_str.compare(m_idx, 4, "null") == 0){
        m_idx += 4;             // 跳过"null"
        return Json();   
    }
    throw logic_error("parse null error");
}
Json Parser::parse_bool(){
    if(m_str.compare(m_idx, 4, "true") == 0){
        m_idx += 4;             // 跳过"true"
        return Json(true);
    }
    else if(m_str.compare(m_idx, 5, "false") == 0){
        m_idx += 5;             // 跳过"false"
        return Json(false);
    }
    throw logic_error("parse bool error");
}
Json Parser::parse_number(){
    size_t pos = m_idx;         // 记录起始下标，便于后面返回

    if (m_str[m_idx] == '-') m_idx++;
    if (m_str[m_idx] == '0') m_idx++;   // 前导0
    else if (m_str[m_idx] >= '1' && m_str[m_idx] <= '9'){
        m_idx++;
        while (m_str[m_idx] >= '0' && m_str[m_idx] <= '9')
            m_idx++;
    }
    else throw std::logic_error("invalid character in number");

    if (m_str[m_idx] != '.')    // 非浮点数
        return Json(std::atoi(m_str.c_str() + pos));    // 返回整数

    m_idx++;    // 浮点数
    if (m_str[m_idx] < '0' || m_str[m_idx] > '9')
        throw std::logic_error("at least one digit required in fractional part");

    while (m_str[m_idx] >= '0' && m_str[m_idx] <= '9')
        m_idx++;

    return Json(std::atof(m_str.c_str() + pos));    // 返回浮点数
}
string Parser::parse_string(){
    string out;
    while (true){
        char ch = m_str[m_idx++];
        if (ch == '"') break;
    
        // 如果是转义字符
        if (ch == '\\'){
            ch = m_str[m_idx++];
            switch (ch){
                case '\b':
                    out += '\b';
                    break;
                case '\t':
                    out += '\t';
                    break;
                case '\n':
                    out += '\n';
                    break;
                case '\f':
                    out += '\f';
                    break;
                case '\r':
                    out += '\r';
                    break;
                case '"':
                    out += "\\\"";
                    break;
                case '\\':
                    out += "\\\\";
                    break;
                case 'u':
                    out += "\\u";
                    break;
                default:
                    break;
            }
        }
        else out += ch;
    }
    return out;
}
Json Parser::parse_array(){
    Json arr(json_array);
    char ch = get_next_token();
    if(ch == ']')   return arr;     // 空数组
    m_idx--;
    while(true){
        arr.append(parse());
        ch = get_next_token();
        if(ch == ']')   break;
        if(ch != ',')
            throw logic_error("parse array error");
        m_idx++;
    }
    return arr;
}
Json Parser::parse_object(){
    Json obj(json_object);
    char ch = get_next_token();
    if (ch == '}') return obj;
    
    m_idx--;
    while (true){
        ch = get_next_token();
        if (ch != '"')      // 以双引号开始
            throw std::logic_error("expected '\"' in object");

        string key = parse_string();   
        ch = get_next_token();
        if (ch != ':')      // 键值和元素值之间永冒号分隔
            throw std::logic_error("expected ':' in object");

        obj[key] = parse(); // 解析键值所对应的值
        ch = get_next_token();
        if (ch == '}') break;
        
        if (ch != ',')      // 如果解析没结束，键值对之间用逗号隔开
            throw std::logic_error("expected ',' in object");
        //m_idx++;
    }
    return obj;
}