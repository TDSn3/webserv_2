#! /usr/bin/env python3

import cgi
import sys

content = "Script CGI en Python :"

for arg in sys.argv:
	content += "\n" + arg

content_length = len(content.encode('utf-8'))



print("HTTP/1.1 200 OK")

print("content-type: text/html")
# print(f"Content-Length: {content_length}")
print()
print()
print(content)
