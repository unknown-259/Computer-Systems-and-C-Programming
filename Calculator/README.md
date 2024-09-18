Names: Thuy Lan Phan (tphan17@ucsc.edu) and Nathan Tran (ntran48@ucsc.edu)


Parts coded by who:

The CalculatorRun function was written by Nathan as well as half of the operation functions. Thuy Lan wrote the other half of the operation functions and wrote the README.pdf. 

Questions:

1. What happens if the line "scanf(" ");" executes? Why?

	The line scanf() is used to read characters or data that's from the standard input and then interprets and stores them accordingly. The problem is that when you run scanf(), not every character gets processed, the whitespace characters get left behind. More specifically, the space " " in scanf(" ") is a whitespace specifier and discards zero or more whitespace characters. This is so when you run scanf() again, any of the newlines from a previous call of scanf() is gone.

2. In this lab, we forbid you from using printf() or scanf() inside certain functions. Explain why a rule like this is useful.

	A rule that impedes you from using printf() or scanf() from inside certain functions is not only more challenging for us to figure out, as the most intuitive solution is to directly use printf() in the situation where we are, for example dividing by zero, but it's good practice to not do so. It means that we are forced to return an error and have the code respond accordingly. Using printf() or scanf() in every function would not only drastically make our code longer and harder to follow for peers, but it will also produce more room for errors. When it comes to debugging the code, it could be time consuming to figure out where the error is located at.

3. How long did this lab take you? Was it harder or easier than you expected? What challenges did you encounter?

	The lab was confusing at first, we both had to spend time to figure out C as it's unfamiliar for both of us, but Nathan also had to use Ubuntu and the Linux timeshare for the first time. He had trouble running it the first couple times, but the TAs were a great helper for resolving this issue. While Nathan was responsible for the main parts of the program, it wouldn't have been possible if it wasn't for Thuy Lan's understanding of the lab document. For Thuy Lan, it was definitely rather daunting at first but once she started it was easier than she expected compared to when first reading the lab document. She personally had a difficult time dealing with the divide function, as she was unsure how to deal with the situation where someone tries to divide by zero, but after reading the lab document again and some time we were able to figure it out. In total, we spent probably a couple hours on this purly coding, though a lot of time was spent reading the and understanding the lab document.
