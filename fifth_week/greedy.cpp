#include <cstdio>
#include <algorithm>
#include <iostream>
#include <vector>
#include <string.h>
#include <math.h>
#include <fstream>
#include <assert.h>
#define MAX_V 4194304
#define MAX_NUM 10000000000.0 
using namespace std;

typedef pair<double,double> P;
vector<P> points;
vector<int> route;
int V = - 1;
int used[MAX_V];

double dist(P s, P e) {
  return pow(s.first - e.first, 2.0) + pow(s.second - e.second, 2.0);
}

double D(int a, int b) {
  return sqrt(dist(points[route[a]], points[route[b]]));
}

double total_dist() {
  double res = 0.0;
  for (int i = 1; i < V; i++)
    res += D(i-1, i);
  res += D(0, V-1);
  return res;
}

int find_min(int cur) {
  int res;
  double min = MAX_NUM, tmp;
  int look = 0;
  for (int i = 1; i < V; i++) {
    if (used[i]) continue;

    if (look > 1) break;
    look++;
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
    if (i%1000 == 0)
      cerr << i << endl;
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
  ips.ignore(1000, '\n');

  while(!ips.eof()) {
    ips >> x;
    ips.ignore(1000, ',');
    ips >> y;
    points.push_back(P(x,y));
    V++;
  }

  greedy();

  cerr << fixed << total_dist() << endl;

  cout << "index" << endl;
  for (int r : route)
    cout << r << endl;

  return 0;
}
