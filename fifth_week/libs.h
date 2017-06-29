#include <cstdio>
#include <algorithm>
#include <iostream>
#include <vector>
#include <string.h>
#include <math.h>
#include <fstream>
#include <assert.h>
using namespace std;
typedef pair<double,double> P;

double dist(P s, P e);
int find_min(int cur);
int next(int v);
int R(int v);
double D(int a, int b);
