#include <cstdio>
#include <algorithm>
#include <iostream>
#include <vector>
#include <string.h>
#include <math.h>
#include <fstream>
#include <assert.h>
#define MAX_V (1 << 22)
#define MAX_NUM 10000000000.0 
#define DIVIDE 10000 
using namespace std;

typedef pair<double,double> P;
vector<P> points;
vector<int> route;
int V = - 1;
int cur,End;
int used[MAX_V];

double dist(P s, P e) {
  return pow(s.first - e.first, 2.0) + pow(s.second - e.second, 2.0);
}

int find_min(int cur, int from) {
  int res;
  double min = MAX_NUM, tmp;
  for (int i = from; i <= End; i++) {
    if (used[i]) continue;

    if ( (tmp = dist(points[i], points[cur])) < min) {
      min = tmp;
      res = i;
    }
  }
  return res;
}

void greedy(int from) {
  for (int i = from; i <= End; i++) {
    if ( used[(cur = find_min(cur, from))] )
      continue;
    route.push_back(cur);
    used[cur] = 1;
  }
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

int swap(int i, int j) {
  if (D(i, j-1) + D(j, i+1) < D(i, i+1) + D(j-1, j)) {
    reverse(route.begin() + i + 1, route.begin() + j);
    return 1;
  }
  return 0;
}

void opt(int from) {
  for(int h = 0;h < 1;h++) {
    int better = 1;
    for (int i = from; i <= End - 4; i++)
      for (int j = from + 3; j <= End; j++)
        if (swap(i, j))
          better = 0;
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
  ips.ignore(1000, '\n');

  while(!ips.eof()) {
    ips >> x;
    ips.ignore(1000, ',');
    ips >> y;
    points.push_back(P(x,y));
    V++;
  }

  cur = 0;
  used[0] = 1;
  route.push_back(0);

  for (int i = 0; i < (1 << 23); i+=DIVIDE) {
    End = (i + DIVIDE > V-1) ? V-1 : i + DIVIDE;
    if (i >= V-1) break;
    cerr << i << endl;
    greedy(i);
  }
  for (int i = 1; i < (1 << 23); i+=DIVIDE) {
    End = (i + DIVIDE > V-1) ? V-1 : i + DIVIDE;
    if (i >= V-1) break;
    opt(i);
    cerr << i << endl;
    cerr << fixed << total_dist() << endl;
  }

  cerr << fixed << total_dist() << endl;

  cout << "index" << endl;
  for (int r : route)
    cout << r << endl;

  return 0;
}
