#! /usr/bin/env python3

# **************************************************************************** #
#                                                                              #
#   Test pour télécharger un fichier texte									   #
#                                                                              #
#   http://localhost:8080/cgi-bin/download_test_2.cgi                          #
#                                                                              #
# **************************************************************************** #

import cgi
import sys

with open("www/test.html", "r") as f:
    content = f.read()

content_length = len(content.encode('utf-8'))

print("HTTP/1.1 200 OK")
print("Content-Type: text/plain")
print(f"Content-Length: {content_length}")
print("Content-Disposition: attachment; filename=\"test.html\"")
print()

print(content)
