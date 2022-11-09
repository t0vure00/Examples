package com.example.viittomatulkki

/*
Part of a translator app. This is the main view that opened up.
It for example handled the communication with firebase, where the database for
the pictures and user info was.
 */


import android.Manifest
import android.app.Activity
import android.content.Context
import android.os.Bundle
import android.content.Intent
import android.database.Cursor
import android.graphics.Bitmap
import android.graphics.drawable.BitmapDrawable
import android.net.Uri
import android.provider.MediaStore
import android.util.Log
import android.view.View
import androidx.appcompat.app.AppCompatActivity

import com.google.firebase.auth.FirebaseAuth
import com.google.firebase.auth.ktx.auth
import com.google.firebase.ktx.Firebase
import com.google.firebase.storage.ktx.storage
import org.opencv.android.OpenCVLoader

import org.opencv.android.Utils
import org.opencv.core.Core
import org.opencv.core.CvType
import org.opencv.core.Mat
import org.opencv.imgproc.Imgproc
import org.opencv.core.Size
import java.io.ByteArrayOutputStream
import kotlin.random.Random
import android.widget.*
import com.google.firebase.firestore.ktx.firestore
import java.io.File


var storageRef = Firebase.storage.reference
private lateinit var auth: FirebaseAuth
private const val REQUEST_VIDEO_CAPTURE = 1
private const val REQUEST_PICTURE_CAPTURE = 2
class FourthActivity() : AppCompatActivity(){
    companion object {
        private const val TAG = "FourthActivityTranslate"
        private val obj = MachineLearning()
    }

    private val db = Firebase.firestore

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_fourth)

        auth = Firebase.auth

        //Making sure OpenCV is installed
        if (OpenCVLoader.initDebug()){
            Log.d(TAG,"OpenCVInstalled")
        }else{
            Log.d(TAG,"Opencv not installed")
        }

        onClickButtonListenerLopeta()
        onClickButtonListenerOtaKuva()
        onClickButtonListenerOtavideo()
    }


    fun onClickButtonListenerLopeta() {
        val button_Lopeta = findViewById<Button>(R.id.button_lopeta)
        button_Lopeta!!.setOnClickListener {
            //Signing out
            auth.signOut()
            //Return to sign in
            this.finish()
        }
    }


    fun onClickButtonListenerOtaKuva()
    {
        //Taking the picture: https://developer.android.com/training/camera/photobasics
        val button_otakuva = findViewById<Button>(R.id.button_picture)
        button_otakuva.setOnClickListener {
            //Taking the picture with the phones camera app, making a new Intent for it
            val takePictureIntent = Intent(MediaStore.ACTION_IMAGE_CAPTURE)

            if(takePictureIntent.resolveActivity(this.packageManager) != null) {
                startActivityForResult(takePictureIntent, REQUEST_PICTURE_CAPTURE)
            }else{
                Toast.makeText(this,"Ei saatu kameraa auki", Toast.LENGTH_SHORT).show()
            }
        }


    }


    fun onClickButtonListenerOtavideo()
    {
        //Taking the video: https://developer.android.com/training/camera/videobasics
        val button_otavideo = findViewById<Button>(R.id.button_video)
        button_otavideo.setOnClickListener {
            //Taking the video with the phones camera app, making a new Intent for it
            val takeVideoIntent = Intent(MediaStore.ACTION_VIDEO_CAPTURE)

            if(takeVideoIntent.resolveActivity(this.packageManager) != null) {
                startActivityForResult(takeVideoIntent, REQUEST_VIDEO_CAPTURE)
            }else{
                Toast.makeText(this,"Ei saatu kameraa auki", Toast.LENGTH_SHORT).show()
            }
        }
    }

    //Functio made for receiving the pic/video with the examples from the links
    override fun onActivityResult(requestCode: Int, resultCode: Int, data: Intent?) {
        Log.d(TAG, data.toString())
        //Defining the imageView objects to point to widgets
        val imageView: ImageView = findViewById(R.id.imageView)
        val videoView : VideoView = findViewById(R.id.videoView)
        //Setting the video- and imageView to transparent
        imageView.setVisibility(View.INVISIBLE)
        videoView.setVisibility(View.INVISIBLE)

        if (requestCode == REQUEST_PICTURE_CAPTURE && resultCode == Activity.RESULT_OK) {
            //If result is ok and our request code is equal to request code
            //Setting imageView back to visible
            imageView.setVisibility(View.VISIBLE)


            //Defining bitmap
            val bitmap = data?.extras?.get("data") as Bitmap
            //Setting it to visible
            imageView.setImageBitmap(bitmap)
            //To editing the picture
            picEditing()

        }else if (requestCode == REQUEST_VIDEO_CAPTURE && resultCode == RESULT_OK){
            Log.d(TAG, "We got the video")

            videoView.setVisibility(View.VISIBLE)

            val videoUri : Uri? = data?.data

            if(videoUri != null) {
                videoView.setVideoURI(videoUri)
                var file = File(getFilesDir().absoluteFile, videoUri.toString())
                Log.d(TAG,"FILE ON $file")
                videoView.start()
                val finalFile = File(videoUri.toString())

                val path = getRealPathFromURIVid(videoUri)
                Log.d(TAG, "VIDEOPATH ON")

                //Sending data trough machine learning algorithm
                val predictedClass = obj.processVideo(path)
                Log.d(TAG, "Predicted class is: $predictedClass")
                //This was meant to set the prediction to the text box, but
                //using the machine learning algorithm did not work straight
                //in the app.
                //val predictionText = findViewById<TextView>(R.id.Arvioteksti)
                //val previousText = predictionText.text
                //predictionText.text = "$previousText$predictedClass"
                //Saving the video
                saveInStorageVideo(videoUri, predictedClass)
            }else {
                Log.d(TAG,"videoUri oli tyhjä")
            }
        }else{
            super.onActivityResult(requestCode, resultCode, data)
            Toast.makeText(this, Activity.RESULT_OK.toString(), Toast.LENGTH_SHORT).show()
            Log.d(TAG, "Did not get anything back")
        }
    }


    private fun picEditing(){
        Log.d(TAG, "picEditing")

        val imageView: ImageView = findViewById(R.id.imageView)

        imageView.isDrawingCacheEnabled = true
        imageView.buildDrawingCache()
        val bitmap = (imageView.drawable as BitmapDrawable).bitmap

        //Bitmap to Mat
        val orig = Mat(bitmap.height, bitmap.width, CvType.CV_8UC3)
        val myBitmap32 = bitmap.copy(Bitmap.Config.ARGB_8888, true)
        Utils.bitmapToMat(myBitmap32, orig)

        //Changing size
        val resizeImage : Mat = orig
        val sz= Size(400.0, 256.0)
        Imgproc.resize(orig, resizeImage, sz,0.0, 0.0)
        //Getting the path from the device
        val tempUri = getImageUri(getApplicationContext(), bitmap)
        val finalFile = File(getRealPathFromURI(tempUri))

        val predictedClass = obj.processImage(finalFile.toString())
        Log.d(TAG, "Predicted class is: $predictedClass")
        //This was meant to set the prediction to the text box, but
        //using the machine learning algorithm did not work straight
        //in the app.
        //val predictionText = findViewById<TextView>(R.id.Arvioteksti)
        //val previousText = predictionText.text
        //predictionText.text = "$previousText$predictedClass"
        //Saving the picture
        saveInStorageImage(resizeImage,predictedClass)
    }


    private fun saveInStorageImage(data: Mat, predictedClass: String){
        Log.d(TAG, "saveInStorageImage")

        //Mat to Bitmap
        val myBitmap32 = Bitmap.createBitmap(data.cols(), data.rows(), Bitmap.Config.RGB_565)
        Utils.matToBitmap(data, myBitmap32)

        val rand = Random.nextInt()
        val name = "image$rand.jpg"
        val imageRef = storageRef.child("dataFromUsers/U$predictedClass/$name")

        Log.d(TAG, imageRef.toString())

        val uploadTask = imageRef.putBytes(myBitmap32.toByteArray())
        uploadTask.addOnFailureListener {
            Log.d(TAG, "Upload was not successful")
        }.addOnSuccessListener {
            Log.d(TAG, "Upload was successful")

            uploadTask.continueWithTask { task ->
                if (!task.isSuccessful) {
                    task.exception?.let {
                        Log.d(TAG, "Getting the url went badly")
                    }
                }
                imageRef.downloadUrl
            }.addOnCompleteListener { task ->
                if (task.isSuccessful) {
                    val downloadUri = task.result
                    Log.d(TAG, "Getting the url went well")
                    writeInFS(downloadUri.toString(), predictedClass)
                } else {
                    Log.d(TAG, "Did not go to the end")
                }
            }
        }
    }

    //Communication with Storage based on this: https://firebase.google.com/docs/storage/android/start
    private fun saveInStorageVideo(data: Uri?, predictedClass: String){
        Log.d(TAG, "Saving the video")

        //Pictures name in the database
        val rand = Random.nextInt()
        val name = "$rand.mp4"
        //Making a reference to the storage
        val imageRef = storageRef.child("dataFromUsers/U$predictedClass/video$name")


        Log.d(TAG, imageRef.toString())
        //Uploading the picture to storage
        val uploadTask = data?.let { imageRef.putFile(it) }
        uploadTask?.addOnFailureListener {
            Log.d(TAG, "Upload was not successful")
        }?.addOnSuccessListener {
            Log.d(TAG, "Upload was successful")
            //Getting the picture URL, to put to Firestore
            val urlTask = uploadTask.continueWithTask { task ->
                if (!task.isSuccessful) {
                    task.exception?.let {
                        Log.d(TAG, "Getting the url went badly")
                        throw it
                    }
                }
                imageRef.downloadUrl
            }.addOnCompleteListener { task ->
                if (task.isSuccessful) {
                    val downloadUri = task.result
                    Log.d(TAG, "Getting the url went well")
                    Log.d(TAG, downloadUri.toString())
                    writeInFS(downloadUri.toString(), predictedClass)
                } else {
                    Log.d(TAG, "Getting the url not complete")
                }
            }

        }

    }


    private fun writeInFS(url: String, predictedClass: String){
        val rand = Random.nextInt() + Random.nextInt()
        Log.d(TAG, rand.toString())

        val data = hashMapOf("URL" to url,
            "num" to rand.toString())


        //Uploading the URL to Firestore to appropriate collection
        db.collection("U$predictedClass-URL").add(data).addOnSuccessListener {
            Log.d(TAG, "Saatiin URL firestoreen")
        }

    }

    // Extension function to convert bitmap to byte array
    private fun Bitmap.toByteArray(): ByteArray {
        ByteArrayOutputStream().apply {
            compress(Bitmap.CompressFormat.JPEG, 10, this)
            return toByteArray()
        }
    }

    //The next functions are based on: https://stackoverflow.com/questions/20327213/getting-path-of-captured-image-in-android-using-camera-intent  User: Atai Ambus
    fun getImageUri(inContext: Context, inImage: Bitmap): Uri? {
        val rand = Random.nextInt()
        val bytes = ByteArrayOutputStream()
        inImage.compress(Bitmap.CompressFormat.JPEG, 100, bytes)
        val path = MediaStore.Images.Media.insertImage(inContext.contentResolver, inImage, "Title$rand", null)
        return Uri.parse(path)
    }


       fun getRealPathFromURI(uri: Uri?): String? {
        var path = ""
        if (contentResolver != null) {
            val cursor: Cursor? = contentResolver.query(uri!!, null, null, null, null)
            if (cursor != null) {
                cursor.moveToFirst()
                val idx: Int = cursor.getColumnIndex(MediaStore.Images.ImageColumns.DATA)
                path = cursor.getString(idx)
                cursor.close()
            }
        }
        return path
    }

    fun getRealPathFromURIVid(contentUri: Uri): String {
        val proj = arrayOf(MediaStore.Images.Media.DATA)
        val cursor = managedQuery(contentUri, proj, null, null, null)
        val column_index = cursor.getColumnIndexOrThrow(MediaStore.Images.Media.DATA)
        cursor.moveToFirst()
        return cursor.getString(column_index)
    }

}