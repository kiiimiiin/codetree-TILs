#include <iostream>
#include <vector>
#include <utility>
#include <tuple>
#include <algorithm>
#include <queue>
#include <stack>
#define X first 
#define Y second
using namespace std;
pair<int, int> board[12][12]; // 공격력, 최근 공격한 turn
int dx[4] = { 0, 1, 0, -1 };
int dy[4] = { 1, 0, -1, 0 }; // 우하좌상
int n, m, k;
int t; // turn
int oneFlag;



pair<int,int> ChooseAttacker() { // 가장 약한 포탑 선정
	vector<tuple<int,int,int>> attackers; // 공격력, 좌표
	int ax, ay;
	
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (board[i][j].first > 0) {
				attackers.push_back({ board[i][j].first, i , j });
			}
		}
	}
	if (attackers.size() == 0) exit(0); 

	sort(attackers.begin(), attackers.end());
	
	
	if (attackers.size() >= 2) {

		vector<tuple<int, int, int>> infos; // 공격 turn, 행열합, 열
		int idx = -1;
		for (int i = 0; i < attackers.size() - 1; i++) {
			int stat, nStat;
			tie(stat, ignore, ignore) = attackers[i];
			tie(nStat, ignore, ignore) = attackers[i + 1];
			if (stat < nStat) { idx = i; break; }
		}

		for (int i = 0; i <= idx; i++) { // 최저 공격력 포탑들에 대해서
			int x, y;
			tie(ignore, x, y) = attackers[i];
			infos.push_back({ board[x][y].second, x + y, y });
		}

		sort(infos.begin(), infos.end());
		
		auto wAttacker = infos.back();
		ax = get<1>(wAttacker) - get<2>(wAttacker);
		ay = get<2>(wAttacker);
	}
	else if (attackers.size() == 0) {
		auto wAttacker = attackers.front();
		ax = get<1>(wAttacker);
		ay = get<2>(wAttacker); 
	}

	board[ax][ay].first += n + m;
	board[ax][ay].second = t;
	return { ax, ay };
	
}

pair<int, int> ChooseTarget(int ax, int ay) { // 가장 강한 포탑 선정
	vector<tuple<int, int, int>> attackers; // 공격력, 좌표
	int tx, ty;

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (i == ax && j == ay) continue; 
			if (board[i][j].first > 0) {
				attackers.push_back({ board[i][j].first, i , j });
			}
		}
	}
	if (attackers.size() == 0) exit(0);

	sort(attackers.begin(), attackers.end());

	if (attackers.size() >= 2) {

		vector<tuple<int, int, int>> infos; // 공격 turn, 행열합, 열
		int idx = -1;
		for (int i = attackers.size() - 1; i >= 1; i--) {
			int stat, bStat;
			tie(stat, ignore, ignore) = attackers[i];
			tie(bStat, ignore, ignore) = attackers[i - 1];
			if (stat > bStat ) { idx = i; break; }
		}

		for (int i = attackers.size() - 1; i >= idx; i--) { // 최저 공격력 포탑들에 대해서
			int x, y;
			tie(ignore, x, y) = attackers[i];
			infos.push_back({ board[x][y].second, x + y, y });
		}

		sort(infos.begin(), infos.end());

		auto sAttacker = infos.front();
		tx = get<1>(sAttacker) - get<2>(sAttacker);
		ty = get<2>(sAttacker);
	}
	else if (attackers.size() == 0) {
		auto sAttacker = attackers.front();
		tx = get<1>(sAttacker);
		ty = get<2>(sAttacker);
	}

	return { tx, ty };
}

pair<int, int> ProcessOOB(int nx, int ny) {
	if (nx < 0) nx = n + nx;
	if (nx >= n) nx = nx - n;
	if (ny < 0) ny = m + ny;
	if (ny >= m) ny = ny - m;

	return { nx, ny }; 
}

bool Laser(int ax, int ay, int tx, int ty) {
	queue<pair<int, int>> q;
	int dist[12][12] = {};
	for (int i = 0; i < n; i++) fill(dist[i], dist[i] + m, -1);

	q.push({ tx,ty });
	dist[tx][ty] = 0;
	while (!q.empty()) {
		auto cur = q.front(); q.pop();
		for (int dir = 0; dir < 4; dir++) {
			int nx = cur.X + dx[dir];
			int ny = cur.Y + dy[dir]; 
			tie(nx, ny) = ProcessOOB(nx, ny);
			if (board[nx][ny].first == 0 || dist[nx][ny] >= 0) 
				continue;
			dist[nx][ny] = dist[cur.X][cur.Y] + 1;
			q.push({ nx,ny });
		}
	} // 타겟부터 공격자까지 거리값 계산

	if (dist[ax][ay] == -1) return false; // 포탑공격
	stack<pair<int, int>> s;
	int route[12][12] = {};
	s.push({ ax, ay });
	while(route[tx][ty] != 1){
		auto cur = s.top(); s.pop();
		route[cur.X][cur.Y] = 1;
		for (int dir = 3; dir >= 0; dir--) { // 상좌하우
			int nx = cur.X + dx[dir];
			int ny = cur.Y + dy[dir];
			tie(nx, ny) = ProcessOOB(nx, ny);
			if (dist[nx][ny] == -1 || route[nx][ny] == 1) 
				continue; 
			if (dist[nx][ny] == dist[cur.X][cur.Y] - 1) {
				s.push({ nx,ny });
			}
		}
	} // 공격자부터 타겟까지 최단루트 탐색

	
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			if (i == ax && j == ay) continue;
			if (route[i][j] == 1) {
				if (i == tx && j == ty)
					board[i][j].first -= board[ax][ay].first;
				else
					board[i][j].first -= board[ax][ay].first / 2;
	
				if (board[i][j].first < 0) 
					board[i][j].first = 0;
			}
		}
	}
}

void Bomb(int ax, int ay, int tx, int ty) {
	int bx[9] = { 0,-1,-1,-1,0, 0,1,1,1 };
	int by[9] = { 0,-1,0,1,-1,1,-1,0,1 };
	
	for (int dir = 0; dir < 9; dir++) {
		int x = tx + bx[dir];
		int y = ty + by[dir];
		tie(x, y) = ProcessOOB(x, y);
		if (x == ax && y == ay) continue;
		if (board[x][y].first == 0) continue;
		if(dir == 0)
			board[x][y].first -= board[ax][ay].first;
		else
			board[x][y].first -= board[ax][ay].first / 2;
		
		if (board[x][y].first < 0)
			board[x][y].first = 0;
	}
	
}

void Copy(pair<int,int> desc[12][12], pair<int,int> src[12][12]) {
	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			desc[i][j] = src[i][j];
}

void Attack(int ax, int ay) {
	int tx, ty;
	tie(tx, ty) = ChooseTarget(ax,ay);

	pair<int,int> cBoard[12][12];
	Copy(cBoard, board);

	// 포탑공격 
	if (!Laser(ax, ay, tx, ty))
		Bomb(ax, ay, tx, ty);

	// 포탑정비
	int cnt = 0;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			if (board[i][j].first == 0) continue;
			if (i == ax && j == ay) continue;
			cnt++;
			if (board[i][j].first == cBoard[i][j].first
				&& board[i][j].second == cBoard[i][j].second)
				board[i][j].first++;
		}
	}

	if (cnt == 1) oneFlag = 1;
}

int main(void) {
	cin >> n >> m >> k;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			int stat; 
			cin >> stat;
			board[i][j].first = stat;
			board[i][j].second = t; // 모든 포탑의 최근 공격turn은 0
		}
	}

	for (t = 1; t <= k; t++) {
		int ax, ay;

		tie(ax, ay) = ChooseAttacker();
		
		Attack(ax, ay);
		if (oneFlag) break; 
	}

	// 가장 강한 포탑 공격력 출력
	int tx, ty;
	tie(tx, ty) = ChooseTarget(-1, -1);
	cout << board[tx][ty].first;
}