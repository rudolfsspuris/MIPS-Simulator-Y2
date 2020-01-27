Architecture II Coursework
==========================

My partner's and my task was to develop a MIPS CPU simulator, which can accurately execute
MIPS-1 big-endian binaries. I also needed to develop a testbench
which was able to test the MIPS simulator.


Reflection
==========================

These are some of my reflections on the project, parts of which were submitted with the project.

Time management
---------------

While we did start the project a week late I do not feel like I mismanaged my time. Honestly, I believe I invested more time into the project than I should have, more work should have been left for my partner. I should have practiced setting clearer deadlines for both him and me, those being varied on the difficulty of the task.

Software skills (e.g. debugging, design, coding)
------------------------------------------------

Clearly understanding the requirements of a piece of software is the first task a programmer should do and which I did not when my first undertaking - creating the memory map had to be remade at the end of the project because I had specifically programmed it so that instruction memory would only be accessible with the program counter. Thus, instruction loading had to be implemented for every load instruction afterwards when at the start I could have created a function which would handle that. While my approach of planning out the functionality of the software yielded code that was more structured, I should have paid more attention to these mistakes at the beggining as they were harder to fix later down the line.


Tools and Infrastructure (e.g. OS, automation, ...)
---------------------------------------------------

Learning the tools I definitely sped up coding: git integration into Atom meant I didn’t have to bother with the console, familiarising myself with the parser my coursemate provided meant I learned that I can pass in txt files. Familiarisation also meant that I learned the bugs in the parser which explained some wrong results and learned how to deal with clashing git master branch versions.  An improvement I could have used is creating a simulator-test, which uses the same structures as the real simulator but always prints the registers (then I wouldn’t have to change the simulator every time I wanted to debug).


Understanding of computer architecture
--------------------------------------

Originally, I had planned to program the simulator by fully implementing the data-paths that we looked at in lectures as I believed it would give me an easier way to figure out the quirks and details of all the instructions. This would have meant creating a structure called ALU, functions that sign extend numbers and variables that change cycle to cycle essentially representing wires. I did not follow through as I realised that the added layer of complexity would only add extra problems for the simulator and decided to take a more result oriented approach. However some parts of the project still were modeled after a real CPU and the knowledge of the hardware allowed me to stucture my cone more neatly. 




