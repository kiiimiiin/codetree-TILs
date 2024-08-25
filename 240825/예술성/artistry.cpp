#include <iostream>
#include <utility>
#include <queue>
#define X first
#define Y second
const int dx[4] = { -1, 0, 1 ,0 };
const int dy[4] = { 0, 1, 0, -1 };
using namespace std;
int n;
int board[31][31];
int temp[31][31];
int center;

struct GroupInfo {
	int num, area;
	int side[900]; // 인접그룹 변 개수
};

GroupInfo groups[900];

bool OOB(int x, int y) {
	return x < 0 || x >= n || y < 0 || y >= n;
}

int getArtScore() {
	int id = 1;
	int group[31][31] = {};
	queue<pair<int, int>> q;

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (!group[i][j]) {
				int area = 0;
				int num = board[i][j];
				q.push({ i, j });
				group[i][j] = id;
				while (!q.empty()) {
					auto cur = q.front(); q.pop(); area++;
					for (int dir = 0; dir < 4; dir++) {
						int nx = cur.X + dx[dir];
						int ny = cur.Y + dy[dir];
						if (OOB(nx, ny)) continue;
						if (group[nx][ny]) {
							if (group[nx][ny] == id) continue;
							groups[id].side[group[nx][ny]]++;
							continue;
						}
						if (num == board[nx][ny]) {
							q.push({ nx,ny });
							group[nx][ny] = id;
						}
					}
				}
				groups[id].num = num;
				groups[id].area = area;
				id++;
			}
		}
	}

	int score = 0; 
	for (int i = 1; i <= id; i++) {
		for (int j = 1; j < i; j++) {
			if (groups[i].side[j]) {
				score += (groups[i].area + groups[j].area)
					* groups[i].num * groups[j].num * groups[i].side[j];
				groups[i].side[j] = 0;
			}
		}
	}


	return score;
}

void RotateClock(int x, int y) {
	
	for (int i = 0; i < center; i++) {
		for (int j = 0; j < center; j++) {
			temp[j][center - 1 - i] = board[x + i][y + j];
		}
	}

	for (int i = 0; i < center; i++) {
		for (int j = 0; j < center; j++) {
			board[x + i][y + j] = temp[i][j];
		}
	}
}

void Rotate() {
	// 1. 십자가 rotate

	for (int s = 1; s <= center; s++) {
		for (int dir = 0; dir < 3; dir++) {
			int x = center + s * dx[dir];
			int y = center + s * dy[dir];
			int nx = center + s * dx[dir + 1];
			int ny = center + s * dy[dir + 1];
			swap(board[x][y], board[nx][ny]);
		}
	}

	// 2. 시계 rotate 
	RotateClock(0, 0);
	RotateClock(0, center+1);
	RotateClock(center+1, 0);
	RotateClock(center+1, center+1);

}



int getScore() {
	return 0;
}

int main() {
	cin >> n;
	center = (n - 1) / 2;

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cin >> board[i][j];
		}
	}
	

	int ans = getArtScore();
	
	for (int i = 0; i < 3; i++) {
		Rotate();
		ans += getArtScore();
	}
	cout << ans;
}



/*
	bfs를 하며 그룹 구분, 이루는 숫자값 & 칸수
	또 다시 bfs를 하며 인접한 그룹간 조화로움 합 구함
	( right down 순회하면 중복이 없이 낮은 그룹을 기준으로 구해질것임 )

*/