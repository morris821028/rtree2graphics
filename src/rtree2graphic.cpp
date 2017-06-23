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
	
	int readInt() {
		char c;
		int x = 0;
		while (isdigit((c = getchar())))
			x = x * 10 + c-'0';
		ungetc(c, stdin);
		return x;
	}
	
	Node* deserial_read() {
		char c;
		while (isspace((c = getchar())));
		
		if (c == ']') {
			ungetc(c, stdin);
			return NULL;
		}		
		Node *u = newNode();
		if (c == 'R') {
			int x = readInt();
			u->eof = &pt[x], u->sub = pt[x];
			return u;
		}
		for (int i = 0; i < MAXM; i++) {
			u->son[i] = deserial_read();
			if (u->son[i]) {
				while (isspace((c = getchar())));
				if (c != ',') {
					ungetc(c, stdin);
					break;
				}
			}
		}
		u->update();
		while (isspace((c = getchar())));	// read ]		
		return u;
	}
	
	void deserial() {
		char name[16];
		int lx, ly, rx, ry, id;
		scanf("%d", &N);
		for (int i = 0; i < N && scanf("%s %d %d %d %d", name, &lx, &ly, &rx, &ry) == 5; i++) {
			sscanf(name+1, "%d", &id);
			pt[i] = Rect(lx, ly, rx, ry), A[i] = &pt[i];
		}
		root = deserial_read();	
	}
	
	int tex_ele_print(Node *u, double shift, string styles[], int styLen, int styId, int &innerId) {
		printf("\\begin{scope}\n");
		printf("\\coordinate(L) at (%lf%+lf,%lf%+lf);\n", 1.f*u->sub.lx, -shift, 1.f*u->sub.ly, -shift);
		printf("\\coordinate(R) at (%lf%+lf,%lf%+lf);\n", 1.f*u->sub.rx, shift, 1.f*u->sub.ry, shift);
		printf("\\draw[%s] (L) rectangle (R);\n", styles[min(styLen-1, styId)].c_str());
		if (u->eof)
			printf("\\node[xshift=0.8em,yshift=-0.8em] at (%lf+%lf, %lf+%lf) {R%d};\n", 
				1.f*u->sub.lx, -shift, 1.f*u->sub.ry, shift, u->eof-pt);
		else
			printf("\\node[xshift=0.8em,yshift=-0.8em] at (%lf+%lf, %lf+%lf) {R%d};\n", 
				1.f*u->sub.lx, -shift, 1.f*u->sub.ry, shift, innerId), innerId++;
		printf("\\end{scope}\n\n");
	}
	
	int tex_print(Node *u, string styles[], int styLen, int &innerId) {
		if (u == NULL)
			return -1;
		if (u->eof) {
			tex_ele_print(u, 0, styles, styLen, 0, innerId);
			return 0;
		}
		int mx = 0;
		for (int i = 0; i < MAXM; i++) {
			int tmp = tex_print(u->son[i], styles, styLen, innerId);
			mx = max(mx, tmp+1);
		}
		tex_ele_print(u, mx*0.2, styles, styLen, mx, innerId);
		return mx;
	}
	
	
	void tex_begin() {
		puts("\\documentclass[tikz,border=5pt,11pt]{standalone}");
		puts("\\begin{document}");
  		puts("  \\begin{tikzpicture}");
  		puts("\n");
	}
	void tex_end() {
		puts("\n");
		puts("  \\end{tikzpicture} ");
		puts("\\end{document}");
	}
	void tex() {
		string styles[5] = {
			"color=red",
			"color=gray, dashed",
			"color=black",
			"color=blue",
			"color=green",
		};
		int styLen = 5, innerId = N;
		tex_begin();
		tex_print(root, styles, styLen, innerId);
		tex_end();
	}
};

int main() {
	int N;
	static Rect A[MAXN];
	static Rtree tree;
	tree.deserial();
	tree.tex();
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
