from deepface import DeepFace
import cv2
import time
import os
import json

user_data = {"Photo": "Face_Recognition/object.jpg"}

def write_file(filename, content): # Zapisywanie nowych danych do pliku .json
    json_file = open(f"Face_Recognition/{filename}", "w")
    json.dump(content, json_file)
    json_file.close()
    return True

def keep_secure(): # 
    global user_data
    caputred_video = cv2.VideoCapture(0)
    is_face_matching = False
    timeout_variable = 0

    print("Zalogowano")

    while True:
        result, frame = caputred_video.read()

        if result:
            cv2.imwrite("Face_Recognition/TempPhoto/TempPhoto.jpg", frame)

            try:
                if DeepFace.verify(img1_path=user_data["Photo"], img2_path="Face_Recognition/TempPhoto/TempPhoto.jpg")['verified']:
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

            if is_face_matching:
                timeout_variable = 0
                is_face_matching = False
                print("Patrzy")
            else:
                timeout_variable += 1
                print("Niepatrzy")

            if timeout_variable >= 20:
                print("Wylogowano")
                break

    
    cv2.destroyAllWindows()

keep_secure()