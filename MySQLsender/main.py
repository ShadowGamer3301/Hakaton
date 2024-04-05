import mysql.connector
import configparser

config = configparser.ConfigParser()
config.read("sendreq.ini")

lname = config["user"]["lname"]
fname = config["user"]["fname"]

mydb = mysql.connector.connect(
  host="localhost",
  user="root",
  password="",
  database="hakaton"
)

req = "SELECT photo_path FROM login WHERE firstname = '" + fname + "' AND lastname = '" + lname + "';"

mycursor = mydb.cursor()
mycursor.execute(req)
myresult = mycursor.fetchall()

print(myresult[0])
