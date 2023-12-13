# CS 4310: Project 2: Student Database with TCP
# Description:
- Course: CS 4310 - Computer Networks | Instructor: Dr. Xiao Chen, Associate Professor
- A simple database that stores student information (ID, Name, Score) on host server (Eros) and allows user to add entry, search for ID or score, delete entry, etc. on the client server (Zeus) . This program used UDP as the transfer protocol.

# Instruction:
1. [Transfer files to servers] scp client_tcp.c server_tcp.c [TXState ID]@eros.cs.txstate.edu:~\
2. [Enter Txstate Password]...
3. [Open a window on Eros] ssh [TXState ID]@eros.cs.txstate.edu
4. [Open a window on Zeus] ssh [TXState ID]@zeus.cs.txstate.edu
5. [Compile and run the following on Eros first]
gcc -o s server_tcp.c
./s
6. [Combine and run the following on Zeus]
gcc -o c client_tcp.c
./c eros.cs.txstate.edu

# README:
- Program is partially incomplete and does not include:
1. a function that read student information from text file.
2. a function that check whether input information is correct type (int, char)
3. a function that mallocate new size of struct object.

![Client program](https://i.imgur.com/Fku1Z76.png)
![Server program](https://i.imgur.com/ddNQ4iM.png)

# Log:
TCP Student Database
- Mar 3, 10 PM | Added last name. Fix formatting. TO DO: read students info from database.
- Mar 3, 9 PM | Fixed searchScore(). Edited formating. Still need to make read textfil for student info. Add lastName attribute to struct.
