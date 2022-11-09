import pyrebase
import os
import firebase_admin
from firebase_admin import credentials
from firebase_admin import firestore
from google.cloud import storage
import cv2
import random
import authentication as a



cred = credentials.Certificate("serviceAccountKey.json")
firebase_admin.initialize_app(cred)


db = firestore.client()                     #Defining the object for Firebase

firebase = pyrebase.initialize_app(a.getConf())
storage = firebase.storage()                #Defining the objet for Firebase Cloud Storage



def img_Or_Vid(path):                       #Seeing if the dat is image or video base on the file ending .jpg vai .mp4
    print("Ollaan funktiossa img_Or_Vid")

    file = path
    #for file in os.listdir(path):
    if file.endswith(".jpg"):
        print("Kuva oli: " + path)
        return 1,path
    elif file.endswith(".mp4"):
        print("Video oli: " + path)
        return 2,path
    else:
        print("Ei tunnistanut kuvaksi (.jpg) eikä videoksi (.mp4)")



def img_Handling(whereGet, dataClass):
    print("Nyt ollaan img_Handling funktiossa")
    pic = cv2.imread(whereGet)                                                          #Reading the image from the file.
    name = "imgf.jpg"
    dir1 = whereGet
    dir2 = "tempForData\\" + name
    cPic = cv2.flip(pic, 1)                                                             #Turning the picture to upright
    cv2.imwrite(dir2, cPic)
    saveToCloud(dir1, dir2, dataClass)


def vid_Handling(whereGet, dataClass):
    print("Nyt ollaan vid_Handling funktiossa")
    print("whereGet on: " + whereGet)

    vid_capture = cv2.VideoCapture(whereGet)
    print(vid_capture)

    
    fourcc = cv2.VideoWriter_fourcc(*'mp4v')

    name = "vidf.mp4"
    dir1 = whereGet
    dir2 = "tempForData\\" + name

    out = cv2.VideoWriter(dir2,fourcc, 7.0, (400,256), isColor=False)

    while(vid_capture.isOpened()):
        ret, frame = vid_capture.read()

        if ret == True:                                     #Processing the frame
            a = cv2.cvtColor(frame,cv2.COLOR_BGR2GRAY)                                      
            b = cv2.resize(a,(400,256),fx = 0,fy = 0, interpolation = cv2.INTER_CUBIC)    
            c = cv2.flip(b,1)
            #Writing the video out
            out.write(c)

        else:
    #print("Video was not found")
            break
    vid_capture.release()       # Releasing the object, so we can delete the files in sace function
    out.release()
    cv2.destroyAllWindows()

    saveToCloud(dir1,dir2, dataClass)

    

    


def saveToCloud(data, dataf, dataClass):
    print("Ollaan save funktiossa ja saatiin: ", data," ja ", dataf)

    x = random.random()         #For naming a random number generator
    y = random.random()

    #The data that gets labeled A goes all to the same collection, the U is for user based data
    datanluokka = whatClass(int(dataClass))
    if datanluokka == "":
        print("Ei tullut luokaksi mitään arvoa")
    else:
        label = 'U' + datanluokka

        dirInStorage = "dataFromUsers/" + label + "/"

        name = "data" + str(x + y)
        print("Kuvan nimeksi tulee: " + name)

        #HOX! Now using FIREBASE STORAGE.
        #Uploading the picture
        #Child parametere is the wanted name for the data, if destination is a in another file the path is "A/my_image" for exmp. 
        #put paramete are the path of the file
        storage.child(dirInStorage + name).put(data, a.emailAndPassword())                           

        #Get URL of image
        url = storage.child(dirInStorage + name).get_url(a.emailAndPassword())
        print(url)
 
        #HOX! Now using FIRESTORE.
        db.collection(label + '-URL').add({'URL':url,'num':x+y}) 

        name = "dataf" + str(x)
        print("Kuvan nimeksi tulee: " + name)

        #Same to the turned picture
        storage.child(dirInStorage + name).put(dataf, a.emailAndPassword())                           

        url = storage.child(dirInStorage + name).get_url(a.emailAndPassword())
        print(url)
          
        db.collection(label + '-URL').add({'URL':url,'num':x+y})

    #Deleting the picture from the server
    os.remove(data)
    os.remove(dataf)



def whatClass(x):
    print("Ollaan what Class funktissa")

    classes = ["A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z","Å","Ä","Ö"]
    return classes[x]



def dataReceive(dataClass, whereGet):
    print("Ollaan data Editing For Saving osiossa ja dataReceive funktiossa ja saatiin parametreinä path: " + whereGet + " ja ennustuksesta data luokka ", dataClass)

    state,path = img_Or_Vid(whereGet)

    print("Käytiin katsomassa kuva vai video ja saatiin state (1=kuva, 2=video): ", state , " ja path: " + path)

    if state == 1:  
        print("Tila oli 1, eli käsitellään kuva")
        whereGet = path
        img_Handling(whereGet, dataClass)
    elif state == 2:
        print("Tila oli 2, eli käsitellään video")
        whereGet = path
        vid_Handling(whereGet, dataClass)
    else:
        print("Nyt meni joku vikaan, ei ohjelman mukaan saatu kuvaa tai videota. HUOM oikeat muodot .jpg ja .mp4")



if __name__ == '__main__':
    path = "tempForData\\vid.mp4"


