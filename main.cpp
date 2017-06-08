#include <cstdio>
#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <algorithm>
#include <functional>
#include <vector>

using namespace std;

typedef pair<string, string> P;
// 辞書は赤黒木でkey, valueで管理する
map<string, string> dic;

// Make dictionary
void makedic(void) {
    string line;
    ifstream dict ("./dict");
    if (dict.is_open()) {
        while ( getline (dict,line) ) {
            string val = line;
            transform(line.begin(), line.end(), line.begin(), ::toupper);

            // Substitute any QU to Q
            for(;;) {
                int flag = 1;
                size_t qu = line.find("QU");
                if(qu != string::npos) {
                    line.replace(qu, 2, "Q");
                    flag = 0;
                }
                if(flag) break;
            }

            if(line.length() > 16)
                continue;
            sort(line.begin(), line.end());

            dic.insert(P(line, val));
        }
        dict.close();
    }
    return;
}

int count_value(string tmp) {
    char s1[] = {'X','Z','Q','K','J'};
    char s2[] = {'Y','F','H','C','W','M','P','V','L'};
    char s3[] = {'A', 'B', 'D', 'E', 'G', 'I', 'N', 'O', 'R', 'S', 'T', 'U'};
    int res = 0;
    string input = tmp;

    for(;;) {
        int flag = 1;
        for(char c : s1) {
            int find = input.find(c);
            if(find != string::npos) {
                input.erase(find, 1);
                res += 3;
                flag = 0;
            }
        }
        for(char c : s2) {
            int find = input.find(c);
            if(find != string::npos) {
                input.erase(find, 1);
                res += 2;
                flag = 0;
            }
        }
        for(char c : s3) {
            int find = input.find(c);
            if(find != string::npos) {
                input.erase(find, 1);
                res += 1;
                flag = 0;
            }
        }
        if(flag) break;
    }
    return (res+1)*(res+1);
}

void print_max(string input) {
    int max = 0;
    string max_s = "";

    for(int i = 0; i < (1 << 16); i++) {
        char s[16] = {} ;
        for(int j = 0; j < 16; j++) {
            if(i & (1 << j)) {
                s[j] = input[j];
            }
        }
        string key = "";
        for(int j = 0; j < 16; j++)
            if(s[j])
                key += s[j];

        auto d = dic.find(key);
        if(d != dic.end() ) {
            string tmp = d -> second;
            transform(tmp.begin(), tmp.end(), tmp.begin(), ::toupper);
            int c = count_value(tmp);
            if( c > max ) {
                max = c;
                max_s = d -> second;
            }
        }
    }

    cout << max_s << " " << max << endl;
    return;
}

int main() {
    makedic();

    // Compare user input and dictionary
    for(;;) {
        string input;
        string s;
        for(int i = 0; i < 16; i++){
            cin >> s;
            input += s;
        }
        transform(input.begin(), input.end(), input.begin(), ::toupper);

        // Substitute any QU to Q
        for(;;) {
            int flag = 1;
            size_t qu = input.find("QU");
            if(qu != string::npos) {
                input.replace(qu, 2, "Q");
                flag = 0;
            }
            if(flag) break;
        }
        sort(input.begin(), input.end());

        print_max(input);
    }
    return 0;
}
