************************************************************************
file with basedata            : cr441_.bas
initial value random generator: 645668803
************************************************************************
projects                      :  1
jobs (incl. supersource/sink ):  18
horizon                       :  120
RESOURCES
  - renewable                 :  4   R
  - nonrenewable              :  2   N
  - doubly constrained        :  0   D
************************************************************************
PROJECT INFORMATION:
pronr.  #jobs rel.date duedate tardcost  MPM-Time
    1     16      0       21       14       21
************************************************************************
PRECEDENCE RELATIONS:
jobnr.    #modes  #successors   successors
   1        1          3           2   3   4
   2        3          3           5   7  14
   3        3          3          11  13  14
   4        3          3           6   7  16
   5        3          3           6   9  13
   6        3          2           8  17
   7        3          2           8  13
   8        3          1          10
   9        3          2          10  12
  10        3          1          11
  11        3          1          15
  12        3          2          16  17
  13        3          2          15  17
  14        3          2          15  16
  15        3          1          18
  16        3          1          18
  17        3          1          18
  18        1          0        
************************************************************************
REQUESTS/DURATIONS:
jobnr. mode duration  R 1  R 2  R 3  R 4  N 1  N 2
------------------------------------------------------------------------
  1      1     0       0    0    0    0    0    0
  2      1     2       0    5    0   10    6    8
         2     3       0    0    0   10    5    8
         3     3       0    3    9   10    4    8
  3      1     3       0    4    0    0    8    3
         2     3       0    0    8    0    9    3
         3     4       5    5    5    3    7    3
  4      1     5       0    6    0    5    9   10
         2     7       0    0    0    5    9    8
         3     8       0    5    6    0    8    7
  5      1     2       0    3    2    6    9    6
         2     4       0    0    2    3    5    3
         3     5       3    0    0    0    3    3
  6      1     1       0    8    9    4    8    7
         2     4       6    0    0    0    6    7
         3     9       0    0    0    4    6    6
  7      1     7       4    0    0    0   10    6
         2     8       0    6    4    0    3    4
         3     8       0    0    0    6    6    5
  8      1     3       4    0    8    0    4   10
         2     8       4    4    6    0    3    7
         3    10       0    0    0    4    2    3
  9      1     1       7    6    0    3    5   10
         2     9       0    0    4    1    4    8
         3     9       0    0    5    1    5    5
 10      1     2       6    0    5   10    8    7
         2     3       6    5    0    7    5    5
         3     6       5    4    0    5    5    3
 11      1     1       5    3    8    0    7    4
         2     3       0    0    3    0    5    3
         3     3       5    0    0    0    3    2
 12      1     8       0    8    0    0    9    5
         2     9       5    7    2    9    8    5
         3    10       0    7    0    5    8    5
 13      1     6       9   10    0    6    4   10
         2     9       0    0    0    3    2    8
         3    10       5    0    0    0    1    7
 14      1     1       0    0    8    0    9    5
         2     4       7    0    0    2    6    5
         3     8       7    7    6    1    4    5
 15      1     3       0    6    5    6    2    8
         2     6       0    5    0    4    2    7
         3     9       2    0    0    0    2    7
 16      1     3       9    3    0    0    8    3
         2     5       4    1    8    9    8    2
         3    10       4    0    0    0    8    1
 17      1     2       2    2    0    8    3    9
         2     7       2    0    0    0    3    7
         3     8       1    0    0    0    3    6
 18      1     0       0    0    0    0    0    0
************************************************************************
RESOURCEAVAILABILITIES:
  R 1  R 2  R 3  R 4  N 1  N 2
   10   13    6   13   91   93
************************************************************************