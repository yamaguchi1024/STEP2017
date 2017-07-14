#include <cstdio>
#include <vector>
#include <algorithm>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
// Me is always 1

using namespace std;
typedef pair<int,int> P;
int Eval[8][8] =
{ {  121, -30, 8, 7, 7, 8, -30, 121 },
  {  -30, -24, 4, 4, 4, 4, -24, -30 }, 
  {  8, 4, 7,  4,  4,  7,  4, 8 },
  {  7,  4, 4,  0,  0,  4,  4, 7 }, 
  {  7,  4, 4,  0,  0,  4,  4, 7 },
  {  8,  4, 7,  4,  4,  7,  4, 8 },
  {  -30, -24,  4, 4, 4, 4, -24,  -30 },
  {  121, -30, 8,  7,  7,  8,  -30, 121 } };

void Eval_reset(char Board[8][8]) {
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      if (Board[0][0] == '1') {
        Eval[0][1] = 120;
        Eval[1][0] = 120;
        Eval[1][1] = 120;
        Eval[0][2] = 30;
        Eval[2][0] = 30;
        Eval[1][2] = 28;
        Eval[2][1] = 28;
        Eval[0][6] = 28;
        Eval[6][0] = 28;
      }
      else if (Board[0][7] == '1') {
        Eval[0][6] = 120;
        Eval[1][7] = 120;
        Eval[1][6] = 120;
        Eval[0][5] = 30;
        Eval[2][7] = 30;
        Eval[1][5] = 28;
        Eval[2][6] = 28;
        Eval[0][1] = 20;
        Eval[6][5] = 20;
      }
      else if (Board[7][0] == '1') {
        Eval[6][0] = 120;
        Eval[7][1] = 120;
        Eval[6][1] = 120;
        Eval[5][0] = 30;
        Eval[7][2] = 30;
        Eval[5][1] = 28;
        Eval[6][2] = 28;
      }
      else if (Board[7][7] == '1') {
        Eval[7][6] = 120;
        Eval[6][7] = 120;
        Eval[6][6] = 120;
        Eval[7][5] = 30;
        Eval[5][7] = 30;
        Eval[5][6] = 28;
        Eval[6][5] = 28;
      }
    }
  }
}

static void connect_alarm(int signo)
{
  printf("PASS\n");  
  exit(0);
}

int Calc_eval(const char Board[8][8]) {
  int res = 0;
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      if (Board[i][j] == '*') continue;
      else if (Board[i][j] == '0') res -= Eval[i][j];
      else res += Eval[i][j];
    }
  }
  return res;
}

bool V(const P p) {
  if (p.first >= 0 && p.first < 8 && p.second >= 0 && p.second < 8) return true;
  return false;
}

vector<pair<P, int> > Placable(const char Board[8][8]) {
  vector<pair<P, int> > vec;
  int dx[] = {-1,0,1,-1,1,-1,0,1}, dy[] = {-1,-1,-1,0,0,1,1,1};
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      int count = 0;
      for(int d = 0; d < 8; d++) {
        if (Board[i][j] != '*') continue;
        P p = P(i,j);
        p.first += dx[d], p.second += dy[d];
        if (! (V(p) && Board[p.first][p.second]=='0')) continue;
        int tmp = 0;
        while (V(p) && Board[p.first][p.second]=='0') {
          tmp++;
          p.first += dx[d], p.second += dy[d];
        }
        if (V(p) && Board[p.first][p.second]=='1')
          count += tmp;
      }
      if (count > 0)
        vec.push_back(make_pair(P(i,j), count));
    }
  }
  return vec;
}

int Cstone(char Board[8][8]) {
  int cnt = 0;
  for(int i = 0; i < 8; i++)
    for(int j = 0; j < 8; j++)
      if(Board[i][j] != '*')
        cnt++;
  return cnt;
}

bool sort_greater(const pair<P,int> &left, const pair<P,int> &right){
  return left.second > right.second;
}

int main() {
  signal(SIGALRM, connect_alarm);
  alarm(12);

  char Board[8][8];
  for(int i = 0; i < 8; i++) scanf("%s", Board[i]);
  Eval_reset(Board);

  vector<pair<P,int> > vec = Placable(Board);
  if (vec.empty()) {
    printf("PASS");
    return 0;
  }

  // 終盤だったら
  /*
  if (Cstone(Board) > 55) {
    sort(vec.begin(),vec.end(),sort_greater);
    P p = vec[0].first;
    printf("[%d,%d]",p.second + 1, p.first + 1);
    return 0;
  }
  */

  if (Cstone(Board) > 40) {
    P max_p;
    int max_n = -500000;
    int max_s = -500000;
    for (int i = 0; i < vec.size(); i++) {
      P p = vec[i].first;
      int t = Eval[p.first][p.second]*2 + vec[i].second;
      if (t > max_n || (t == max_n && max_s < vec[i].second)) {
        max_n = t;
        max_p = p;
        max_s =  vec[i].second;
      }
    }
    printf("[%d,%d]",max_p.second + 1, max_p.first + 1);
    return 0;
  }

  P max_p;
  int max_n = -500000;
  int max_s = -500000;
  for (int i = 0; i < vec.size(); i++) {
    P p = vec[i].first;
    int t = Eval[p.first][p.second] + vec[i].second;
    if (t > max_n || (t == max_n && max_s < vec[i].second)) {
      max_n = t;
      max_p = p;
      max_s =  vec[i].second;
    }
  }
  printf("[%d,%d]",max_p.second + 1, max_p.first + 1);
  return 0;
}
