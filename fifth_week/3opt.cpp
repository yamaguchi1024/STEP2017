#include <cstdio>
#include <algorithm>
#include <iostream>
#include <vector>
#include <string.h>
#include <math.h>
#include <fstream>
#include <assert.h>
#define MAX_V 2048
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

int swap(int i, int j, int k) {
  //cerr << i << " " << j << " " << k << endl;
  double R[7];
  double min = MAX_NUM;
  int pos;
  R[0] = D(i,i+1) + D(j,j+1) + D(k-1,k);
  R[1] = D(i,j) + D(i+1,j+1) + D(k-1,k);
  R[2] = D(i,j) + D(i+1,k-1) + D(j+1,k);
  R[3] = D(i,j+1) + D(k-1,i+1) + D(j,k);
  R[4] = D(i,j+1) + D(k-1,j) + D(i+1,k);
  R[5] = D(i,k-1) + D(j+1,i+1) + D(j,k);
  R[6] = D(i,k-1) + D(j+1,j) + D(i+1,k);
  for (int u = 0; u < 7; u++)
    if (R[u] < min) {
      min = R[u];
      pos = u;
    }
  switch (pos) {
    case 1:
      reverse(route.begin() + i+1, route.begin() + j+1);
      return 1;
    case 2:
      reverse(route.begin() + i+1, route.begin() + j+1);
      reverse(route.begin() + j+1, route.begin() + k);
      return 1;
    case 3:
      rotate(route.begin() + i+1, route.begin() + j+1, route.begin() + k);
      return 1;
    case 4:
      reverse(route.begin() + i+1, route.begin() + j+1);
      rotate(route.begin() + i+1, route.begin() + j+1, route.begin() + k);
      return 1;
    case 5:
      reverse(route.begin() + j+1, route.begin() + k);
      rotate(route.begin() + i+1, route.begin() + j+1, route.begin() + k);
      return 1;
    case 6:
      reverse(route.begin() + i+1, route.begin() + j+1);
      reverse(route.begin() + j+1, route.begin() + k);
      rotate(route.begin() + i+1, route.begin() + j+1, route.begin() + k);
      return 1;
  }
  return 0;
}

void opt() {
  for(;;) {
    int better = 1;
    for (int i = 1; i < V - 6; i++) {
      for (int j = i + 3; j < V - 4; j++)
        for (int k = j + 3; k < V; k++)
          if (swap(i, j, k))
            better = 0;
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
  ips.ignore(1000, '\n');

  while(!ips.eof()) {
    ips >> x;
    ips.ignore(1000, ',');
    ips >> y;
    points.push_back(P(x,y));
    V++;
  }

  greedy();

  opt();

  cerr << fixed << total_dist() << endl;

  cout << "index" << endl;
  for (int r : route)
    cout << r << endl;

  return 0;
}
