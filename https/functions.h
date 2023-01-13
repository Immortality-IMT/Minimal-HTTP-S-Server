#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <libgen.h>
#include <arpa/inet.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

const char *get_content_type(char *extension) {
    if (strcmp(extension, ".html") == 0) {
        return "text/html";
    } else if (strcmp(extension, ".css") == 0) {
        return "text/css";
    } else if (strcmp(extension, ".js") == 0) {
        return "text/javascript";
    } else if (strcmp(extension, ".jpg") == 0) {
        return "image/jpeg";
    } else if (strcmp(extension, ".png") == 0) {
        return "image/png";
    } else if (strcmp(extension, ".webm") == 0) {
        return "video/webm";
    } else if (strcmp(extension, ".mp4") == 0) {
        return "video/mp4";
    } else if (strcmp(extension, ".svg") == 0) {
        return "image/svg+xml";
    } else if (strcmp(extension, ".json") == 0) {
        return "application/json";
    } else if (strcmp(extension, ".xml") == 0) {
        return "application/xml";
    } else if (strcmp(extension, ".txt") == 0) {
        return "text/plain";
    } else if (strcmp(extension, ".php") == 0) { //text/x-php
        return "text/html";
    } else if (strcmp(extension, ".py") == 0) { //text/plain, application/x-python, text/x-python, application/x-httpd-cgi or text/x-script.python
        return "text/html";
    } else {
        // For any other file types
        return "application/octet-stream";
    }
}

