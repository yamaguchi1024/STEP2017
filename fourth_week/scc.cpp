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
vector<int> vs; // 通った順に入れていく。sccが番号つけるのに対応してる
bool used[MAX_V]; // DFSなので
string page[MAX_V]; // 題名
vector<int> nodes; // 順番を保存して表示するため
vector<int> all[MAX_V];

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

int rdfs(int v, int k, int s) {
    used[v] = true;
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
            all[size - 1] = nodes;
        }
    }
    printf("強連結成分の個数 %d\n",size);
    printf("最大の強連結成分のノードの数 %d\n",max);
    printf("最大の強連結成分のノード:\n\n");
    for(int i = 0; i < res.size(); i++) {
        cout << page[res.back()] << endl;
        res.pop_back();
    }

    printf("ノード数が1でない全ての強連結成分を表示するよ:\n\n");
    for (int i = 0; i < size; i++) {
      if (all[i].size() <= 2)
        continue;         
      printf("%d 個目の強連結成分 大きさ: %d \n\n",i,all[i].size()-1);  
      for (int j = 0; j < all[i].size(); j++) {
        cout << page[all[i].back()] << endl;
        all[i].pop_back();
      }
      printf("\n");  
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
