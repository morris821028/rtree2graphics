#include <bits/stdc++.h>
#include <random>
using namespace std;
double frandom() {
	return (double)(rand()%10) / 2;
}
double mrandom() {
	return (rand() * rand())%10;
}
int main() {
//	freopen("in.txt", "w", stdout);
    srand(time(NULL));
    int N;
    int testcase = 1;
//    printf("%d\n", testcase);
    while (testcase--) {
    	int n = 6;
    	printf("%d\n", n);
    	for (int i = 0; i < n; i++) {
    		while (1) {
	    		int lx = rand()%20;
	    		int ly = rand()%20;
	    		int rx = lx + rand()%5+1;
	    		int ry = ly + rand()%5+1;
	    		if (lx < rx && ly < ry) {
	    			printf("%d %d %d %d\n", lx, ly, rx, ry);
	    			break;
				}
    		}
		}
    	puts("");
	}
    return 0;
}
/*
GCGCAATG
3 1 1 3 0 0 2 1
GCCCTAGCG
2 0 3 2 0 1 2 0 1
*/
