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


void SubRecRotate(int x, int y, int cBoard[32][32]) { // 기준좌표 x, y
	int m = (n - 1) / 2;

	for (int i = 0; i < m; i++) {
		for (int j = 0; j < m; j++) {
			cBoard[j][m - 1 - i] = board[i + x][j + y];
		}
	}
	// rotate할 기준좌표의 board영역을 (0,0)기준으로 rotate한채 backUp

	for (int i = 0; i < m; i++) {
		for (int j = 0; j < m; j++) {
			board[i + x][j + y] = cBoard[i][j];
		}
	}
	// (0,0)기준으로 저장된 rotatedBoard를 기준좌표에 맞춰 기존 board에 복사 
}

void Rotate() {
	// 1. SubCrossRotate
	int m = (n - 1) / 2;

	int cdx[4] = { 0, 1, 0, -1 };
	int cdy[4] = { 1, 0, -1, 0 };

	for (int s = 1; s <= (n - 1) / 2; s++) {
		for (int dir = 0; dir < 3; dir++) {
			int nx = m + s * cdx[dir];
			int ny = m + s * cdy[dir];
			int nnx = m + s * cdx[dir + 1];
			int nny = m + s * cdy[dir + 1];
			swap(board[nx][ny], board[nnx][nny]);
		}
	}
	//2 .SubRecRotate

	int cBoard[32][32] = {};

	SubRecRotate(0, 0, cBoard);
	SubRecRotate(m + 1, 0, cBoard);
	SubRecRotate(0, m + 1, cBoard);
	SubRecRotate(m + 1, m + 1, cBoard);
	
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