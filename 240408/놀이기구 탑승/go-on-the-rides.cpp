#include <iostream>
#include <tuple>
#include <vector>
#include <algorithm>
using namespace std;

int stuNums[402][4]; 
int board[22][22];
int n;
int dx[4] = { 1, 0, -1 ,0 };
int dy[4] = { 0, -1, 0 , 1 };

bool cmp(tuple<int, int, int, int>& t1, tuple<int, int, int, int>& t2) {
	if (get<0>(t1) != get<0>(t2)) return get<0>(t1) > get<0>(t2); // 인접선호친구 많은순
	if (get<1>(t1) != get<1>(t2)) return get<1>(t1) > get<1>(t2); // 인접 빈칸 많은순
	if (get<2>(t1) != get<2>(t2)) return get<2>(t1) < get<2>(t2); // 행이 작은순
	return get<3>(t1) < get<3>(t2); // 열이 작은순 정렬
}

bool OOB(int x, int y) {
	return (x < 0 || x >= n || y < 0 || y >= n);
}

bool IsPreferred(int nx, int ny, int stuNum) {
	
	for (auto pre : stuNums[stuNum]) {
		if (pre == board[nx][ny]) return true;
	}
	return false;
}

void Mark(int stuNum) {
	vector<tuple<int, int, int, int>> infos; 
	// 인접 좋아하는 친구수 , 빈칸수, 좌표
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (board[i][j]) continue;
			int emptyNum = 0;
			int preferredNum = 0;
			for (int dir = 0; dir < 4; dir++) {
				int nx = i + dx[dir];
				int ny = j + dy[dir];
				if (OOB(nx, ny)) continue;
				if (board[nx][ny] == 0) emptyNum++;
				else if (IsPreferred(nx, ny, stuNum)) preferredNum++;
			}
			infos.push_back({ preferredNum, emptyNum, i, j });
		}
	}

	sort(infos.begin(), infos.end(), cmp);
	int tx, ty;
	tie(ignore, ignore, tx, ty) = infos.front();
	board[tx][ty] = stuNum;
}

int Ans() {
	int ret = 0;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			int preferredNum = 0;
			for (int dir = 0; dir < 4; dir++) {
				int nx = i + dx[dir];
				int ny = j + dy[dir];
				if (OOB(nx, ny)) continue;
				if (IsPreferred(nx, ny, board[i][j]))
					preferredNum++;
			}
			
			if (preferredNum == 1) ret += 1;
			else if (preferredNum == 2) ret += 10;
			else if (preferredNum == 3) ret += 100;
			else if (preferredNum == 4) ret += 1000;
		}
	}

	return ret;
}
int main(void) {
	cin >> n;
	
	for (int i = 1; i <= n * n; i++) {
		int stuNum;
		cin >> stuNum;
		for (int p = 0; p < 4; p++) {
			int preferdNum;
			cin >> preferdNum;
			stuNums[stuNum][p] = preferdNum;
		}
		
		Mark(stuNum);
	}

	int ans = Ans(); 
	cout << ans;
}