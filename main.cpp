#include <iostream>
#include <fstream>
#include <sstream>
#include "json.h"
#include "parser.h"

using namespace std;
using namespace JP::Json;

int main(){
    ifstream fin("./test.json");
    stringstream ss;

    ss << fin.rdbuf();
    const string& str = ss.str();
    
    Json v;
    v.parse(str);

    bool isLogin = v["data"]["isLogin"];
    int current_level = v["data"]["level_info"]["current_level"];
    string uname = v["data"]["uname"];

    cout << v.str() << endl;
    return 0;
}