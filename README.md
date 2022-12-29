# Assembler-Project
Final project in the C programming course (20465) at the Open University.  
This project was developed by a team of two.

The program implements an Assembler, which translates code written in Assembly language to binary code.  
**Note:** the computer model and Assembly language utilizd in this project are both fictional and defined by the university.  

## Usage  
Use makefile to compile the program:  
```
>   make
```  
After preparing Assembly files with an ``.as`` extension, pass their names as arguments to the command line:  
```
>   ./assembler file1 file2 file3  
```  
The Assembler will generates output files with the same input file names, only with the following extensions:  
- ``.ob`` - Object file  
- ``.ent`` - Entries file  
- ``.ext`` - Externals file  

## Example  
Input source file name ``ps.as``  

```
;file ps.as  
;sample source code  

.entry  Next  
.extern wNumber  
STR:    .asciz "aBcd"  
MAIN:   add    $3, $5, $9  
LOOP:   ori    $9, -5, $2  
        la     val1  
        jmp    Next  
Next:   move   $20, $4  
LIST:   .db    6,-9  
        bgt    $4,$2,END  
        la     K  
        sw     $0,4,$10  
        bne    $31,$9, LOOP  
        call   val1  
        jmp    $4  
        la     wNumber  
.extern val1  
        .dh    27056  
K:      .dw    31,-12  
END     stop  
.entry  K  
```  

The Assembler scans and parses the file and generates the following output files:  
- ``ps.ob`` - Object file:  
```
;ps.ob file  

      52  17  
0100  40  48  65  00  
0104  FB  FF  22  35  
0108  00  00  00  7C  
0112  74  00  00  78  
0116  40  20  80  06  
0120  1C  00  82  48  
0124  A1  00  00  7C  
0128  04  00  0A  58  
0132  E4  FF  E9  3F  
0136  00  00  00  80  
0140  04  00  00  7A  
0144  00  00  00  7C  
0148  00  00  00  FC  
0152  61  42  63  64  
0156  00  06  F7  B0  
0160  69  1F  00  00  
0164  00  F4  FF  FF  
0168  FF
```  
- ``ps.ent`` - Entries file:  
```
;ps.ent file  

Next  116  
K  0161  
```  
- ``ps.ext`` - Externals file:  
```
;ps.ext file  

val1  0108  
val1  0136  
wNumber  0144  
```  





