
FROM		debian:buster

RUN			apt-get update				\
				&& apt-get install -y	\
					procps				\
					gcc					\
					g++					\
					make				\
					valgrind			\
					python3

WORKDIR		/app
COPY		. /app

RUN			make re

EXPOSE		8080

# CMD			["./webserv"]
CMD			["valgrind", "--tool=memcheck", "--track-origins=yes", "--leak-check=full", "--show-leak-kinds=all", "--track-fds=yes", "./webserv"]
