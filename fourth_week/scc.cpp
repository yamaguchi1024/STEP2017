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
vector<int> queue;

void add_edge(int from, int to) {
    G[from].push_back(to);
    rG[to].push_back(from);
}

int rdfs(int v, int scc_num, int size) {
    used[v] = true;
    for (int i = 0; i < rG[v].size(); i++) {
        if (!used[rG[v][i]]) return rdfs(rG[v][i], scc_num, size++);
    }
    return size;
}

void scc() {
    memset(used, 0, sizeof(used));

    vs.clear();
    queue.clear();
    std::vector<int> queue;
    for (int v = 0; v < V; v++) {
        if (!used[v]) {
            used[v] = true;
            queue.push_back(v);
        }
        while (!queue.empty()) {
            int v = queue.back(); queue.pop_back();
            vs.push_back(v);
            for (int i = 0; i < G[v].size(); i++)
                if (!used[G[v][i]]) {
                    used[v] = true;
                    queue.push_back(G[v][i]);
                }
        }
    }

    printf("dfs1回目 終了\n");

    memset(used, 0, sizeof(used));
    int scc_num = 0, max_size = 0, tmp;
    for (int i = 0; i < vs.size(); i++) {
        if (!used[vs[i]])
            if( (tmp = rdfs(vs[i], scc_num++, 1)) > max_size )
                max_size = tmp;
    }
    printf("強連結成分の数: %d\n",scc_num);
    printf("最大の強連結成分の大きさ: %d\n",max_size);
    return;
}

int main() {
    ifstream ifs("links.txt");
    if (ifs.fail())
        perror("links open error");

    int from,to;
    int s = 52973671;

    int i = 0;
    while(!ifs.eof()) {
 //       if(i >= (s/10000)) break;
        ifs >> from >> to;
        add_edge(from,to);
        i++;
    }

    scc();

    return 0;
}
