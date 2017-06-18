#include <cstdio>
#include <algorithm>
#include <iostream>
#include <vector>
#include <string.h>
#include <fstream>
using namespace std;

#define MAX_V 2000000
int V = 0; //node
vector<int> G[MAX_V]; // G[i]はiから伸びている辺の終端のvector
vector<int> rG[MAX_V]; // 辺の向きを逆にしたグラフ
vector<int> vs;
bool used[MAX_V];
int cmp[MAX_V];

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

void rdfs(int v, int k) {
    used[v] = true;
    cmp[v] = k;
    for (int i = 0; i < rG[v].size(); i++) {
        if (!used[rG[v][i]]) rdfs(rG[v][i], k);
    }
}

int scc() {
    memset(used, 0, sizeof(used));
    vs.clear();
    for (int v = 0; v < V; v++) {
        if (!used[v]) dfs(v);
    }
    memset(used, 0, sizeof(used));
    int k = 0;
    for (int i = vs.size() - 1; i >= 0; i--) {
        if (!used[vs[i]]) rdfs(vs[i], k++);
    }
    return k;
}

int main() {
    ifstream ifs("links.txt");
    if (ifs.fail())
        perror("links open error");

    int from,to;

    while(!ifs.eof()) {
        ifs >> from >> to;
        add_edge(from,to);
        V++;
        printf("%d\n",V);
    }

    printf("%d\n", scc());

    return 0;
}
