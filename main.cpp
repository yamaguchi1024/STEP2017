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

// only most low point charactors
vector<char> c3;
// c3 and Y,F,H,C,W,M,P,V,L
vector<char> c2;
// c2 and X,Z,Qu,K
vector<char> c1;


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
    c1.clear();
    c2.clear();
    c3.clear();

    string input = tmp;

    // make c3
    char s3[] = {'A', 'B', 'D', 'E', 'G', 'I', 'J', 'N', 'O', 'R', 'S', 'T', 'U'};

    int flag;
    for(;;) {
        flag = 1;
        for(char s : s3) {
            size_t find = input.find(s);
            if(find == string::npos)
                continue;
            c3.push_back(s);
            input.erase(find, 1);
            flag = 0;
        } 
        if(flag) break;
    }

    // make c2
    // c3 and Y,F,H,C,W,M,P,V,L
    c2 = c3;
    char s2[] = {'Y','F','H','C','W','M','P','V','L'};

    for(;;) {
        flag = 1;
        for(char s : s2) {
            size_t find = input.find(s);
            if(find == string::npos)
                continue;
            c2.push_back(s);
            input.erase(find, 1);
            flag = 0;
        } 
        if(flag) break;
    }

    // make c1
    c1 = c2;
    for(char s : input) {
        c1.push_back(s);
    }
    return;
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
                if (res != dic.end() )
                return true;
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

string search(string input, int i, int w) {
    // when nothing will be deleted
    if(i == 0) {
        auto z = dic.find(input);
        if (z != dic.end())
            return z->second;
        else
            return "";
    }

    // compare c3(most lower point set) first
    string res;
    if(w == 3)
        if( (res = search_set(input, i, c3)) != "")
            return res;
    if(w == 2)
        if( (res = search_set(input, i, c2)) != "")
            return res;
    if(w == 1)
        if( (res = search_set(input, i, c1)) != "")
            return res;

    return "";
}

int count_value(string tmp) {
    char s1[] = {'X','Z','Q','K'};
    char s2[] = {'Y','F','H','C','W','M','P','V','L'};
    char s3[] = {'A', 'B', 'D', 'E', 'G', 'I', 'J', 'N', 'O', 'R', 'S', 'T', 'U'};
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
    return res;
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

        string res;
        string tmp;
        int max[3] = {0};
        string maxs[3];
        for(int i = 0; i < input.length(); i++) {
            for (int j = 0; j < 3; j++) {
                if((max[j] == 0) && (res = search(input, i, 3 - j)) != "") { 
                    tmp = res;
                    transform(tmp.begin(), tmp.end(), tmp.begin(), ::toupper);
                    max[j] = count_value(tmp);
                    maxs[j] = res;
                }
            }
            if((max[0] != 0) && (max[1] != 0) && (max[2] != 0))
                break;
        }
        if(max[0] >= max[1] && max[0] >= max[2])
            cout << maxs[0] << " " << max[0] << endl;
        if(max[1] >= max[0] && max[1] >= max[2])
            cout << maxs[1] << " " << max[1] << endl;
        else
            cout << maxs[2] << " " << max[2] << endl;
    }
    return 0;
}
