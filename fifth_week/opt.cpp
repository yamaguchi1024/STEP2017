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

double distance(P s, P e) {
  return sqrt(pow(s.first - e.first, 2.0) + pow(s.second - e.second, 2.0));
}

int find_min(int cur) {
  int res;
  double min = MAX_NUM, tmp;
  for (int i = 0; i < V; i++) {
    if (used[i]) continue;

    if ( (tmp = distance(points[i], points[cur])) < min) {
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

  cout << "index" << endl;
  for (int i : route)
    cout << i << endl;

  return 0;
}
