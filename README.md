# Minimal-HTTP(S)Server in C

A minimal http(s) server that support php cgi and python cgi

A minimalist code base implementing either a http or https server. The code bases are identical except for the SSL alterations.

http server, compile with...

gcc -g -pthread -o http http.c functions.h

http://127.0.0.1:8080/index.html

https server, compile with...

gcc -g -pthread -o https2 https2.c -l ssl -l crypto

https://127.0.0.1:4433/index.html

The SSLCert directory contains the self signed certificate creation bash script.

The www directory are test pages that show server functions. Copy the www directory to the same folder where running the server.

