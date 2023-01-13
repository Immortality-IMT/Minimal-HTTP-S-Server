#!/usr/bin/python
import os
import random
import cgi
import mysql.connector

print("Content-Type: text/html")    # The type of content being sent
print()                             # Leave a blank line
print("<h1>Hello, World!</h1>")     # The actual content


current_directory = os.getcwd()
print("The current working directory is", current_directory)

print ('<br>')
print ('<br>')

num = random.randint(1, 100)
is_prime = True

for i in range(2, num):
    if num % i == 0:
        is_prime = False
        break

if is_prime:
    print(num, "is a prime number.")
else:
    print(num, "is not a prime number.")

print ('<html>')
print ('<head>')
print ('<title>Hello Word</title>')
print ('</head>')
print ('<body>')
print ('<h2>Hello Word! This is my first CGI program</h2>')
print ('</body>')
print ('</html>')

num1 = "4"
num2 = "7"
result = int(num1) + int(num2)
print("The result is:", result)

print ('<br>')
print ('<br>')

with open("www/python/text_file.txt", "r") as file:
    print(file.read())






# Connect to the database
cnx = mysql.connector.connect(user='root', password='<insert password here>',
                              host='localhost',
                              database='gochat')

# Create a cursor
cursor = cnx.cursor()

# Execute the SELECT statement
query = "SELECT * FROM chatlog"
cursor.execute(query)

# Fetch and print the result
result = cursor.fetchall()
for row in result:
    print(row)

# Close the cursor and connection
cursor.close()
cnx.close()


