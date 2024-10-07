#include <iostream>
#include <tuple>
#include <algorithm>
#include <utility>
#include <queue>
#define X first
#define Y second
using namespace std; 
const int dx[4] = { -1,0, 1, 0 }; // 상 우 하 좌
const int dy[4] = { 0, 1, 0, -1 };
struct Knight {
	int r, c, h, w, k, dmgSum;
};

int l, n, q;
int board[42][42]; // 함정, 벽
int kboard[42][42]; // 기사

Knight knight[32];


bool OOB(int x, int y) {
	return x < 0 || x >= l || y < 0 || y >= l;
}
void MoveAndDamage(int idx, int d) {


	if (knight[idx].k == 0) return ;
	queue<int> q;
	int vis[32] = {};
	bool pushPossible = true;

	q.push(idx);
	vis[idx] = 2;
	
	// 1. 움직임 가능여부와 움직이는 기사 체크
	while (!q.empty()) {
		auto kn = knight[q.front()]; q.pop();
		
		if (d == 0 || d == 2) { // 상, 하
			int nx = ( d == 2 ? kn.r + kn.h : kn.r - 1 );
			int ny;
			for (int j = 0; j < kn.w; j++) {
				ny = kn.c + j;
				if (OOB(nx, ny) || board[nx][ny] == 2) {
					pushPossible = false;
					break;
				}
				if (kboard[nx][ny] && !vis[kboard[nx][ny]]) {
					q.push(kboard[nx][ny]);
					vis[kboard[nx][ny]] = 1;
				}
			}
		}
		else if (d == 1 || d == 3) { // 우, 좌
			int nx;
			int ny = ( d == 1 ? kn.c + kn.w : kn.c - 1 );
			for (int i = 0; i < kn.h; i++) {
				nx = kn.r + i;
				if (OOB(nx, ny) || board[nx][ny] == 2) {
					pushPossible = false;
					break;
				}
				if (kboard[nx][ny] && !vis[kboard[nx][ny]]) {
					q.push(kboard[nx][ny]);
					vis[kboard[nx][ny]] = 1;
				}
			}
		}
	}
	
	if (!pushPossible) return ; // 움직일 수 없음

	// 2. 움직이면서 데미지 기록

	for (int idx = 1; idx <= n; idx++) {
		if (!vis[idx]) continue;
		auto k = knight[idx];
		for (int i = k.r; i < k.r + k.h; i++)
			for (int j = k.c; j < k.c + k.w; j++)
				kboard[i][j] = 0;
	}

	
	for (int idx = 1; idx <= n; idx++) {
		if (!vis[idx]) continue;
		knight[idx].r += dx[d];
		knight[idx].c += dy[d];

		auto k = knight[idx];
		int dmg = 0;
		for (int i = k.r; i < k.r + k.h; i++) {
			for (int j = k.c; j < k.c + k.w; j++) {
				kboard[i][j] = idx;
				if (board[i][j] == 1)
					dmg++;
			}
		}

		if (vis[idx] == 2) continue;
		
		knight[idx].k -= dmg;
		knight[idx].dmgSum += dmg;
		
		if (knight[idx].k < 0) {
			knight[idx].k = 0;

			for (int i = k.r; i < k.r + k.h; i++)
				for (int j = k.c; j < k.c + k.w; j++)
					kboard[i][j] = 0;
		}

	}

	return ;
}

int getLivedDamage() {
	int ret = 0;
	for (int idx = 1; idx <= n; idx++) {
		if (knight[idx].k == 0) continue;
		ret += knight[idx].dmgSum;
	}
	return ret;
}

int main() {
	ios::sync_with_stdio(0), cin.tie(0);

	cin >> l >> n >> q;

	for (int i = 0; i < l; i++) {
		for (int j = 0; j < l; j++) {
			cin >> board[i][j];
		}
	}

	for (int idx = 1; idx <= n; idx++) {
		int r, c, h, w, k;
		cin >> r >> c >> h >> w >> k;
		r--; c--;
		knight[idx] = { r , c , h, w,k, 0 };

		for (int i = r; i < r + h; i++)
			for (int j = c; j < c + w; j++)
				kboard[i][j] = idx;
	}

	int ans = 0;
	while(q--){
		int idx, d;
		cin >> idx >> d;
		MoveAndDamage(idx, d);
	}

	ans = getLivedDamage();
	cout << ans;
}