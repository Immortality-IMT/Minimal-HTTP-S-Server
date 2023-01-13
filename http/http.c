#include "functions.h"

#define BUFFER_SIZE 1024 * 1024
#define MAX_CLIENTS 100

void send_404(int client_fd);

int client_fds[MAX_CLIENTS];
pthread_t client_threads[MAX_CLIENTS];
int num_clients = 0;
int server_fd;

int is_regular_file(const char *path)
{
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISREG(path_stat.st_mode);
}

void signal_handler(int signum) 
{
    printf("Received signal %d, shutting down the server\n", signum);
    close(server_fd); // Close the server socket
    exit(0); // Exit the program
}

void send_404(int client_fd)
{
    char * message = "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\n\r\n<html><head><title>404 Not Found</title></head><body><h1>404 Not Found</h1>The requested URL was not found on this server.</body></html>";
    send(client_fd, message, strlen(message), 0);
    close(client_fd);
}


void *handle_client(void *arg)
{

    int client_fd = *((int *)arg);
    char buffer[BUFFER_SIZE];
    int num_bytes;

    // Read the HTTP request frombuffer the client
    num_bytes = recv(client_fd, buffer, BUFFER_SIZE - 1, 0);

        char *copy_buffer_gen = strdup(buffer);

        if (copy_buffer_gen == NULL) {
        //Handle error 
        }

    if (num_bytes == 0) {
        // Connection has been closed by the client
        close(client_fd);
        return NULL;
    }

    buffer[num_bytes] = '\0';

    // Parse the HTTP request to extract the requested URL
    char *request_line = strtok(copy_buffer_gen, "\r\n");

    char *url = strtok(request_line, " ");
    url = strtok(NULL, " ");

  //    printf("E: %s", url);

    // Remove the leading '/' from the URL
    if (url[0] == '/') {
        url++;
    }

    // Open the file corresponding to the requested URL
    char file_path[BUFFER_SIZE];
    sprintf(file_path, "./www/%s", url);

    //Needs a valid path and filename
    int ret = is_regular_file(file_path);
    if (ret != 1) {
        send_404(client_fd);
        return NULL;
    }

    char *extension = strrchr(url, '.');

    const char* file_type = get_content_type(extension);

    // Check the file type, if it is PHP, handle it differently
    if (strcmp(extension, ".php") == 0) {

        /*Generate this in a script to execute, requires post 
          content and the file returns filename for system to execute
          #!/bin/sh
          REQUEST_DATA="var_1=val_1&var_2=val_2"
          export GATEWAY_INTERFACE="CGI/1.1"
          export SERVER_PROTOCOL="HTTP/1.1"
          export QUERY_STRING="test=querystring"
          export REDIRECT_STATUS="200"
          export ="/test.php"
          export REQUEST_METHOD="POST"
          export CONTENT_LENGTH=${#REQUEST_DATA}
          export CONTENT_TYPE="application/x-www-form-urlencoded;charset=utf-8"
          echo $REQUEST_DATA | /usr/bin/php-cgi
         */

        char *copy_buffer = strdup(buffer);

        if (copy_buffer == NULL) {
        //Handle error 
        }

        char *headers_end = strstr(copy_buffer, "\r\n\r\n");
        char *query_string_copy;
        int query_string_len = 0;

        if (headers_end) {
            char *query_string = headers_end + 4;
            size_t query_string_len = strlen(query_string);
            query_string_copy = strndup(query_string, query_string_len);

               //printf("Query string: %s ", query_string_copy);

            if (query_string_copy != NULL) {
                setenv("QUERY_STRING", query_string_copy, 1);
                query_string_len = strlen(query_string_copy);
               // free(query_string_copy);
            }
        }
 
        //Extract the relevant parts from the header
        char *req = strtok(copy_buffer, " ");
        char *request_uri = strtok(NULL, " ");
        char *query_string = strchr(request_uri, '?');

        /*
        setenv("GATEWAY_INTERFACE", "CGI/1.1", 1);
        setenv("SERVER_PROTOCOL", "HTTP/1.1", 1);
        setenv("QUERY_STRING", query_string_copy, 1);
        setenv("REDIRECT_STATUS", "200", 1);
        setenv("SCRIPT_FILENAME", file_path, 1);
        setenv("REQUEST_METHOD", "POST", 1);
        char query_string_len_str[10];
        sprintf(query_string_len_str, "%d", query_string_len);
        setenv("CONTENT_LENGTH", query_string_len_str, 1);
        setenv("CONTENT_TYPE", "application/x-www-form-urlencoded;charset=utf-8", 1);
        */

        //Turn the PHP request into a shell to pass to php-cgi
        FILE *shell_script; // Create and open a .sh file

        char cwd[1024];
        getcwd(cwd, sizeof(cwd));
        char cgi_script_path[1280];
        sprintf(cgi_script_path, "%s/cgi.sh", cwd);

        shell_script = fopen(cgi_script_path, "w");
        fprintf(shell_script, "#!/bin/sh\n");
        fprintf(shell_script, "REQUEST_DATA=\"%s\"\n", query_string_copy);
        fprintf(shell_script, "export GATEWAY_INTERFACE=\"CGI/1.1\"\n");
        fprintf(shell_script, "export SERVER_PROTOCOL=\"HTTP/1.1\"\n");
        fprintf(shell_script, "export QUERY_STRING=\"%s\"\n", query_string_copy);
        fprintf(shell_script, "export REDIRECT_STATUS=\"200\"\n");
        fprintf(shell_script, "export SCRIPT_FILENAME=\"%s\"\n", file_path);
        fprintf(shell_script, "export HTTP_COOKIE=\"user_id=mehs\"\n");
        fprintf(shell_script, "export REQUEST_METHOD=\"%s\"\n", req);
        fprintf(shell_script, "export CONTENT_LENGTH=${#REQUEST_DATA}\n");
        fprintf(shell_script, "export CONTENT_TYPE=\"application/x-www-form-urlencoded;charset=utf-8\"\n");
        fprintf(shell_script, "echo $REQUEST_DATA | /usr/bin/php-cgi -q");
        fclose(shell_script);

        char chmod_script[1536];
        sprintf(chmod_script, "chmod 777 %s", cgi_script_path);
        system(chmod_script);

        //Execute the script
        char temp_file_path[] = "/tmp/temp.txt";
        char command[100];
        sprintf(command, "./cgi.sh > %s", temp_file_path);
        int ret = system(command);

        strcpy(file_path, temp_file_path);

    } else if (strcmp(extension, ".py") == 0) {

        printf("%s", buffer);

 char *copy_buffer = strdup(buffer);

        if (copy_buffer == NULL) {
        //Handle error 
        }

        char *headers_end = strstr(copy_buffer, "\r\n\r\n");
        char *query_string_copy;
        int query_string_len = 0;

        if (headers_end) {
            char *query_string = headers_end + 4;
            size_t query_string_len = strlen(query_string);
            query_string_copy = strndup(query_string, query_string_len);

               //printf("Query string: %s ", query_string_copy);

            if (query_string_copy != NULL) {
                setenv("QUERY_STRING", query_string_copy, 1);
                query_string_len = strlen(query_string_copy);
               // free(query_string_copy);
            }
        }
 
        //Extract the relevant parts from the header
        char *req = strtok(copy_buffer, " ");
        char *request_uri = strtok(NULL, " ");
        char *query_string = strchr(request_uri, '?');


        setenv("GATEWAY_INTERFACE", "CGI/1.1", 1);
        setenv("SERVER_PROTOCOL", "HTTP/1.1", 1);
        setenv("QUERY_STRING", query_string_copy, 1);
        setenv("REDIRECT_STATUS", "200", 1);
        setenv("SCRIPT_FILENAME", file_path, 1);
        setenv("REQUEST_METHOD", req, 1);
        char query_string_len_str[10];
        sprintf(query_string_len_str, "%d", query_string_len);
        setenv("CONTENT_LENGTH", query_string_len_str, 1);
        setenv("CONTENT_TYPE", "application/x-www-form-urlencoded;charset=utf-8", 1);
        //printf("X: %s\r\n", req);
 
        char temp_file_path[] = "/tmp/temp.txt";
        char command[1048592];
        sprintf(command, "%s > %s", file_path, temp_file_path);
        int ret = system(command);

        strcpy(file_path, temp_file_path);

}

        /** Send it **/

        // Open the file
        FILE *file = fopen(file_path, "rb");
        if (file == NULL) {
            printf("File not found: %s\n", file_path);
            send_404(client_fd);
        return NULL;
        }
        // Determine the file size
        fseek(file, 0, SEEK_END);
        long file_size = ftell(file);
        rewind(file);
        // Read the file contents into memory
        char *file_contents = (char *)malloc(file_size);
        fread(file_contents, 1, file_size, file);
        fclose(file);
        // Send the HTTP response
        char response[BUFFER_SIZE];
        sprintf(response, "HTTP/1.1 200 OK\r\nContent-Type: %s\r\nContent-Length: %ld\r\n\r\n", file_type, file_size);
        send(client_fd, response, strlen(response), 0);
        send(client_fd, file_contents, file_size, 0);
        // Clean up
        free(file_contents);
        close(client_fd);

    close(client_fd);
    return NULL;
}

int main(int argc, char ** argv)
{

  int server_fd, client_fd;
  struct sockaddr_in server_addr, client_addr;
  socklen_t sin_size;

  // Set up the signal handler
  signal(SIGINT, signal_handler);

  // Parse the port number from the command-line arguments
  int PORT = 8000;
  if (argc > 1) {
    PORT = atoi(argv[1]);
  }

  // Create a socket for the server
  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    perror("socket");
    exit(1);
  }

  // Configure the server address
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(PORT);
  server_addr.sin_addr.s_addr = INADDR_ANY;
  memset(server_addr.sin_zero, '\0', sizeof(server_addr.sin_zero));

  // Bind the socket to the server address
  if (bind(server_fd, (struct sockaddr * ) & server_addr, sizeof(struct sockaddr)) == -1) {
    perror("bind");
    exit(1);
  }

  // Listen for incoming connections
  if (listen(server_fd, 10) == -1) {
    perror("listen");
    exit(1);
  }

  printf("Server is listening on port %d\n", PORT);

  // Accept incoming connections and create a new thread for each client
  while (1) {
    sin_size = sizeof(struct sockaddr_in);
    if ((client_fd = accept(server_fd, (struct sockaddr * ) & client_addr, & sin_size)) == -1) {
      perror("accept");
      continue;
    }

    //printf("New client connected\n");

    // Add the client socket to the client_fds array
    client_fds[num_clients] = client_fd;

    // Create a new thread to handle the client
    if (pthread_create( & client_threads[num_clients], NULL, handle_client, & client_fds[num_clients]) != 0) {
      perror("pthread_create");
      continue;
    }

    num_clients++;

  }

  return 0;
}
