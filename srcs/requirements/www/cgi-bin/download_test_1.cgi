#! /usr/bin/env python3

# **************************************************************************** #
#                                                                              #
#   Test pour télécharger un binaire										   #
#                                                                              #
#   http://localhost:8080/cgi-bin/download_test_1.cgi                          #
#                                                                              #
# **************************************************************************** #

import cgi
import sys

with open("www/favicon.png", "rb") as f:
    content = f.read()

content_length = len(content)

sys.stdout.write("HTTP/1.1 200 OK\r\n")

sys.stdout.write("Content-Type: application/octet-stream\r\n")
sys.stdout.write(f"Content-Length: {content_length}\r\n")
sys.stdout.write("Content-Disposition: attachment; filename=\"favicon.png\"\r\n")
sys.stdout.write("\r\n")
sys.stdout.flush()

sys.stdout.buffer.write(content)
