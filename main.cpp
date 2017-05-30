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

vector<char> c;

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

void makeset(string tmp) {
    c.clear();
    string input = tmp;

    // make c
    for(char s : input) {
        c.push_back(s);
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

// コンビネーションを求める。
// functionがtrueならそこでやめる。
// subsetを後ろから埋めていく
bool for_each_combination(vector<char>::iterator begin,
        vector<char>::iterator end,
        int n,
        function<bool(vector<char> &)> callback,
        vector<char> &subset) {

    if(n < 0)
        return callback(subset);

    if(end - begin == n) {
        subset[n] = *begin;
        return for_each_combination(begin + 1, end, n - 1, callback, subset);
    }

    if(for_each_combination(begin + 1, end, n, callback, subset))
        return true;

    subset[n] = *begin;
    return for_each_combination(begin + 1, end, n - 1, callback, subset);

}

// 受け取ったcに対してfor_each_combinationを適用する
string search_set(string tmp, int i, vector<char> &c) {
    string input = tmp;
    // important
    if (i > c.size())
        return "";

    int max = 0;
    vector<char> subset(i);
    if(
            for_each_combination(c.begin(), c.end(), subset.size() - 1,
                // functionはsubstringが辞書にあればtrue,なければfalseを返す。
                [=](vector<char> &comb) {
                string str = input;
                for(char c : comb) {
                    int find = str.find(c);
                    if(find != string::npos)
                    str.erase(find, 1);
                }
                auto res = dic.find(str);
                if (res != dic.end() ) {
                    transform(str.begin(), str.end(), str.begin(), ::toupper);
                    if(count_value(str) > max)
                        return true;
                    else
                        return false;
                }
                else
                    return false;
                },
                subset) ) {

        for(char c : subset) {
            int find = input.find(c);
            if(find != string::npos)
                input.erase(find, 1);
        }
        auto res = dic.find(input);
        if (res != dic.end() )
            return res->second;
        else
            return "";
    }
    return "";
}

string search(string input, int i) {
    // when nothing will be deleted
    if(i == 0) {
        auto z = dic.find(input);
        if (z != dic.end())
            return z->second;
        else
            return "";
    }

    string res;
    if( (res = search_set(input, i, c)) != "")
        return res;

    return "";
}

int main() {
    makedic();

    // Compare user input and dictionary
    for(;;) {
        string input;
        cin >> input;
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

        // c3から使う
        makeset(input);

        string res,tmp;
        int max = 0;
        string max_s = "";
        for(int i = 0; i < input.length(); i++) {
                if( (res = search(input, i)) != "") { 
                    tmp = res;
                    transform(tmp.begin(), tmp.end(), tmp.begin(), ::toupper);
                    int huga;
                    if( (huga = count_value(tmp)) > max) {
                        max_s = res;
                        max = huga;
                    }
            }
        }
        cout << max_s << " " << max << endl;
    }
    return 0;
}
