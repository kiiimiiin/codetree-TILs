#include <iostream>
#include <vector>
#include <queue>
#include <tuple>
#include <utility>
#define X first
#define Y second
using namespace std;
int dx[4] = { 1, 0, -1, 0 }; // 하 우 상 좌
int dy[4] = { 0, 1, 0, -1 };
int n;
int board[32][32];
struct Group {
	int idx;
	int area;
	int num;
	
};

bool OOB(int x, int y) {
	return (x < 0 || x >= n || y < 0 || y >= n); 
}
int GetArtScore() {
	//1. 그룹 정보 get
	vector<Group> groups;
	int idxBoard[32][32] = {};
	for (int i = 0; i < n; i++) fill(idxBoard[i], idxBoard[i] + n, -1);

	int groupIdx = 0;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (board[i][j] && idxBoard[i][j] == -1) { // bfs
				int num = board[i][j]; 
				int area = 0;
				queue<pair<int, int>> q;
				idxBoard[i][j] = groupIdx;
				q.push({ i,j });
				while (!q.empty()) {
					auto cur = q.front(); q.pop(); area++;
					for (int dir = 0; dir < 4; dir++) {
						int nx = cur.X + dx[dir];
						int ny = cur.Y + dy[dir];
						if (OOB(nx, ny)||idxBoard[nx][ny] >= 0) continue;
						if (board[nx][ny] != num) continue;
						q.push({ nx,ny });
						idxBoard[nx][ny] = groupIdx;
					}
				}
				groups.push_back({ groupIdx, area, num });
				groupIdx++;
			}
		}
	}

	// 그룹간 최인접한 그룹의 변개수 구하기 
	int vis[32][32] = {};

	vector<tuple<int, int, int>> nearNums; // 인접그룹쌍과 변개수
	

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (idxBoard[i][j] >= 0 && !vis[i][j]) { // bfs
				int nearNum = 0;
				vector<int> nearNumByGroup(groupIdx, 0);
				queue<pair<int, int>> q;
				vis[i][j] = 1; 
				q.push({ i,j });
				while (!q.empty()) {
					auto cur = q.front(); q.pop();
					for (int dir = 0; dir < 4; dir++) {
						int nx = cur.X + dx[dir];
						int ny = cur.Y + dy[dir];
						if (OOB(nx, ny) || vis[nx][ny]) continue;
						if (idxBoard[nx][ny] != idxBoard[i][j]) {
							nearNumByGroup[idxBoard[nx][ny]]++;
							continue;
						}
						q.push({ nx,ny });
						vis[nx][ny] = 1;
					}
				}
				
				for (int k = 0; k < nearNumByGroup.size(); k++) {
					if (nearNumByGroup[k])
						nearNums.push_back({ idxBoard[i][j], k, nearNumByGroup[k] });
				}
			}
		}
	}

	int ret = 0; 
	for (auto cse : nearNums) {
		int idx1, idx2, nearNum;
		tie(idx1, idx2, nearNum) = cse; 
		ret += (groups[idx1].area + groups[idx2].area) * 
			groups[idx1].num * groups[idx2].num * nearNum;
	}//!!!

	return ret;
}




void Rotate() {
	// 1. SubCrossRotate
	int mx, my;
	mx = my = (n - 1) / 2;

	int cdx[4] = { 0, 1, 0, -1 };
	int cdy[4] = { 1, 0, -1, 0 };

	for (int s = 1; s <= (n - 1) / 2; s++) {
		for (int dir = 0; dir < 3; dir++) {
			int nx = mx + s * cdx[dir];
			int ny = my + s * cdy[dir];
			int nnx = mx + s * cdx[dir + 1];
			int nny = my + s * cdy[dir + 1];
			swap(board[nx][ny], board[nnx][nny]);
		}
	}
	//2 .SubRecRotate

	int cBoard[32][32] = {};
	for (int i = 0; i < mx; i++)  // (0,0) 사각형 rotate
		for (int j = 0; j < my; j++)
			cBoard[j][mx -1 - i] = board[i][j];

	for (int i = 0; i < mx; i++)
		for (int j = 0; j < my; j++)
			board[i][j] = cBoard[i][j];

	for (int i = mx + 1; i < n; i++) // (mx + 1, 0) 사각형 rotate
		for (int j = 0; j < my; j++)
			cBoard[j][mx - 1 - (i -(mx + 1))] = board[i][j];

	for (int i = 0; i < mx; i++) 
		for (int j = 0; j < my; j++)
			board[i + (mx + 1)][j] = cBoard[i][j];

	for (int i = 0; i < mx; i++) // (0,my + 1) 사각형 rotate
		for (int j = mx + 1; j < n; j++)
			cBoard[j - (mx + 1)][mx -1 -i] = board[i][j];

	for (int i = 0; i < mx; i++) 
		for (int j = 0; j < my; j++)
			board[i][j +(mx+1)] = cBoard[i][j];
	 
	for (int i = mx + 1; i < n; i++) // (mx + 1,my + 1) 사각형 rotate
		for (int j = mx + 1; j < n; j++)
			cBoard[j - (mx + 1)][mx - 1 - (i - (mx + 1))] = board[i][j];

	for (int i = 0; i < mx; i++)  
		for (int j = 0; j < my; j++)
			board[i + (mx + 1)][j + (mx + 1)] = cBoard[i][j];
}

int main(void) {
	cin >> n;

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cin >> board[i][j];
		}
	}

	int cnt = 3;
	int ans = 0;
	ans += GetArtScore(); // 초기예술점수
	
	while (cnt--) {
		Rotate();
		ans += GetArtScore();
	}

	cout << ans;
}