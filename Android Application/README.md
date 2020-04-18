# Android Application

This is the Android application code to control servomotors with an Android device via Bluetooth. 

## Getting Started

Download Android-nRF-UART-master zip file and extract it at a known place. This folder will be the root the poject on Android Studio.

### Prerequisites

#### Computer
* 2 Go of RAM
* 1,5 Go free memory

##### For Windows
Vista or newer

##### For Mac OS
10.8.5 or newer

##### For Linux
Ubuntu 14.04 or newer

GNU C librairy 2.15 or newer

64 bits distribution should be able to run 32 bits applications

#### JDK
https://www.oracle.com/java/technologies/javase-downloads.html

Click on JDK Download

#### Android studio and SDK Android
https://developer.android.com/sdk/installing/studio.html

Click on DOWNLOAD ANDROID STUDIO and execute the file when ready

If you don't have an Android device you have to install Android Virtual Device when asking. Either way you don't need any of other components than Android Studio 

After the download run Android studio and choose the stadard installation

After the installation click on configation in the right buttom and select SDK Manager

Rigth below Android SDK Location select SDK Tools and make sure that Android SDK Build-Tools XX is checked

#### Android device
Go to your settings and scroll to About Phone

Click seven (7) times on Build number

Back --> System --> Developper options 

Authorize USB debugging

## Deployment

Connect your Android device on your computer

Open Android Studio 

And open Android-nRF-UART-master project

Wait until Android Studio finish its build

MAJ+F10

The application is ready to be use. If you read TRASFERED_README in th Software folder and started the Main.py program on Raspberry Pi, you should see Robot Arm as connexion option after had clicked on the connect button of the application.

## License

Licensed under the Apache License, Version 2.0


