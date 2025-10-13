chat server

server
    take messages from client
        broadcast that message to all connected clients

join server
create server

create server
    open port for listening
    handle new connections

join server
    bind to port

tasks: 
1. create an echo server - done
2. implement server
3. handle multiple concurrent connections
4. command line/login page thing

server tasks:
1. read thread
2. write thread
3. message queue (mutex, atomic?)
