#! /usr/bin/env python3

import cgi
import sys

content = "Script CGI en Python :"

for arg in sys.argv:
	content += "\n" + arg



print("Status: 200 OK")

print(f"Content-Type: text/html; charset=utf-8")
print()

print(content)
