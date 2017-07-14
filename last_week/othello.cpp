#include <cstdio>
#include <vector>
#include <algorithm>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <cstring>
#include <stdlib.h>
// Me is always 1

using namespace std;
typedef pair<int,int> P;
vector<pair<P,int> > vec;
int Eval[8][8] =
{ {  99, -8, 8, 6, 6, 8, -8, 99 },
  {  -8, -24, -4, -3, -3, -4, -24, -8 }, 
  {  8, -4, 7,  4,  4,  7,  -4, 8 },
  {  6,  -3, 4,  0,  0,  4,  -3, 6 }, 
  {  6,  -3, 4,  0,  0,  4,  -3, 6 },
  {  8,  -4, 7,  4,  4,  7,  -4, 8 },
  {  -8, -24,  -4, -3, -3, -4, -24,  -8 },
  {  99, -8, 8,  6,  6,  8,  -8, 99 } };

bool sort_greater(const pair<P,int> &left, const pair<P,int> &right){
  return left.second > right.second;
}

static void connect_alarm(int signo)
{
  sort(vec.begin(),vec.end(),sort_greater);
  P p = vec[0].first;
  printf("%c%d\n",p.second + 'A', p.first + 1);

  exit(0);
}

int Calc_eval(const char Board[8][8]) {
  int res = 0;
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      if (Board[i][j] == '*') continue;
      else if (Board[i][j] == '1') res ++;
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

int calmax(const vector<pair<P,int> > &vec) {
  P max_p;
  int max_n = -500000;
  for (int i = 0; i < vec.size(); i++) {
    P p = vec[i].first;
    int t = Eval[p.first][p.second] + vec[i].second;
    if (t > max_n) {
      max_n = t;
      max_p = p;
    }
  }
  return max_n;
}

int minmax(char Board[8][8], int player, int depth) {
  if (depth == 0) {
    return Calc_eval(Board);
  }

  if (player == 1) {
    vector<pair<P,int> > newvec = Placable(Board);

    int max_b = -5000;
    P max_p;
    for (int i = 0; i < newvec.size(); i++) {
      char newBoard[8][8];
      memcpy(newBoard, Board, 64);
      P p = newvec[i].first;
      newBoard[p.first][p.second] = 1;
      int t = minmax(newBoard, 0, depth - 1);
      if (max_b < t) {
        max_b = t;
        max_p = p;
      }
    }
    return max_b;
  }

  char revBoard[8][8];
  memcpy(revBoard, Board, 64);
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      if(revBoard[i][j] == '1')
        revBoard[i][j] = '0';
      else if(revBoard[i][j] == '0')
        revBoard[i][j] = '1';
    }
  }

  vector<pair<P,int> > newvec = Placable(revBoard);
  int max_b = -5000;
  P max_p;
  for (int i = 0; i < newvec.size(); i++) {
    char newBoard[8][8];
    memcpy(newBoard, Board, 64);
    P p = newvec[i].first;
    newBoard[p.first][p.second] = 0;
    int t = minmax(newBoard, 1, depth - 1);
    if (max_b < -t) {
      max_b = -t;
      max_p = p;
    }
  }
  return max_b;
}

int main() {
  signal(SIGALRM, connect_alarm);
  alarm(1000);

  char Board[8][8];
  for(int i = 0; i < 8; i++) scanf("%s", Board[i]);
  vec = Placable(Board);

  if (vec.empty()) {
    printf("PASS");
    return 0;
  }

  // 終盤だったら
  if (Cstone(Board) > 40) {
    sort(vec.begin(),vec.end(),sort_greater);
    P p = vec[0].first;
    printf("%c%d\n",p.second + 'A', p.first + 1);
    return 0;
  }

  int max_b = -5000;
  P max_p;
  for (int i = 0; i < vec.size(); i++) {
    char newBoard[8][8];
    memcpy(newBoard, Board, 64);
    P p = vec[i].first;
    newBoard[p.first][p.second] = 1;
    int t = minmax(newBoard, 0, 3);
    if (max_b < t) {
      max_b = t;
      max_p = p;
    }
  }
  printf("%c%d\n",max_p.second + 'A', max_p.first + 1);

  return 0;
}
