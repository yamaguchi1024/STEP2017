#include <cstdio>
#include <algorithm>
#include <iostream>
#include <vector>
#include <string.h>
#include <math.h>
#include <fstream>
#include <assert.h>
#define MAX_V 2048
#define MAX_NUM 100000000.0 
using namespace std;

typedef pair<double,double> P;
vector<P> points;
vector<int> route;
int V = - 1;
int used[MAX_V];

double dist(P s, P e) {
  return sqrt(pow(s.first - e.first, 2.0) + pow(s.second - e.second, 2.0));
}

int find_min(int cur) {
  int res;
  double min = MAX_NUM, tmp;
  for (int i = 0; i < V; i++) {
    if (used[i]) continue;

    if ( (tmp = dist(points[i], points[cur])) < min) {
      min = tmp;
      res = i;
    }
  }

  return res;
}

void greedy() {
  int cur = 0;
  used[0] = 1;
  route.push_back(0);

  for (int i = 0; i < V - 1; i++) {
    cur = find_min(cur);
    route.push_back(cur);
    used[cur] = 1;
  }
}

int next(int v) {
  if (route[V - 1] == v)
    return 0;
  for (int i = 0; i < V - 1; i++)
    if (route[i] == v)
      return route[i+1];
}

void change(int a, int b) {
  int a_i, b_i;
  for (int i = 0; i < V; i++) {
    if (route[i] == a)
      a_i = i;
    if (route[i] == b)
      b_i = i;
  }

  cout << a_i << " " << b_i << endl;
  route[a_i] = b;
  route[b_i] = a;
}

int think(int s1, int d1, int s2, int d2) {
  if (dist(points[s1],points[d2]) + dist(points[s2],points[d1]) < dist(points[s1],points[d1]) + dist(points[s2],points[d2])) {
    change(d1,d2);
    return 1;
  }
  return 0;
}

void opt() {
  for(int h = 0;h < 1;h++) {
    int better = 1;
    for (int i = 0; i < V; i++) {
      for (int j = 0; j < V; j++) {
        if (i == j) continue;

        if (think(i, next(i), j, next(j)))
          better = 0;
      }
    }

    if (better) break;
  }
}

int main(int argc, char **argv) {
  if (argc < 2) {
    cout << "./main <filename>" << endl;
    return 0;
  }

  double x,y;
  ifstream ips(argv[1]);
  if (ips.fail())
    perror("file open error");
  while(!ips.eof()) {
    ips >> x >> y;
    points.push_back(P(x,y));
    V++;
  }

  greedy();
  change(0, 1993);
  change(1993, 0);

//  opt();

  cout << "index" << endl;
  for (int i : route)
    cout << i << endl;

  return 0;
}
