#! /usr/bin/env python3

import cgi
import sys

form = cgi.FieldStorage()

expression = form.getvalue('expression', '')
result = ''

try:
	result = eval(expression)
except:
	pass

if not expression or not result:
	file = 'www/vss/calculator_failure.html'
else:
	file = 'www/vss/calculator_success.html'

with open(file, 'r') as f:
	content = f.read()

content = content.format(_expression = expression, _result = result)


print("status: 200 OK")

print("content-type: text/html")
print()
print(content)
