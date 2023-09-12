# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tda-silv <tda-silv@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/08/14 16:32:49 by tda-silv          #+#    #+#              #
#    Updated: 2023/09/04 18:27:00 by tda-silv         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

all:
	@docker-compose -f ./srcs/docker-compose.yml up -d --build

ps:
	docker image ls
	@echo ""
	docker ps
	@echo ""
	docker volume ls	

start:
	@docker-compose -f ./srcs/docker-compose.yml start

stop: 
	@docker-compose -f ./srcs/docker-compose.yml stop

down: 
	@docker-compose -f ./srcs/docker-compose.yml down --rmi all --volumes

# **************************************************************************** #
#                                                                              #
#   Manual     														           #
#                                                                              #
# **************************************************************************** #

build_run:
	@docker build -t webserv srcs/requirements
	@docker run				\
		-d					\
		--name webserv		\
		-p 8080:8080		\
		--restart always	\
		webserv

build:
	@docker build -t webserv srcs/requirements

run_d:
	@docker run				\
		-d					\
		--name webserv		\
		-p 8080:8080		\
		webserv

run_it:
	@docker run				\
		-it					\
		--name webserv		\
		-p 8080:8080		\
		webserv				\
		bash

it_webserv:
	@docker exec -it $$(docker ps --filter name=webserv --format "{{.ID}}") bash

ps_webserv:
	@docker exec -it $$(docker ps --filter name=webserv --format "{{.ID}}") ps aux

cat_log_webserv cat:
	@docker exec -it $$(docker ps --filter name=webserv --format "{{.ID}}") cat log

kill_webserv:
	@docker kill $$(docker ps --filter name=webserv --format "{{.ID}}")

# **************************************************************************** #

clean:
	@docker-compose -f ./srcs/docker-compose.yml kill
	@echo ""
	@docker system prune -a -f
	@if [ $$(docker volume ls -q | wc -l) -gt 0 ]; then	\
		docker volume rm $$(docker volume ls -q);		\
	fi

fclean: clean
	@make -C srcs/requirements fclean
	@make -C client_test fclean

down_other_fclean downfc: down
	@make -C srcs/requirements fclean
	@make -C client_test fclean	

re: down all

.PHONY: all ps start stop down build_run build run_d run_it it_webserv ps_webserv cat_log_webserv cat kill_webserv clean fclean down_other_fclean downfc re
