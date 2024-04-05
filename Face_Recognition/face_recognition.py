from deepface import DeepFace
import cv2
import os
import json
import time
import io
from ftplib import FTP
import numpy

#Zmienne i listy
user_data = {}

def read_file(filename): # Odczytywanie danych z pliku .json
    json_file = open(f"Face_Recognition/{filename}", "r")
    data = json_file.read()
    json_file.close()
    return data

def write_file(filename, content): # Zapisywanie nowych danych do pliku .json
    json_file = open(f"Face_Recognition/{filename}", "w")
    json.dump(content, json_file)
    json_file.close()
    return True

def get_user_photo(): #Pobieranie zdjęcia użytkownika z bazy dancyh
    file_name = open("Face_Recognition/photo.txt")
    photo_name = file_name.read()
    connection= FTP('192.168.159.148')
    connection.login()
    r = io.BytesIO()
    connection.retrbinary(f'RETR {str(photo_name)}', r.write)
    image = numpy.asarray(bytearray(r.getvalue()), dtype="uint8")
    image = cv2.imdecode(image, cv2.IMREAD_COLOR)
    return image


def verify_user(): #Weryfkiacja użytkownika po jego twarzy
    #haarcasde = cv2.CascadeClassifier('Face_Recognition/haarcascade_frontalface_default.xml')
    caputred_video = cv2.VideoCapture(0)
    Counter_Variable = 0
    is_face_matching = False

    while True:
        result, frame = caputred_video.read()
        if is_face_matching:
            print("Dostęp przyznany")
            write_file("confirmation.txt", "True")
            try:
                os.remove("Face_Recognition/TempPhoto/TempPhoto.jpg")
            except:
                None

            break
        else:
            print("Brak dostępu")

        if result:
            cv2.imwrite("Face_Recognition/TempPhoto/TempPhoto.jpg", frame)

            try:
                if DeepFace.verify(img1_path=get_user_photo(), img2_path="Face_Recognition/TempPhoto/TempPhoto.jpg")['verified']:
                    is_face_matching = True
                else:
                    is_face_matching = False
                    try:
                        os.remove("Face_Recognition/TempPhoto/TempPhoto.jpg")
                    except:
                        None

            except ValueError:
                is_face_matching = False
                try:
                    os.remove("Face_Recognition/TempPhoto/TempPhoto.jpg")
                except:
                    None

        time.sleep(0.2)
        if Counter_Variable >= 15:
            write_file("confirmation.txt", "False")
            break

        Counter_Variable +=1
    
    cv2.destroyAllWindows()

verify_user()




