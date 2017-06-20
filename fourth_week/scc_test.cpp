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

int rdfs(int v, int k, int size) {
    used[v] = true;
    cmp[v] = k;
    for (int i = 0; i < rG[v].size(); i++) {
        if (!used[rG[v][i]]) return rdfs(rG[v][i], k, ++size);
    }
    return size;
}

int scc() {
    memset(used, 0, sizeof(used));
    vs.clear();
    for (int v = 0; v < V; v++) {
        if (!used[v]) dfs(v);
    }
    memset(used, 0, sizeof(used));
    printf("第一回終わり\n");
    int k = 0, max = 0,tmp;
    for (int i = vs.size() - 1; i >= 0; i--) {
        if (!used[vs[i]]) {
            if( (tmp = rdfs(vs[i], k++, 1)) > max )
                max = tmp;
        }
    }
    printf("max: %d\n",max);
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
    }

    printf("%d\n", scc());

    return 0;
}
