#include <iostream>
#include "json.h"
#include "parser.h"

using namespace std;
using namespace JP::Json;

int main(){
    Json arr;
    arr.append(true);
    arr.append(1);
    arr.append("abc");

    Json obj;
    obj["lbj"] = "win1";
    obj["ad"] = "win2";
    obj["rv"] = "win3";
    obj["vb"] = "win4";

    cout << arr.str() << endl;

    // 只释放obj即可，因为浅拷贝
    // arr的内存会自动转移到obj的内存
    obj.clear();    
    
    return 0;
}