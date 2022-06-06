Student: Sofia Meropi Katsaouni  AM: 1115201800070

~~README file for hw2-spring-2022.pdf~~

Execution:

Compile both server and client with: make
( clean object files with: make clean )
( clean object files and compile: make comp )

Run server with: ./dataServer -p <port> -s <thread_pool_size> -q <queue_size> -b <block_size>
Run client with: ./remoteClient -i <server_ip> -p <server_port> -d <directory>

[ i personally run server and client on two terminals the following way (example): 
 ./dataServer -p 12500 -s 2 -q 2 -b 512
 ./remoteClient -i linux12.di.uoa.gr -p 12500 -d ../code-6-set ]

Code deficiency:
- The program is fully implemented but works for one client at a time and not many clients
  at the same time.

*******************************************************
Other than the above the code should execute just fine.
for any questions message me at: sdi1800070@di.uoa.gr
*******************************************************

---- When running the program expect the execution to look like this ----
-------------------------------------------------------------------------
                       _##### Server #####_
-------------------------------------------------------------------------
Server's parameters are: 
port: 12500
thread_pool_size: 2
queue_size: 2
block_size: 512
Server was successfully initialized...
Listening for connections to port 12500
Accepted connection from linux12.di.uoa.gr
[Thread: 117769984]: About to scan directory ../code-6-set
[Thread: 117769984]: Adding file ~#@../code-6-set to the queue…
[Thread: 100984576]: Received task: <~#@../code-6-set, 4>
[Thread: 117769984]: Adding file ../code-6-set/detachedThread.c to the queue…
[Thread: 109377280]: Received task: <../code-6-set/detachedThread.c, 4>
[Thread: 117769984]: Adding file ../code-6-set/incprint.c to the queue…
[Thread: 117769984]: Adding file ../code-6-set/create_many_threads.c to the queue…
[Thread: 109377280]: Received task: <../code-6-set/incprint.c, 4>
                .
                .
                .

-------------------------------------------------------------------------
                       _##### Client #####_
-------------------------------------------------------------------------
Client's parameters are:
serverIP: linux12.di.uoa.gr
port: 12500
directory: ../code-6-set
Connecting to linux12.di.uoa.gr on port 12500
Received: code-6-set
Received: code-6-set/detachedThread.c
Received: code-6-set/incprint.c
Received: code-6-set/create_many_threads.c
Received: code-6-set/twordcount3.c
                .
                .
                .

-------------------------------------------------------------------------

Terminate server with: Ctrl+C

The code for my program is in the src directory. As you may notice above,
the server adds directory code-6-set to the queue with the prefix ~#@ . 
It's a trick i did so that the client can make a distinction between 
directories, files and file contents. files have the $^$* prefix and 
file contents have no prefix at all.

******* My code was implemented with the help of the following pdfs.

cgi.di.uoa.gr/~mema/courses/k24/lectures/topic5-Sockets.pdf
cgi.di.uoa.gr/~mema/courses/k24/lectures/topic6-Threads.pdf
cgi.di.uoa.gr/~mema/courses/k24/lectures/topic3-UnixFileCommands.pdf

As for the queue implementation i used my own files from 
assignements i had for the subject Data Structures *******

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ FILES ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
client.c :           Contains main code for the client.

create_directory.c:  CLient uses the file to collect data from server, create the 
                     directory, create the files and fill them with the corresponding 
                     contents.

server.c:            Contains main code for the server.

directory.c:         Server uses the file to receive the directory name, and send files 
                     and file contents to the client. All communication thread and worker
                     thread useful code is there.

queue.c:             implementation of C queue obviously.

validation.c:        Both client and server uses this file. It Contains functions to check
                     if the command line parameters were given correctly, and functions to 
                     print errors in code if any.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

There are comments everywhere in the files explaining the thought behind The
implementation. I made sure the code was clean enough for a 2nd person to 
understand. For any further questions message me.
