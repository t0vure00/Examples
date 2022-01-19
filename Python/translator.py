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


db = firestore.client()                     #määritellään db olio Firestoreen

firebase = pyrebase.initialize_app(a.getConf())
storage = firebase.storage()                #määritellään storage olio Firebase/Cloud Storageen



def img_Or_Vid(path):                       #katsotaan onko kuva vai video nimen perusteella eli onko .jpg vai .mp4
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
    pic = cv2.imread(whereGet)                                                          #Lukee kuvan tiedostosta.
    name = "imgf.jpg"
    dir1 = whereGet
    dir2 = "tempForData\\" + name
    cPic = cv2.flip(pic, 1)                                                             #Kuvan kääntäminen, jos laittaa 0 kääntää pystysuunnassa, 1:llä vaakatasossa
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

        if ret == True:
            a = cv2.cvtColor(frame,cv2.COLOR_BGR2GRAY)                                      #En ymmärrä mksi ei toimi ilman tätä
            b = cv2.resize(a,(400,256),fx = 0,fy = 0, interpolation = cv2.INTER_CUBIC)      #ja tätä, mutta ei toimi
            c = cv2.flip(b,1)
            #kirjoitetaan video
            out.write(c)

        else:
    #print("Ei löytyny videota")
            break
    vid_capture.release()       #Päästetään irti jo tässä, että save funktiossa voidaan poistaa tiedostot
    out.release()
    cv2.destroyAllWindows()

    saveToCloud(dir1,dir2, dataClass)

    

    


def saveToCloud(data, dataf, dataClass):
    print("Ollaan save funktiossa ja saatiin: ", data," ja ", dataf)

    x = random.random()         #nimeämistä varten vaan laitoin tällasen random number generator, niin toivottavasti ei satu samoja nimiä :)
    y = random.random()

    #Miten labelin saa tähän, että esim A kirjaimeksi päätelly menee kaikki samaan collectioniin jne.. U koska käyttäjältä saadut
    datanluokka = whatClass(int(dataClass))
    if datanluokka == "":
        print("Ei tullut luokaksi mitään arvoa")
    else:
        label = 'U' + datanluokka

        dirInStorage = "dataFromUsers/" + label + "/"

        name = "data" + str(x + y)
        print("Kuvan nimeksi tulee: " + name)

        #HUOM KYSE FIREBASEN STORAGESTA
        #Kuvan lataaminen
        #child parametreihin laitetaan haluttu nimi tiedostolle, jos haluaa kansioon tallentaa niin esim "A/my_image"
        #put parametreihin tulee tiedoston polku/sijainti, ei onnistunut itsellä arrayn laittaminen
        storage.child(dirInStorage + name).put(data, a.emailAndPassword())                           

        #Get URL of image
        url = storage.child(dirInStorage + name).get_url(a.emailAndPassword())
        print(url)
 
        #HUOM NYT FIRESTORESTA KYSE
        db.collection(label + '-URL').add({'URL':url,'num':x+y}) 

        name = "dataf" + str(x)
        print("Kuvan nimeksi tulee: " + name)

        #Sama käännetyllä kuvalle
        storage.child(dirInStorage + name).put(dataf, a.emailAndPassword())                           

        url = storage.child(dirInStorage + name).get_url(a.emailAndPassword())
        print(url)
          
        db.collection(label + '-URL').add({'URL':url,'num':x+y})

    #Poistetaan kuvat serveriltä
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


