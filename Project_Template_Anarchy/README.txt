Project Template

A great place to start developing your own projects from.

The project is a basic template, complete with plugin; demo component,
demo action. It will also run in vForge’s “play the game” mode.


This project can be copied to any location outside of the Vision SDK folder. 

To get this project up and running:

The project contains templates for console actions and a project plugin
component example.

------------------------------------
Instructions for Windows and Android
------------------------------------

(1) Copy to folder to your new location.


(2) Run the included batch file RUN_ONCE.bat to copy the SDK data and source files to your project.


(3) Open the visual studio *.sln file located in the Source folder. If some files or projects cannot be found, check if 
- the environment variable VISION_SDK is set to your Vision root directory
- the correct Android SDK and NDK is installed
- the environment variable NDKROOT is set to your Android NDK root


(4) Build and Run 


--------------------
Instructions for iOS
--------------------

(1) Copy to folder to your new location.


(2) Open the terminal, go to your copied project template folder and execute RUN_ONCE_IOS.sh file (e.g. /.RUN_ONCE_IOS.sh). The script will copy SDK shared data and source files to your project template folder.


(3) Open the ProjectTemplate_ios.xcodeproj file located in the Source/GameApplication folder.


(4) Build and Run 

NOTE: The project template includes a game plugin, which can be loaded directly from vForge. However, vForge will always load the 'Dev' DLLs, so to get the plugin running in vForge, you need to build it in the 'Dev' configuration.
You can still use the 'Debug' and 'Release' configurations for the standalone application, of course, but be aware that changes to the plugin will only show up in vForge, when you build the DLL with the 'Dev' configuration.



