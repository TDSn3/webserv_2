# webserv
Create a basic HTTP server from scratch in C++.

## Usage

- ` make ` : Creates and starts two containers. One with webserv connected to port ` 8080 ` and another for testing purposes with Nginx connected to port ` 8181 `.
- ` make cat_log_webserv ` : Displays webserv log file. It lists every connection and **HTTP** request received.
- ` make ps ` :  View images, running containers, and volumes.
- ` make it_webserv ` :  Enters the webserv container in interactive mode.
- ` make start ` : Starts containers.
- ` make stop ` : Stops containers.
- ` make down ` : Stops and deletes containers and their images.
- ` make fclean ` : Stops and deletes **ALL** containers **(Note that this will delete all your other containers, if any)**.
- Other commands are available in the Makefile.

The configuration file is located in ` srcs/requirements/conf/webserv.conf `

## Comparison between webserv and nginx

``` bash
cd client_test
make
./client_test <port>
```
Compiles and starts the test client on the ` port ` passed as an argument. By default, use ` 8080 ` to send to **webserv** and ` 8181 ` to **nginx**.
Use ` client_test/http_request.txt ` to customise your HTTP request.

## Todo

### Config file parsing
- [x] Choose the port and host of each server.  
- [x] Setup the ```server_names``` or not.  
- [x] The first server for ```host:port``` will be the default one.  
- [x] Setup default error pages.  
- [x] Limit client body size.  
- For routes:  
- [x] Define a list of accepted HTTP methods  
- [x] Define a HTTP redirection  
- [x] Define a root directory  
- [x] Turn on or off directory listing  
- [x] Set a default file to answer if the request is a directory  
- [x] Execute CGI based on certain file extension  
- [x] Make it work with POST and GET methods  
- [x] Make the route able to accept uploaded files and configure where they should be saved  

curl -v -H "Transfer-Encoding: chunked" -d @Makefile http://127.0.0.1:8080/post_body
curl -v -H "Transfer-Encoding: chunked" --data-binary @Makefile http://127.0.0.1:8080/post_body
curl -v -T Makefile http://127.0.0.1:8080/post_body
curl -v -H "Transfer-Encoding: chunked" -T Makefile http://127.0.0.1:8080/post_body
curl -v -X POST -F "file=@Makefile" http://localhost:8080/test_POST.mk