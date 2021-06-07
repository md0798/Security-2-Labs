Name - Meet Desai
UNI- mpd2155

Problem 1:

Part A:

The code first takes input from the shadow_a file and then extracts salt and password hash for admin and root and stores them in different variables. The code executes sequentially, i.e., first it searches for admin's password and then for root's password. First the code decodes the admins base64 password hash and computes hash of different dates starting from 19000101 till 2021, I keep increasing the date by 1 and increase a month when date gets bigger than 31 and increase a year when month get bigger than 12. I started the date from 19000101 to increase the speed of code execution. I have kept the whole thing in a while loop with the condition that when the hash of date is equal to the decoded hash the while loop breaks. After it finds the date corresponding to admin's password, it resets all the values calculates base64 decode for root's password hash and then it starts searching for root's password using the same method as described above for admin's password.


Part B:

The code first takes input from the shadow_b file and then extracts salt and password hash for bob and alice and stores them in different variables.  I have also included a word.txt file along with other files, This files contains over 15000 6-letter english words all in small letters. The code executes sequentially, i.e., first it searches for bob's password and then for alice's password. First the code decodes the root's base64 password hash, then I open the word.txt files and extract a single word from it since the word stored in the word.txt file is in small letters, the code computes hash of 64 different combinations of the same word with different big and small letter and compares it with the decoded password hash, after this the code extracts the next word from the word.txt file and repeats the same procedure. If a w If no word from the file matches the password hash then the code starts computing hash of different strings starting from AAAAAA. It starts increasing from the 6th letter, After the a letter reaches Z it jumps to a and then the letter on the left is incremented as a letter reaches z, the letter itself is rested to A. I have kept the whole thing in a while loop with the condition that when the hash of date is equal to the decoded hash the while loop breaks. After it finds the password corresponding to Bob's password it resets all the values calculates base64 decode for alice's password hash and it starts searching for Alice's password using the same method as described above for bob's password.


Execution:

The files submitted are Part1.c, Part2.c, Makefile, word.txt, Problem1.

To execute shadow_a and shadow_b should be in the same folder as all the other files. The whole code can be executed using the following command in the terminal.

$bash Problem1  


