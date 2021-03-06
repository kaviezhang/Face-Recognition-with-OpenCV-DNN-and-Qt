# Face-Recognition-with-OpenCV-DNN-and-Qt
Face Recognition on Windows and Ubuntu Raspberry Pi platform. The process of recognition can be completed within 1 second on  a Ubuntu-cored FriendlyDesktop NanoPi device. And this project also support WiringPi control to access hardware such as GPIO/I2C/SPI/UART/PWM etc by editing wire.cpp and wire.h.

## Requirements
> Qt5  
> Ubuntu OpenCV  
> Windows minGW_build_OpenCV  

## UI Display
<center>
<img src="https://raw.githubusercontent.com/kaviezhang/Face-Recognition-with-OpenCV-DNN-and-Qt/master/README_md_files/Widget%205_29_2020%2011_09_50%20AM.png" width=50% height=50%/>
<center>Main Widget</center>

<img src="https://raw.githubusercontent.com/kaviezhang/Face-Recognition-with-OpenCV-DNN-and-Qt/master/README_md_files/Widget%205_29_2020%2011_19_31%20AM.png" width=50% height=50%/>
<center>Edit Form</center>

<img src="https://raw.githubusercontent.com/kaviezhang/Face-Recognition-with-OpenCV-DNN-and-Qt/master/README_md_files/Widget%205_29_2020%2011_10_17%20AM.png" width=50% height=50%/>
<center>Choose Form</center>

<img src="https://raw.githubusercontent.com/kaviezhang/Face-Recognition-with-OpenCV-DNN-and-Qt/master/README_md_files/Widget%205_29_2020%2011_10_31%20AM.png" width=50% height=50%/>
<center>Registion Form</center>

<img src="https://raw.githubusercontent.com/kaviezhang/Face-Recognition-with-OpenCV-DNN-and-Qt/master/README_md_files/Widget%205_29_2020%2012_45_42%20PM.png" width=50% height=50%/>
<center>Result Form</center>

</center>


## Reference
We load this [VGGFace2 ResNet-50-256D Pretrained model ](https://github.com/ox-vgg/vgg_face2) in OpenCV DNN module for face recognition. Download and Copy the **resnet50_256.caffemodel** file to the **.\\weights\\** folder.

[1]VGGFace2: A dataset for recognising faces across pose and age,  Q. Cao, L. Shen, W. Xie, O. M. Parkhi, A. Zisserman, In FG 2018. 



