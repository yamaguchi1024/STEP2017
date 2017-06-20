#include <cstdio>
#include <algorithm>
#include <iostream>
#include <vector>
#include <string.h>
#include <fstream>
using namespace std;

#define MAX_V 1483277
int V = 1483277; //node
vector<int> G[MAX_V]; // G[i]はiから伸びている辺の終端のvector
vector<int> rG[MAX_V]; // 辺の向きを逆にしたグラフ
vector<int> vs;
bool used[MAX_V];
int cmp[MAX_V];
string page[MAX_V];

void add_edge(int from, int to) {
    G[from].push_back(to);
    rG[to].push_back(from);
}

void dfs(int v) {
    used[v] = true;
    for (int i = 0; i < G[v].size(); i++) {
        if (!used[G[v][i]]) dfs(G[v][i]);
    }
    vs.push_back(v);
}

vector<int> nodes;
int rdfs(int v, int k, int s) {
    used[v] = true;
    cmp[v] = k;
    nodes.push_back(v);
    for (int i = 0; i < rG[v].size(); i++) {
        if (!used[rG[v][i]]) return rdfs(rG[v][i], k, ++s);
    }
    return s;
}

void scc() {
    memset(used, 0, sizeof(used));
    vs.clear();
    for (int v = 0; v < V; v++) {
        if (!used[v]) dfs(v);
    }
    memset(used, 0, sizeof(used));
    int size = 0, max = 0,tmp,max_n;
    vector<int> res;
    for (int i = vs.size() - 1; i >= 0; i--) {
        if (!used[vs[i]]) {
            nodes.clear();
            if( (tmp = rdfs(vs[i], size++, 1)) > max ) {
                max = tmp;
                max_n = size - 1;
                res = nodes;
            }
        }
    }
    printf("強連結成分の個数 %d\n",size);
    printf("最大の強連結成分のノードの数 %d\n",max);
    printf("強連結成分のノード:\n\n");
    for(int i = 0; i < res.size(); i++) {
        cout << page[res.back()] << endl;
        res.pop_back();
    }
    return;
}

int main() {
    int from,to;
    int num;
    string name;

    ifstream ips("pages.txt");
    if (ips.fail())
        perror("pages open error");
    while(!ips.eof()) {
        ips >> num >> name;
        page[num]=name;
    }

    ifstream ifs("links.txt");
    if (ifs.fail())
        perror("links open error");
    while(!ifs.eof()) {
        ifs >> from >> to;
        add_edge(from,to);
    }

    scc();

    return 0;
}
