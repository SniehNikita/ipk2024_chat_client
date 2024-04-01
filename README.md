# IPK2024 Chat client

Author: Sniehovskyi Nikita (xsnieh00). Date: 31.03.2024.

Table of contents:
1. [Overview of the project](#overview)
2. [Theory](#theory)
3. [Source code implementation](#source-code-implementation)
4. [Literature](#literature)
5. [Testing](#testing)



## Overview

Implementation of chat client which uses UDP and TCP protocols to communicate with server. It can get user input like messages and different commands which are then converted and sent to server. Project requirements are specified [here](https://git.fit.vutbr.cz/NESFIT/IPK-Projects-2024/src/branch/master/Project%201#user-content-program-execution)

Program is implemented usin C language.
Root directory contains Makefile file, to build project use 'build' command in terminal, which will generate ipk24chat-client executable.

List of executable arguments (specified [here](https://git.fit.vutbr.cz/NESFIT/IPK-Projects-2024/src/branch/master/Project%201#client-behaviour-input-and-commands)):
- -t: (required) tcp or udp Transport protocol used for connection
- -s: (required) IP address or hostname Server IP or hostname
- -p: (default is 4567) uint16 Server port
- -d: (default is 250) uint16 UDP confirmation timeout
- -r: (default is 3) uint8 Maximum number of UDP retransmissions
- -h: Prints program help output and exits

List of commands starting with '/' (specified [here](https://git.fit.vutbr.cz/NESFIT/IPK-Projects-2024/src/branch/master/Project%201#client-behaviour-input-and-commands)):
- /auth {Username} {Secret} {DisplayName}
- /join {ChannelID}
- /rename {DisplayName}
- /help
- /close: end communication with server

Everything that does not start with '/' is treated like message.



## Theory

From network perspective project can be divided into 3 problems:
- [UDP communication](#udp-communication)
- [TCP communication](#tcp-communication)
- [Message translation](#message-translation)

#### UDP Communication

Since whole communication is stateless some problems occure:
- Client identification: implemented using port numbers. Each client gets unique port number which is defined after successful authorization. Server will typicaly answer to authorization message from different port.
- Message delivery: to ensure that both parties have received message they should send confirm messages whenever they receive message from other side.

#### TCP Communication

Communication is stateful so there is much less problems within communication.

#### Message translation

For each protocol different message encoding is being used:
- UDP protocol uses custom message definition (specified [here](https://git.fit.vutbr.cz/NESFIT/IPK-Projects-2024/src/branch/master/Project%201#message-contents))
- TCP protocol is based on [ABNF](https://datatracker.ietf.org/doc/html/rfc5234). Rules are specified as follow:

```
; core content components
ID        = 1*20   ( ALPHA / DIGIT / "-" )
SECRET    = 1*128  ( ALPHA / DIGIT / "-" )
CONTENT   = 1*1400 ( VCHAR / SP )
DNAME     = 1*20   VCHAR

; additional content components
is     = SP "IS"    SP
as     = SP "AS"    SP
using  = SP "USING" SP

; message content variant parts
contentJoin     = "JOIN"     SP ID    as DNAME
contentAuth     = "AUTH"     SP ID    as DNAME using SECRET
contentMessage  = "MSG FROM" SP DNAME is CONTENT
contentError    = "ERR FROM" SP DNAME is CONTENT
;
contentReply    = "REPLY" SP ( "OK" / "NOK" ) is CONTENT
contentBye      = "BYE"

; message content variants
content  =  contentAuth
content  =/ contentJoin
content  =/ contentMessage
content  =/ contentError
content  =/ contentReply
content  =/ contentBye

; each message is is either variant terminated with "\r\n"
message = content CRLF
```
(specified [here](https://git.fit.vutbr.cz/NESFIT/IPK-Projects-2024/src/branch/master/Project%201#message-grammar))



## Source code implementation

File main.c contains implementation of server, other files contain utilities for correct work:
- main.h: entry point, main program
- types.h: definition of types used in program
- client.h: implementation of network part
- command.h: user command processing
- state.h; state transition calculator
- msg_compose.h: transtales internal message to byte array
- msg_parse.h: translates byte array to internal message
- err_out.h: error messages
- argv_parser.h: parser of arguments
- queue.h: implementation of queue

Diagram of all available states can be found [here](https://git.fit.vutbr.cz/NESFIT/IPK-Projects-2024/src/branch/master/Project%201#specification)

Main routine of program is while with polling function which checks stdio and socked file descriptors every 0.125s. Within routine theres 3 main processes:
- User input processing: Reads stdin fd -> processes command/message -> [generates](src/command.h) internal message type -> if message type is allowed in current state -> [sends](src/client.h) it to server
- Server input processing: Reads socket fd -> [converts](src/msg_parse.h) message into internal message type -> checks if received message is allowed in current state -> processes message. In case of UDP communication non-confirm messages are confirmed.
- UDP timeouts are checked: In order to not freeze program completely, messages that are waiting for confirmation are being stored in [message queue](src/queue.h), so user can immediately enter next input (he still cannot send it while previous message was not confirmed though). For each message timeout clock is being counted. In every polling timeouts are checked and if breached - message is resent/app. enters error state.
Also end-of-program conditions are also checked in polling routine so program won't be stopped in random places of program.



## Literature

1. [Project specification](https://git.fit.vutbr.cz/NESFIT/IPK-Projects-2024/src/branch/master/Project%201#user-content-program-execution)
2. [ABNF RFC5234](https://datatracker.ietf.org/doc/html/rfc5234)
3. [UDP RFC768](https://datatracker.ietf.org/doc/html/rfc768)
4. [TCP RFC9293](https://datatracker.ietf.org/doc/html/rfc9293)



## Testing

Complete functionality was tested using mock server responces injected into communication.

Common use cases where controlled using provided reference discord server wireshark in provided reference enviroment. Results were visualy confirmed in discord.

##### Test case 1: (TCP) auth -> SIGINT:
Testing: SIGINT
Result: OK

Flow:
```
./ipk24chat-client -t tcp -s anton5.fit.vutbr.cz -p 4567
/auth xsnieh00 3b67481f-b1d8-48d7-8532-d9150e5ea663 nekdo
Success: Authentication successful.
Server: nekdo joined discord.general.
```
Total message count (wireshark): 4
```
C → Server | AUTH xsnieh00 AS nekdo USING 3b67481f-b1d8-48d7-8532-d9150e5ea663
Server → C | REPLY OK IS Authentication successful.
Server → C | MSG FROM Server IS nekdo joined discord.general.
C → Server | BYE
```

##### Test case 2: (TCP) auth -> msg -> rename -> msg -> join -> msg -> /close:
Testing: common use case
Result: OK

Flow:
```
./ipk24chat-client -t tcp -s anton5.fit.vutbr.cz -p 4567
/auth xsnieh00 3b67481f-b1d8-48d7-8532-d9150e5ea663 nekdo
Success: Authentication successful.
Server: nekdo joined discord.general.
1
/rename a
1
/join discord.verified-1
Server: a joined discord.verified-1
Success: Channel discord.verified-1 successfully joined.
1
/close
```
Total message count (wireshark): 10
```
C → Server | AUTH xsnieh00 AS nekdo USING 3b67481f-b1d8-48d7-8532-d9150e5ea663
Server → C | REPLY OK IS Authentication successful.
Server → C | MSG FROM Server IS nekdo joined discord.general.
C → Server | MSG FROM nekdo IS 1
C → Server | MSG FROM a IS 1
C → Server | JOIN discord.verified-1 AS a
Server → C | MSG FROM Server IS a joined discord.verified-1.
Server → C | REPLY OK IS Channel discord.verified-1 successfully joined.
C → Server | MSG FROM a IS 1
C → Server | BYE
```

##### Test case 3: (TCP, Valgrind) auth -> msg -> rename -> msg -> join -> msg -> SIGINT:
Testing: memory leaks
Result: OK

Flow:
```
valgrind --leak-check=full --show-leak-kinds=all ./ipk24chat-client -t tcp -s anton5.fit.vutbr.cz -p 4567
/auth xsnieh00 3b67481f-b1d8-48d7-8532-d9150e5ea663 nekdo
Success: Authentication successful.
Server: nekdo joined discord.general.
1
/rename a
1
/join discord.verified-1
Server: a joined discord.verified-1
Success: Channel discord.verified-1 successfully joined.
1
/close
...
==50658== All heap blocks were freed -- no leaks are possible
...
```
Total message count (wireshark): 10
```
C → Server | AUTH xsnieh00 AS nekdo USING 3b67481f-b1d8-48d7-8532-d9150e5ea663
Server → C | REPLY OK IS Authentication successful.
Server → C | MSG FROM Server IS nekdo joined discord.general.
C → Server | MSG FROM nekdo IS 1
C → Server | MSG FROM a IS 1
C → Server | JOIN discord.verified-1 AS a
Server → C | MSG FROM Server IS a joined discord.verified-1.
Server → C | REPLY OK IS Channel discord.verified-1 successfully joined.
C → Server | MSG FROM a IS 1
C → Server | BYE
```


##### Test case 4: (UDP) auth -> SIGINT:
Testing: SIGINT
Result: OK

Flow:
```
./ipk24chat-client -t udp -s anton5.fit.vutbr.cz -p 4567
/auth xsnieh00 3b67481f-b1d8-48d7-8532-d9150e5ea663 nekdo
Success: Authentication successful.
Server: nekdo joined discord.general.
```
Total message count: 8
```
C → Server | AUTH xsnieh00 AS nekdo USING 3b67481f-b1d8-48d7-8532-d9150e5ea663
Server → C | Type=confirm, RefID=0
Server → C | REPLY OK IS Authentication successful.
C → Server | Type=confirm, RefID=0
Server → C | MSG FROM Server IS nekdo joined discord.general.
C → Server | Type=confirm, RefID=1
C → Server | BYE
Server → C | Type=confirm, RefID=1
```

##### Test case 5: (UDP) auth -> msg -> rename -> msg -> join -> msg -> /close:
Testing: common use case
Result: OK

Flow:
```
/ipk24chat-client -t tcp -s anton5.fit.vutbr.cz -p 4567
/auth xsnieh00 3b67481f-b1d8-48d7-8532-d9150e5ea663 nekdo
Success: Authentication successful.
Server: nekdo joined discord.general.
1
/rename a
1
/join discord.verified-1
Server: a joined discord.verified-1
Success: Channel discord.verified-1 successfully joined.
1
/close
```
Total message count: 20
```
C → Server | AUTH xsnieh00 AS nekdo USING 3b67481f-b1d8-48d7-8532-d9150e5ea663
Server → C | Type=confirm, RefID=0
Server → C | REPLY OK IS Authentication successful.
C → Server | Type=confirm, RefID=0
Server → C | MSG FROM Server IS nekdo joined discord.general.
C → Server | Type=confirm, RefID=1
C → Server | MSG FROM nekdo IS 1
Server → C | Type=confirm, RefID=1
C → Server | MSG FROM a IS 1
Server → C | Type=confirm, RefID=2
C → Server | JOIN discord.verified-1 AS a
Server → C | Type=confirm, RefID=3
Server → C | MSG FROM Server IS a joined discord.verified-1.
C → Server | Type=confirm, RefID=2
C → Server | REPLY OK IS Channel discord.verified-1 successfully joined.
C → Server | Type=confirm, RefID=3
C → Server | MSG FROM a IS 1
Server → C | Type=confirm, RefID=4
C → Server | BYE
Server → C | Type=confirm, RefID=5
```

##### Test case 6: (UDP, Valgrind) auth -> msg -> rename -> msg -> join -> msg -> /close:
Testing: memory leaks
Result: OK

Flow:
```
valgrind --leak-check=full --show-leak-kinds=all ./ipk24chat-client -t udp -s anton5.fit.vutbr.cz -p 4567
/auth xsnieh00 3b67481f-b1d8-48d7-8532-d9150e5ea663 nekdo
Success: Authentication successful.
Server: nekdo joined discord.general.
1
/rename a
1
/join discord.verified-1
Server: a joined discord.verified-1
1
/close
...
==50658== All heap blocks were freed -- no leaks are possible
...
```
Total message count: 20
```
C → Server | AUTH xsnieh00 AS nekdo USING 3b67481f-b1d8-48d7-8532-d9150e5ea663
Server → C | Type=confirm, RefID=0
Server → C | REPLY OK IS Authentication successful.
C → Server | Type=confirm, RefID=0
Server → C | MSG FROM Server IS nekdo joined discord.general.
C → Server | Type=confirm, RefID=1
C → Server | MSG FROM nekdo IS 1
Server → C | Type=confirm, RefID=1
C → Server | MSG FROM a IS 1
Server → C | Type=confirm, RefID=2
C → Server | JOIN discord.verified-1 AS a
Server → C | Type=confirm, RefID=3
Server → C | MSG FROM Server IS a joined discord.verified-1.
C → Server | Type=confirm, RefID=2
C → Server | REPLY OK IS Channel discord.verified-1 successfully joined.
C → Server | Type=confirm, RefID=3
C → Server | MSG FROM a IS 1
Server → C | Type=confirm, RefID=4
C → Server | BYE
Server → C | Type=confirm, RefID=5
```
