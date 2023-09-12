
#
# For testing
#

FROM		debian:buster

RUN			apt-get update				\
				&& apt-get install -y	\
					nginx				\
					python3

COPY		./conf/nginx.conf /etc/nginx/conf.d
COPY		./tools /var/www/html

EXPOSE		8181

CMD			["nginx", "-g", "daemon off;"]
