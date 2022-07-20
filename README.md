# TextDetector

Visual Studio 2015, OpenCV yazı tespit ve tesseract ile yazıları okuma programı.

Kurulması gerekenler: 
  -Visual Studio 2015
      https://visualstudio.microsoft.com/tr/vs/older-downloads/
  -CMake 
      https://cmake.org/download/
   -OpenCV 2.4
      https://github.com/opencv/opencv
 
 
 ## OpenCV kurulum 
 
  ![image](https://user-images.githubusercontent.com/68062151/180008190-82050b52-fd02-47e3-86d6-15b178ee2291.png) 

  1) OpenCV 2.4'ü [buradan](https://github.com/opencv/opencv) indirebilirsiniz. 
  2) İndirdiğiniz .zip uzantılı dosyayı bir klasörün içerisine çıkartın.
  3) Aynı klasörün içinde "build" isimli yeni bir klasör oluşturun.
  4) CMake GUI'yi açın.
  5) Yukarıda ki resimde 1 ile gösterilen butondan indirdiğiniz OpenCV klasörünü seçin
  6) Resimde 2 ile gösterilen butonda build klasörünü seçin  
  7) Daha sonra 3 ile gösterilen butona basın.<br/>
   ![image](https://user-images.githubusercontent.com/68062151/180011087-7975cf0e-580f-465d-8427-b323d19f0961.png)<br/>
  8) Açılan pencereden Visual studio versiyonunuzu ve "Optional platform for genarator" seçeneğinde işletim sisteminize uygun seçimi yapın "Finish"'e basın. 
  9) Yükleme bittikten sonra 4 ile gösterilen "Generate" butonuna basın.
  10) Daha sonra build klasörünün içinde oluşan OpenCV.sln dosyasını açın.
  11) Dosya açıldıktan sonra şağıda görülen dosyları önce ALL_BUILD daha sonra INSTALL dosyalarına sağ tıklayarak "Optional platform for genarator" da seçtiğim seçeneğe uygun Debug yapın
  ![image](https://user-images.githubusercontent.com/68062151/180009338-5af20b91-5395-4edc-82ce-9e75013507bb.png)

Yukarıdaki işlemler bittikten sonra Visual Studio yu kapatın. 

Windows ortam değişkenlerine gidip bende aşağıdaki konumlarda olan dosyların pathlerini ekleyin
```
C:\Users\90553\Desktop\OpenCV\myBuild\install\x64\vc14\lib
C:\Users\90553\Desktop\OpenCV\myBuild\install\x64\vc14\bin
```
Daha sonra visual studio da yeni bir proje açın ve projeyi sağ tıklarayak properties i açın.
"C/C++ -> General -> Additional Inlude Directories" buradaki değere build dosayasındaki "install\include\" klasörünün pathini yapıştırın. 

"Linker -> General -> Additional library Directories" seçeneğine build içinde install\x64\vc14\lib pathini ekleyin.

"Linker -> Input -> Additional Dependencies" seçeneğine aşağıdakileri ekleyin.
```
opencv_core2413d.lib
opencv_highgui2413d.lib
opencv_imgproc2413d.lib
opencv_video2413d.lib
```
"Linker -> Advanced -> Entry Point" seçeneğine "Main" yazın

Daha sonra windows ortam değişkenlerine girip aşağıdaki pathleri kendi dosya konumlarınıza göre ayarlayıpekleyin
```
C:\Users\90553\Desktop\OpenCV\myBuild\install\x64\vc14\lib
C:\Users\90553\Desktop\OpenCV\myBuild\install\x64\vc14\bin
```
Daha sonra openCV kütüphanesini test etmek için aşağıdaki kodu deneyebilirsiniz.
```
#include <opencv2/opencv.hpp>
#include <opencv\highgui.h>

using namespace cv;

int main() {
    Mat image;          //Create Matrix to store image
    VideoCapture cap;          //initialize capture
    cap.open(0);
    namedWindow("window", 1);          //create window to show image
    while (1) {
        cap >> image;          //copy webcam stream to image
        imshow("window", image);          //print image to screen
        waitKey(33);          //delay 33ms
    }
    return 0;
}
```



