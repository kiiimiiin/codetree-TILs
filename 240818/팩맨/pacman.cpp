#include <iostream>
#include <vector>
#define X first
#define Y second
using namespace std;
struct Monster {
	int r, c, d;
	int idx;
	bool isLived = true;
};

struct Pack {
	int r, c;
};

const int dx[8] = { -1,-1,0,1,1,1,0,-1 };
const int dy[8] = { 0, -1,-1,-1,0, 1,1,1 };
Pack pack;
vector<Monster> monster;
vector<Monster> copied;
vector<int> board[5][5]; // 인덱스 보드
int demon[5][5]; // 시체 보드
int m, t; 
int mx;
int mxDir[3], packDir[3];
int eattenAlready[5][5]; 
vector<pair<int, int>> eattenPos;


bool OOB(int x, int y) {
	return x < 0 || x >= 4 || y < 0 || y >= 4; 
}

bool ExistPack(int x, int y) {
	return x == pack.r && y == pack.c; 
}

void CopyMonster() {
	for (int i = 0; i < monster.size(); i++) {
		if (!monster[i].isLived) continue;
		copied.push_back(monster[i]);
	}
}


void MoveMonster() {
	vector<int> temp[5][5];
	int dd; 
	for (int i = 0; i < monster.size(); i++) {
		if (!monster[i].isLived) continue;
		
		for (dd = 0; dd < 8; dd++) {
			int nd = ( monster[i].d + dd ) % 8;
			int nx = monster[i].r + dx[nd];
			int ny = monster[i].c + dy[nd];
			if (OOB(nx, ny) || demon[nx][ny] || ExistPack(nx, ny)) 
				continue;
			temp[nx][ny].push_back(i);
			monster[i].r = nx;
			monster[i].c = ny; 
			monster[i].d = nd;
			break;
		}

		if (dd == 8)
			temp[monster[i].r][monster[i].c].push_back(i);
	}

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			board[i][j] = temp[i][j];
		}
	}
}

bool EattenAlready(int x, int y) {
	for (auto pos : eattenPos) {
		if (x == pos.X && y == pos.Y) return true;
	}

	return false;
}

void dfs(int k, int pr, int pc, int eatten) {
	if (k >= 3) {
		// 최대 초기화
		if (eatten > mx) {
			mx = eatten;
			for (int i = 0; i < 3; i++) 
				mxDir[i] = packDir[i];
		}
		return;
	}

	for (int dir : {0, 2, 4, 6}) {
		int nx = pr + dx[dir];
		int ny = pc + dy[dir];
		if (OOB(nx, ny) || eattenAlready[nx][ny]) continue;
		packDir[k] = dir;
		if (!board[nx][ny].empty()) {
			eattenAlready[nx][ny] = 1;
		}
		dfs(k + 1, nx, ny, eatten + board[nx][ny].size());
		
		eattenAlready[nx][ny] = 0;
	}
}

void MovePack() {
	mx = -0x7f7f7f7f;
	fill(mxDir, mxDir + 3, -1);
	dfs(0, pack.r, pack.c, 0);

	//if (mx == -0x7f7f7f7f) { exit(0); }
	for (int i = 0; i < 3; i++) {
		int nx = pack.r + dx[mxDir[i]];
		int ny = pack.c + dy[mxDir[i]];
		
		if (!board[nx][ny].empty())
			demon[nx][ny] = 3;
		
		for (auto idx : board[nx][ny])
			monster[idx].isLived = false;

		board[nx][ny].clear();
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
	while (!copied.empty()) {
		Monster cm = copied.back();
		monster.push_back(cm);
		board[cm.r][cm.c].push_back(monster.size() - 1);
		copied.pop_back();
	}
}

int getAns() {
	int ret = 0;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (board[i][j].empty()) continue;
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
		Monster ms; 
		cin >> ms.r >> ms.c >> ms.d;
		ms.r--; ms.c--; ms.d--;
		monster.push_back(ms);
		board[ms.r][ms.c].push_back(i);
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