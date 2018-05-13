# Arduino + Ionic 3 + Pierburg 2E2 throttle.

You can control couple throttle valve in carburator type Pierburg 2E2 

## Structure

  1. Folder `carb_prototupe` contains sketch from arduino platform
  2. Folder `CarburatorAPP` consists Ionic 3 project.

## Rin Ionic 3 project
 
 Instal enviroment like sead in [offical tutorial](https://ionicframework.com/docs/intro/deploying/);

  1. Install ionic 3
  2. Install Android Studio.
  3. Include SDK platforms version 26 (Android 8)
  4. Install [Java version 8(only)](http://www.oracle.com/technetwork/java/javase/downloads/jdk8-downloads-2133151.html)
  5. Install path variable. === !!! find path  `WIN + PAUSE/BRAKE` !!! ===
  
      - ANDROID_HOME - C:\Users\(user_name)\AppData\Local\Android\sdk	
      - JAVA_HOME - c:\Program Files\Java\jdk1.8.0_152\	
    
  6. Insatl Path  === !!! find path  `WIN + PAUSE/BRAKE` !!! ===
  
      - c:\Program Files\Java\jdk1.8.0_152\bin\		
      - c:\Users\(user_name)\AppData\Local\Android\Sdk\tools\
      - c:\Users\(user_name)\AppData\Local\Android\Sdk\platform-tools\
    
  7. Run in terminal command `ionic cordova run android -l`


### Fix some errors


1. If can't run in android 4.2.2 (you see blank screen). 

    - Run comand in teminal: `cordova plugin add cordova-plugin-crosswalk-webview`

2. Run in Redmi 4. Run comand in teminal:

    1. `ionic cordova build android`	
    2. `ionic cordova run android -l`

3. adb server version (32) doesn't match this client (39); killing... could not read ok from ADB Server * failed to start daemon
   error: cannot connect to daemon

    - Your system have couple adb.exe. Need find it (from TotalCommander - disc C), and just rewrite oldest version newest

4. adb server is out of date.  killing... ADB server didn't ACK   * failed to start daemon *   error: unknown host service

    - Run command where locate SDK `c:androidsdkplatform-tools>netstat -aon|findstr 5037`
    - now you can run you devices; [details](http://dennylabs.com/adb-server-didnt-ack-failed-to-start-deamon/)

5. "SDK Platform" for API level android-26

    - go to cd c:\Users\Super User\AppData\Local\Android\Sdk\tools\bin\
    - run command - `sdkmanager "build-tools;26.0.0"`

