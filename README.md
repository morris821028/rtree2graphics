## README ##

### Usage ###

* gcc 4.8+
* pdflatex

### Introduction ###

For example, there is a graphic with six rectangles.

```
6
11 16 12 20
14 6 19 9
4 1 6 6
12 14 17 17
3 14 4 15
1 16 5 18
```

First, we can use the r-tree builder `./rtree <test.in` which is provided by
the this repository (That is not great, but it work for demo).  Then, it
output the format as follows:

```
6
R0 11 16 12 20
R1 14 6 19 9
R2 4 1 6 6
R3 12 14 17 17
R4 3 14 4 15
R5 1 16 5 18
[ [ R2 , [ R4 , R0  ] ], [ R1 , [ R3 , R5  ] ] ]
```

The above text has two parts.  The first part is the information of
rectangles.  The second part is the structure of r-tree which use the inorder
traversal.  Of course, you can adjust the string to make it easy-to-view.

```
6
R0 11 16 12 20
R1 14 6 19 9
R2 4 1 6 6
R3 12 14 17 17
R4 3 14 4 15
R5 1 16 5 18
[ 
	[ 	
		R2 , 
		[ R4 , R0 ] 
	], 
	[ 
		R1 , 
		[ R3 , R5  ] 
	] 
]
```

```bash
./rtree2graphic <test.out >test.tex
pdflatex test.tex
open text.pdf
```
