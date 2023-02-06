# Tests

### Test 0:
Input file:
```
,A,B,Cell
1,1,0,1
2,2,30,0
30,0,2,5
```
Output:
```
,A,B,Cell
1,1,0,1
2,2,30,0
30,0,2,5
```
### Test 1:
Input file:
```
,A,B,Cell
1,1,0,1
2,2,=A1+Cell30,0
30,0,=B1+A1,5
```
Output:
```
,A,B,Cell
1,1,0,1
2,2,6.0,0
30,0,1.0,5
```
### Test 2:
Input file:
```
,A,B,Cell
1,1,0,1
2,2,=A1+Cell30,0
30,0,=A2/B1,5

```
Output:
```
invalid expression: division by zero
```
B1 = 0
### Test 3:
Input file:
```
,A,N,XyZ 
6,5,1,8
,G,K,7,u
30,5,8,0
```
Output:
```
amount of commas in this file is incorrect
```
actually the point is - there's two lines with commas at the beginning
### Test 4:
Input file:
```
,A,b,J
6,8,7
6,7
```
Output:
```
amount of commas in this file is incorrect
```
### Test 5:
Input file:
```
,g,k,w,u
3,,,,
5,6,7,0,0
1,=g3-w5,6,8,7
```
Output:
```
invalid expression: cell is empty
```
g3 is empty
### Test 6:
Input file:
```
,A,H,Un
8,1,1,1
7,0,=Un1-A8,1
1,1,1,=H7*Un8
```
Output:
```
data dependency
```
Un1 and H7 depend on each other
### Test 7:
Input file:
```
,A,B,C,D
2,5,6,1,0
3,=C2/4,0,0,0
```
Output:
```
,A,B,C,D
2,5,6,1,0
3,0.250,0,0,0
```
### Test 8:
Input file:
```
,A,I,O,Z
1,6,=I2*A2,=Z2/A2,1
2,3,5,8,9
```
Output:
```
,A,I,O,Z
1,6,15.0,3.0,1
2,3,5,8,9
```
### Test 9:
Input file:
```
,A,B,C
2,6,=A22-A2,7
22,4,6,0
```
Output:
```
,A,B,C
2,6,-2.0,7
22,4,6,0
```
this test means that lines with same characters at the beginning are differed
### Test 10:
Input file:
```
5,A,B,Cell
1,1,0,1
2,2,30,0
30,0,2,5
```
Output:
```
no header with column names in this file
```
### Test 11:
Input file:
```
,A,B,C
22,4,-6,0
2,6,=A22-A2,7
```
Output:
```
,A,B,C
22,4,-6,0
2,6,-2.0,7
```
### Bonus:
this program checks whether you entered a .csv file or not