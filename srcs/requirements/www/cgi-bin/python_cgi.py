#! /usr/bin/env python3

import cgi
import sys
import os

content = "Script CGI en Python :"

for arg in sys.argv:
	content += "\n" + arg

content_length = len(content.encode('utf-8'))



print("status: 200 OK")

print("content-type: text/html")
# print(f"Content-Length: {content_length}")
print()
print(str(sys.argv))
print(os.getcwd())
print(str(os.environ["PATH_INFO"]))
print(content)
