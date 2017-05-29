#include <cstdio>
#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <algorithm>
using namespace std;

typedef pair<string, string> P;

int main() {
    map<string, string> dic;

    // Make dictionary
    string line;
    ifstream dict ("/usr/share/dict/american-english");
    if (dict.is_open())
    {
        while ( getline (dict,line) )
        {
            if(line.length() > 16)
                continue;
            if(line.find("'") != string::npos)
                continue;
            string val = line;
            sort(line.begin(), line.end());
            dic.insert(P(line, val));
        }
        dict.close();
    }

    // Compare user input and dictionary
    for(;;) {
        cout << ">> ";
        string input;
        cin >> input;

        auto res = dic.find(input);
        if ( res != dic.end() )
            cout << res->second << endl;
        else
            cout << "not found..." << endl;
    }

    return 0;
}
