#include <iostream>
using namespace std;
int n, m, d, p;
int board[27][27];
int idx[27][27];
int arr[630];
const int dx[4] = { 0, 1, 0, -1 }; // 우하좌상
const int dy[4] = { 1, 0, -1, 0 };

bool OOB(int x, int y) {
	return x < 0 || x >= n || y < 0 || y >= n; 
}

void Set() {
	
	int num = n * n - 1;
	int x = 0, y = 0, dir = 0;
	idx[x][y] = num;
	

	while (!(x == n / 2 && y == n / 2)) {
		int nx = x + dx[dir];
		int ny = y + dy[dir];
		if (OOB(nx, ny) || idx[nx][ny]) {
			dir = (dir + 1) % 4; 
			continue;
		}
		idx[nx][ny] = idx[x][y] - 1; 
		x = nx; y = ny; 
	}

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			arr[idx[i][j]] = board[i][j];
		}
	}

}

int Attack(int d, int p) {
	int ret = 0;
	int x = n / 2;
	int y = n / 2;
	
	for (int s = 1; s <= p; s++) {
		int nx = x + s * dx[d];
		int ny = y + s * dy[d];
		if (OOB(nx, ny) || arr[idx[nx][ny]] == 0) 
			break;
		ret += arr[idx[nx][ny]];
		arr[idx[nx][ny]] = -1;
	}

	return ret;
}

void Push() {

	int i, k;
	int tmp[630] = {};
	for ( i = 1, k = 1; arr[i] != 0; i++) {
		if (arr[i] == -1) continue;
		tmp[k] = arr[i];
		k++;
	}

	for (int j = 1; j <= i - 1; j++) {
		arr[j] = tmp[j];
	}
}

int Remove() {
	int ret = 0;
	int sum = -1;
	int end;
	int cnt = 0;

	while (sum != 0) {
		sum = 0;
		for (int i = 1; arr[i] != 0; i++) {
			if (arr[i] == arr[i + 1]) {
				end = i + 1;
				cnt++;
				continue;
			}

			if (cnt >= 3) {
				for (int j = end - cnt; j <= end; j++) {
					sum += arr[j];
					arr[j] = -1;
				}
			}
			cnt = 0;
		}
		ret += sum;
		Push();
	}
	
	return ret;
}

void Copy() {
	int num = n * n - 1;
	int tmp[630] = {};
	int end;
	int cnt = 1;
	int i, k;

	for (i = 1, k = 1; arr[i] != 0; i++) {
		if (arr[i] == arr[i + 1]) {
			cnt++;
			continue;
		}

		tmp[k++] = cnt;
		tmp[k++] = arr[i];
		cnt = 1;
		
	}

	for (int i = 1; i <= num; i++) {
		arr[i] = tmp[i];
	}
}

int main(void) {
	cin >> n >> m;

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cin >> board[i][j];
		}
	}

	int ans = 0;

	Set();

	while (m--) {
		cin >> d >> p; 
		ans += Attack(d, p);
		Push();
		ans += Remove();
		Copy(); 
	}

	cout << ans;
}


/*
	
	배열을 일자로 핀다
	각 배열에 따른 인덱스들을 나선형 보드와 맵핑

	1. 공격하기
	2. 앞으로 밀기 
	3. 4중복 지우기
	4. 복제
*/