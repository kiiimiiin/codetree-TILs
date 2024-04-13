#include <iostream>
#include <vector>
#include <utility>
#include <tuple>
#include <algorithm>
#include <queue>
#define X first
#define Y second
using namespace std;
int n, m, p, c, d; // p : 산타수, c : 루돌프 힘 , d: 산타힘
int rx, ry;
vector<tuple<int, int, int>> santas(33); // 산타 좌표, 기절여부
int score[32]; // 산타점수
int board[52][52]; // 산타위치 board ( 9 : 정상 산타번호  ) 
int rdx[8] = { 1, 1, 1, 0, -1,-1,-1, 0 };
int rdy[8] = { -1, 0, 1, 1, 1, 0, -1, -1 };
int sdx[4] = { -1, 0, 1, 0 }; // 상우하좌
int sdy[4] = { 0, 1, 0, -1 };

bool Cmp(tuple<int, int, int, int>& t1, tuple<int, int, int, int>&t2) {
	if (get<0>(t1) != get<0>(t2)) return get<0>(t1) < get<0>(t2);
	if (get<1>(t1) != get<1>(t2)) return get<1>(t1) > get<1>(t2);
	return get<2>(t1) > get<2>(t2);
}

bool OOB(int x, int y) {
	return (x < 0 || x >= n || y < 0 || y >= n);
}

int getDist(int x1, int y1, int x2, int y2) {
	return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
}

void Push(int sx, int sy, int i, int sdir, int flag) {
	
	// 밀려진 산타 기절 처리
	sdir = flag ? ( sdir + 2 ) % 4 : sdir; // 루돌프는 그냥 밀림

	sx = flag ? sx + d * sdx[sdir] : sx + c * rdx[sdir];
	sy = flag ? sy + d * sdy[sdir] : sy + c * rdy[sdir];

	score[i] += (flag ? d : c);
	if (OOB(sx, sy)) {
		santas[i] = { -1,-1,-1 };
		return;
	}
	int tmp = board[sx][sy]; // 밀려진 기존에 있던 산타 번호
	
	board[sx][sy] = i;
	santas[i] = { sx, sy, 2 }; // 기절처리

	if (tmp > 0) { // 기존에 산타가 있었다면
		queue<tuple<int, int, int>> q; 
		int x = sx;
		int y = sy;
		int nx = flag ? x + sdx[sdir] : x + rdx[sdir];
		int ny = flag ? y + sdy[sdir] : y + rdy[sdir];
		q.push({ nx, ny, tmp });

		while (!q.empty()) {

			int x1, y1, tmp1;
			tie(x1, y1, tmp1) = q.front(); q.pop();

			if (OOB(x1, y1)) {
				santas[tmp1] = { -1,-1,-1 };
				break;
			}
			else if (board[x1][y1] == 0) {
				board[x1][y1] = tmp1;
				santas[tmp1] = { x1, y1, get<2>(santas[tmp1]) };
				break;
			}
			else if (board[x1][y1] > 0) {
				int tmp2 = board[x1][y1];
				board[x1][y1] = tmp1;
				santas[tmp1] = { x1, y1,  get<2>(santas[tmp1]) };
				int x2 = flag ? x1 + sdx[sdir] : x1 + rdx[sdir];
				int y2 = flag ? y1 + sdy[sdir] : y1 + rdy[sdir];
				q.push({ x2, y2, tmp2 });
			}
		}

	}
	
	
	

	
}

void RMove() {
	// 가장 가까운 산타 탐색
	
	vector<tuple<int, int, int, int>> infos; // 거리, 행, 열 , 산타번호

	for (int i = 1; i <= p; i++) {
		int sx, sy;
		tie(sx, sy, ignore) = santas[i];
		if (sx == -1) continue; // 게임에서 탈락시 스킵 
		auto sPos = santas[i];
		
		int dist = getDist(rx, ry, sx, sy );
		
		infos.push_back({ dist, sx, sy , i });
	}

	if (infos.empty()) exit(0); // 산타가 없을일은 없음

	sort(infos.begin(), infos.end(), Cmp); // 거리작고 행열큰 산타 탐색
	
	
	int sx, sy, n;
	tie(ignore, sx, sy, n) = infos.front();

	// 돌진


	int mDist = 0x7f7f7f7f;
	int mrnx = -1; int mrny = -1; int mdir = -1;
	for (int dir = 0; dir < 8; dir++) {
		int rnx = rx + rdx[dir];
		int rny = ry + rdy[dir];
		if (OOB(rnx, rny)) continue;
		int dist = getDist(rnx, rny, sx, sy);
		
		if (dist < mDist) {
			mDist = dist;
			mrnx = rnx; mrny = rny; 
			mdir = dir;
		}
	}

	// n번 산타 충돌 처리 필요
	if (mrnx == sx && mrny == sy) // 루돌프와 충돌
	{
		board[sx][sy] = 0;
		Push(mrnx, mrny, n, mdir, 0);
	}

		

	rx = mrnx;
	ry = mrny; 


}

void SMove() {


	for (int i = 1; i <= p; i++) {
		int sx, sy, sCnt;
		tie(sx, sy, sCnt) = santas[i];
		if (sx == -1 || sCnt > 0) continue; // 탈락하거나 기절
		
		// 산타 이동
		
		int mDist = getDist(sx, sy, rx, ry);
		int msnx = -1; int msny = -1; int mdir = -1;

		for (int dir = 3; dir >= 0; dir--) { // 상우하좌 우선순위
			int snx = sx + sdx[dir];
			int sny = sy + sdy[dir];
			if (OOB(snx, sny) || board[snx][sny] > 0) // 격자밖이나 다른 산타
				continue;
			int dist = getDist(snx, sny, rx, ry);
			
			if (dist <= mDist) { // 현재보다 루돌프가 가까워지는 최단위치이면
				mDist = dist;
				msnx = snx; msny = sny; mdir = dir;
			}
		}

		if (msnx != -1) { // 가까워지는 경우
			if (msnx == rx && msny == ry) // 루돌프와 충돌
			{
				board[sx][sy] = 0;
				Push(msnx, msny, i, mdir, 1);
			}
			else { // 빈칸으로 이동
				board[sx][sy] = 0;
				board[msnx][msny] = i;
				santas[i] = { msnx , msny, 0 };
			}
		}
		
		// 나머지는 산타 정지
	}
}

void Set() { // 기절 및 턴종료 점수 세팅
	for (int i = 1; i <= p; i++) {
		int sx, sy, sCnt;
		tie(sx, sy, sCnt) = santas[i];
		if (sx == -1) continue;
		score[i]++;
		if (sCnt > 0) {
			sCnt--;
			santas[i] = { sx, sy, sCnt };
		}
	}
}

bool IsNoSanta() {
	for (int i = 1; i <= p; i++) {
		int sx, sy, sCnt;
		tie(sx, ignore, ignore) = santas[i];
		if (sx != -1) return false;
	}
	return true;
}


void Solve() {
	cin >> n >> m >> p >> c >> d;
	cin >> rx >> ry;
	rx = rx - 1; ry = ry - 1;

	for (int i = 1; i <= p; i++) {
		int num, sx, sy;
		cin >> num >> sx >> sy;
		santas[num] = { sx - 1, sy - 1 , 0};
		board[sx - 1][sy - 1] = num;
	}

	for (int t = 1; t <= m; t++) {
		RMove();
		SMove();
		Set();


		if (IsNoSanta()) break;
	}


	for (int i = 1; i <= p; i++) {
		cout << score[i] << ' ';
	}
}

int main(void) {
	Solve();

	return 0;
}