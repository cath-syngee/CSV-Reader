# CSV reader
This program reads CSV file, calculates cell values if necessary, and outputs the resulting table to the console. The cells may contain integer numbers and expressions like `= ARG1 OP ARG2`, where:
- `ARG1` and `ARG2` are integers or cell addresses in the format Column_Name Row_Number
- `OP` is an arithmetic operation such as +, -, *, /

### Example
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
2,2,6.0,0,
30,0,1.0,5
```

### To compile: 
You can copy this command in command line
```
g++ Source.cpp CSVreader.cpp -o csvreader
```
or simply use `make`

### To run:
This program reads only one file at a time, otherwise you get the `amount of files is incorrect` message. If you want to run on windows - ```csvreader.exe file.csv```. In case you use linux - ```./csvreader file.csv```

### To run tests on linux:
```
bash tests.sh
```
![](https://github.com/cath-syngee/CSV-Reader/blob/master/example.png)