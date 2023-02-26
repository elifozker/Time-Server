# Time-Server

Create a simple time server that takes a time request
from a client using “telnet” console command on a Linux machine.

Socket communication, you will create a server that takes date request from a
client and send the correct date data that is requested.


The basic function of your time server would be getting the inputs given above, using
“date” command to generate correct output and returning the output to the client. For any
other kind of input (including lowercase versions of above examples) just send
“INCORRECT REQUEST” to the client without needing any further explanation.

Only one connection (one client) is enough for time server. You are not required to
implement a multi user time server for this assignment. You are also required to use port
“60000” for this communication. You should define this port as it is show in below and
close it when you receive “CLOSE_SERVER” command from client before closing the
program. This is especially important because if it is not closed, next program will not be
able to use this port.

<img width="501" alt="Ekran Resmi 2023-02-26 ÖS 7 05 52" src="https://user-images.githubusercontent.com/111710264/221421991-adcd7444-4111-4611-8831-047d154bfce5.png">
