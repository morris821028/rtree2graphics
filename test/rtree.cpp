#include <bits/stdc++.h>
using namespace std;

const int MAXN = 30000;
const int MAXM = 2;
const int MAXLOGN = 16;
const int MAXXY = 10000;
struct Rect {
	int lx, ly, rx, ry;
	Rect(int lx=0, int ly=0, int rx=0, int ry=0):
		lx(lx), ly(ly), rx(rx), ry(ry) {}
	int intersect(const Rect &other) const  {
		return max(lx, other.lx) < min(rx, other.rx)
				&& max(ly, other.ly) < min(ry, other.ry);
	}
	int intersect_area(const Rect &other) const {
		int px = max(lx, other.lx), py = max(ly, other.ly);
		int qx = min(rx, other.rx), qy = min(ry, other.ry);
		return (qx - px)*(qy - py);
	}
	void extend(const Rect &other) {
		lx = min(lx, other.lx), ly = min(ly, other.ly);
		rx = max(rx, other.rx),	ry = max(ry, other.ry);
	}
};

class Rtree {
public:
	struct Node {
		Rect sub, *eof;
		Node* son[MAXM];
		int size;
		void update() {
			sub = Rect(MAXXY, MAXXY, -MAXXY, -MAXXY);
			size = 0;
			for (int i = 0; i < MAXM; i++) {
				if (son[i] != NULL) {
					sub.extend(son[i]->sub);
					size += son[i]->size;
				}
			}
		}
	};
	struct cmp {
		const int sortDidx;
		cmp(const int sortDidx): sortDidx(sortDidx) {}
        bool operator()(const Rect* x, const Rect* y) const {
			if (sortDidx == 0)
				return x->lx+x->rx < y->lx+y->rx;
			else
				return x->ly+x->ry < y->ly+y->ry;
        }
    };
	Node nodes[MAXN*MAXLOGN];
	Rect pt[MAXN], *A[MAXN];
	Node *root;
	int N;
	
	int bufsize;
	void init() {
		bufsize = 0;
	}
	
	Node* newNode() {
		Node *ret = &nodes[bufsize++];
		assert(bufsize < MAXN*MAXLOGN);
		memset(ret->son, 0, sizeof(ret->son));
		ret->eof = NULL;
		return ret;
	}
	
	void prepare(Rect pt[], int N) {
		init();
		for (int i = 0; i < N; i++) {
			this->pt[i] = pt[i];
			this->A[i] = &(this->pt[i]);
		}
		this->N = N;
		root = build(0, 0, N-1);
	}
	
	Node* build(int k, int l, int r) {
		if (l > r)
			return NULL;
		if (k == 2)	k = 0;
		Node *ret = newNode();
		int m = (l+r)/2;
		sort(A+l, A+r, cmp(k));
//		nth_element(A+l, A+m, A+r, cmp(k));
		if (l != r) {
			int t = r-l+1;
			int branch = min(t, MAXM);
			for (int i = 0; i < branch; i++)
				ret->son[i] = build(k+1, l+i*t/branch, l+(i+1)*t/branch-1);
			ret->update();
		} else {
			ret->sub = *A[m], ret->eof = A[m];
			ret->size = 1;
		}
		return ret;
	}
	
	int q_Ct;
	Rect *q_C[MAXN];
	void dfs(Node *u, Rect B) {
		if (u == NULL)
			return ;
		if (!u->sub.intersect(B))
			return ;
		if (u->eof) {
			q_C[q_Ct++] = u->eof;
			return ;
		}
		for (int i = 0; i < MAXM; i++)
			dfs(u->son[i], B);
	}
	int find(Rect B) {
		q_Ct = 0;
		dfs(root, B);
		return q_Ct;
	}
	
	void serial_print(Node *u) {
		if (u == NULL)
			return ;
		if (u->eof) {
			printf("R%d ", u->eof - pt);
			return ;
		}
		printf("[ ");
		for (int i = 0; i < MAXM; i++) {
			serial_print(u->son[i]);
			if (i != MAXM-1)
				printf(", ");
		}
		printf(" ]");
	}
	void serial() {
		printf("%d\n", N);
		for (int i = 0; i < N; i++) {
			printf("R%d %d %d %d %d\n", i, pt[i].lx, pt[i].ly, pt[i].rx, pt[i].ry);
		}
		serial_print(root);
	}
};

int main() {
	int N;
	static Rect A[MAXN];
	static Rtree tree;
	scanf("%d", &N);
	for (int i = 0; i < N; i++) {
		int lx, ly, rx, ry;
		scanf("%d %d %d %d", &lx, &ly, &rx, &ry);
		A[i] = Rect(lx, ly, rx, ry);
	}
	tree.prepare(A, N);
	tree.serial();
	return 0;
}
/*
10
64 0 72 8
75 3 84 23
47 27 58 45
51 88 57 97
59 46 70 54
64 45 69 58
60 54 75 61
45 82 52 96
34 65 51 82
65 32 79 48
*/
