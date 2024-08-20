#include <iostream>
#include <vector>
#include <tuple>
#define X first
#define Y second
using namespace std;


struct Pack {
	int r, c;
};

const int dx[8] = { -1,-1,0,1,1,1,0,-1 };
const int dy[8] = { 0, -1,-1,-1,0, 1,1,1 };

Pack pack;
vector<int> board[5][5]; // 몬스터 보드 ( 방향 )
vector<int> copied[5][5];
int demon[5][5];
int m, t;


bool OOB(int x, int y) {
	return x < 0 || x >= 4 || y < 0 || y >= 4; 
}

bool ExistPack(int x, int y) {
	return x == pack.r && y == pack.c; 
}

void CopyMonster() {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			copied[i][j] = board[i][j];
		}
	}
}


void MoveMonster() {
	vector<int> temp[5][5];

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (board[i][j].empty()) continue;
			for (auto md : board[i][j]) {
				int dd;
				for (dd = 0; dd < 8; dd++) {
					int nd = (md + dd) % 8;
					int nx = i + dx[nd];
					int ny = j + dy[nd];
					if (OOB(nx, ny) || demon[nx][ny] || ExistPack(nx, ny))
						continue;
					temp[nx][ny].push_back(nd);
					break;
				}
				if (dd == 8)
					temp[i][j].push_back(md);
			}
		}
	}

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			board[i][j] = temp[i][j];
		}
	}
}



void MovePack() {
	/*
		상 좌 하 우
		0 1 2 3
	*/
	tuple<int, int, int, int> mx = { -1,0,0,0};
	int mxDir[3];
	
	// 1. 최대 섭취 방향 구함
	for (int cse = 0; cse < (1 << 6); cse++) {
		int brute = cse;
		int dir[3];
		bool passed[5][5] = {};
		
		// 방향정보와 우선순위 확인
		for (int i = 0; i < 3; i++) {
			dir[i] = brute % 4;
			brute /= 4;
		}
		
		
		// 해당 방향으로 
		int curX = pack.r;
		int curY = pack.c; 
		int eatten = 0;
		bool isSkipped = false;
		for (int i = 0; i < 3; i++) {
			int nx = curX + dx[dir[i] * 2];
			int ny = curY + dy[dir[i] * 2];
			if (OOB(nx, ny)) {
				isSkipped = true;
				break;
			}
			
			if(!passed[nx][ny])
				eatten += board[nx][ny].size();
			passed[nx][ny] = true;

			curX = nx;
			curY = ny;
		}
		if (isSkipped) continue;
		if (make_tuple(eatten, -dir[0], -dir[1], -dir[2]) > mx){
			mx = make_tuple(eatten, -dir[0], -dir[1], -dir[2]);
		}
	}


	tie(ignore, mxDir[0], mxDir[1], mxDir[2]) = mx;
	for (int i = 0; i < 3; i++) mxDir[i] = -mxDir[i];

	// 2. 최대섭취에 따라 실제 섭취

	for (int i = 0; i < 3; i++) {
		int nx = pack.r + dx[mxDir[i] * 2];
		int ny = pack.c + dy[mxDir[i] * 2];
		
		if (!board[nx][ny].empty()) {
			board[nx][ny].clear();
			demon[nx][ny] = 3;
		}
			
		pack.r = nx;
		pack.c = ny;
	}
}

void RemoveDemon() {
	for (int i = 0; i < 4; i++) 
		for (int j = 0; j < 4; j++)
			if (demon[i][j]) 
				demon[i][j]--;
}

void WakeUp() {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (copied[i][j].empty()) continue;
			while (!copied[i][j].empty()) {
				board[i][j].push_back(copied[i][j].back());
				copied[i][j].pop_back();
			}
		}
	}
}

int getAns() {
	int ret = 0;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			ret += board[i][j].size();
		}
	}
	return ret;
}

int main() {
	ios::sync_with_stdio(0), cin.tie(0);

	cin >> m >> t; 
	
	cin >> pack.r >> pack.c;
	pack.r--; pack.c--;

	for (int i = 0; i < m; i++) {
		int r, c, d;
		cin >> r >> c >> d;
		r--; c--; d--;
		board[r][c].push_back(d);
	}

	int ans = 0;

	while (t--) {
		CopyMonster();
		MoveMonster();
		MovePack();
		RemoveDemon();
		WakeUp();
	}
	
	ans += getAns();
	cout << ans;
}


/*
	
	인덱스 보드와 구조체 벡터
	1. Copy
	2. MoveMonster
	3. MovePack
	4. 시체소멸
	5. 1의 복제 몬스터 깨어남
*/