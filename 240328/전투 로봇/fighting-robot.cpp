#include <iostream>
#include <vector>
#include <tuple>
#include <queue>
#include <algorithm>
#define X first
#define Y second
using namespace std;
int n, rx ,ry, eatNum;
int board[22][22];
int dx[4] = { 1, 0, -1 , 0 };
int dy[4] = { 0, 1, 0, -1 };
int rLevel = 2; 

bool OOB(int x, int y) {
	return (x < 0 || x >= n || y < 0 || y >= n);
}
void Bfs(vector<tuple<int, int, int>>& possibleMonster) {
	int dist[22][22];
	for (int i = 0; i < n; i++) fill(dist[i], dist[i] + n, -1);
	queue<pair<int, int>> q;
	q.push({ rx,ry });
	dist[rx][ry] = 0;
	
	while (!q.empty()) {
		auto cur = q.front(); q.pop();
		for (int dir = 0; dir < 4; dir++) {
			int nx = cur.X + dx[dir];
			int ny = cur.Y + dy[dir];
			if (OOB(nx,ny) || dist[nx][ny] >= 0) continue;
			if (board[nx][ny] > rLevel) continue;
			q.push({ nx,ny });
			dist[nx][ny] = dist[cur.X][cur.Y] + 1;
			if (board[nx][ny] && (board[nx][ny] < rLevel))
				possibleMonster.push_back({ dist[nx][ny], nx,ny });
		}
	}
}


int Eat(vector<tuple<int, int, int>>& possibleMonster) {
	int md, mx, my, time;
	sort(possibleMonster.begin(), possibleMonster.end()); // 거리가까운순, 좌표 좌상순 나열
	auto monster = possibleMonster.front(); 
	
	tie(md, mx, my) = monster; 

	board[rx][ry] = 0;
	board[mx][my] = 9; 

	rx = mx; ry = my; 
	time = md;

	eatNum++;
	if (eatNum >= rLevel) { rLevel++; eatNum = 0; }
	return time; 
}
int main(void){

	cin >> n;

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cin >> board[i][j];
			if (board[i][j] == 9) {
				rx = i;
				ry = j;
			}
		}
	}

	int ans = 0;

	while (1) {
		
		vector<tuple<int, int, int>> possibleMonster; 

		Bfs(possibleMonster); // 1. 거리측정, 먹을 몬스터 존재여부 확인 
		
		if (possibleMonster.empty()) break; // 전투로봇이 먹을 몬스터 없음 
		
		ans += Eat(possibleMonster); // 2. 몬스터 먹기
	}

	cout << ans;
}