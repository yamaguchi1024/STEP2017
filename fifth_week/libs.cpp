#include <cstdio>
#include <algorithm>
#include <iostream>
#include <vector>
#include <string.h>
#include <math.h>
#include "libs.h"
#define MAX_V 2048
#define MAX_NUM 100000000.0 
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

double D(int a, int b) {
  return dist(points[a],points[b]);
}

int next(int v) {
  if (route[V - 1] == v)
    return 0;
  for (int i = 0; i < V - 1; i++)
    if (route[i] == v)
      return route[i+1];
}

int R(int v) {
  for (int i = 0; i < V - 1; i++)
    if (route[i] == v)
      return i;
}
