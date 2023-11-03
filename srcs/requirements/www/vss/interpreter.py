#! /usr/bin/env python3

import cgi
import sys
import os


form = cgi.FieldStorage()

client_code = form.getvalue('client_code', '')
result = ''

print("status: 200 OK")
print("content-type: text/html")
print()
exec(client_code)

