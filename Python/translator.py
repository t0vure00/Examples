import pyrebase
import os
import firebase_admin
from firebase_admin import credentials
from firebase_admin import firestore
from google.cloud import storage
import cv2
import random
import authentication as auth


class TranslatorDataHandling:
    
    def __init__(self):
        self.IMG_ID = 1
        self.VID_ID = 2
        self.data_class = None
        self.letters = ["A","B","C","D","E","F","G","H","I","J","K","L",
                        "M","N","O","P","Q","R","S","T","U","V","W","X",
                        "Y","Z","Å","Ä","Ö"]
        cred = credentials.Certificate("serviceAccountKey.json")
        firebase_admin.initialize_app(cred)
        self.db = firestore.client()
        firebase = pyrebase.initialize_app(auth.getConf())
        self.storage = firebase.storage()

    def process_data(self, source_path, data_class):
        self.data_class = data_class
        if(source_path.endswith(".jpg")):
            self.__img_handling(source_path)
        elif(source_path.endswith(".mp4")):
            self.__vid_handling(source_path)
        else:
            raise(TypeError("File is not .jpg or .mp4"))

    def __img_handling(self, source):
        img = cv2.imread(source)
        cloud_dir = "tempForData\\imgf.jpg"
        vert_pic = cv2.flip(img, 1)
        cv2.imwrite(cloud_dir, vert_pic)
        self.__save_to_cloud(source, cloud_dir)

    def __vid_handling(self, source):
        vid_capture = cv2.VideoCapture(source)
        fourcc = cv2.VideoWriter_fourcc(*'mp4v')
        cloud_dir = "tempForData\\vidf.mp4"
        processed_vid = cv2.VideoWriter(cloud_dir, fourcc, 7.0, (400,256), 
                                        isColor=False)

        while(vid_capture.isOpened()):
            ret, frame = vid_capture.read()
            if(ret):      
                grayscale_frm = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)                                      
                resized_frm = cv2.resize(grayscale_frm, (400,256), fx=0,
                                        fy=0, interpolation=cv2.INTER_CUBIC)
                vert_frame = cv2.flip(resized_frm, 1)
                # Writing the processed video out
                processed_vid.write(vert_frame)
            else: break

        # Releasing the object, so we can delete the files later
        processed_vid.release()
        cv2.destroyAllWindows()
        self.__save_to_cloud(source, cloud_dir) 

    def __save_to_cloud(self, source, processed):
        rand_x = random.random()         #For naming a random numbers
        rand_y = random.random()

        # The data is put in collections based on the letter classification
        letter = self.letters[int(self.data_class)]
        # U to distinguish user based data
        label = "U" + letter

        storage_dir = "dataFromUsers/" + label + "/data" + str(rand_x + rand_y)
        # Now using FIREBASE STORAGE. Uploading the original data
        self.storage.child(storage_dir).put(source, auth.emailAndPassword())                           
        # Get URL of image
        url = self.storage.child(storage_dir).get_url(auth.emailAndPassword())
        # Now using FIRESTORE. Creating collection reference.
        self.db.collection(label + "-URL").add({'URL':url,'num':rand_x+rand_y}) 

        storage_dir = "dataFromUsers/" + label + "/dataf" + str(rand_x)
        # Same to the processed data
        self.storage.child(storage_dir).put(processed, auth.emailAndPassword())                         
        url = self.storage.child(storage_dir).get_url(auth.emailAndPassword())
        self.db.collection(label + "-URL").add({'URL':url,'num':rand_x})

        # Deleting the data from the server
        os.remove(source)
        os.remove(processed)


# For Testing
if __name__ == '__main__':
    data_handler = TranslatorDataHandling()
    data_handler.process_data("tempForData\\sign_for_a.jpg", 0)