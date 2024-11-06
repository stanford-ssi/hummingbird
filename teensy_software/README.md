1. main.cpp in /src. For more information about how the folders are structured, check the README files in /include and /lib
2. To see what board this folder of code supports, see platformio.init. For this board it's Teensy 4.1 with the Arduino framework.
3. The following commands can be used for compiling and uploading code. Make sure you open a New Terminal from the PlatformIO menu, not the terminal from VScode. Download instructions: https://platformio.org/install/ide?install=vscode
    - pio run: compile code
    - pio run --target uplaod: upload code (when board connected)
    - for VScode users, you can also just use the icons on the bottom left of the app screen to run code. 
4. Make sure you have all the libraries needed for the BME680 to run if you are testing that. See main.cpp for more.
5. If you're running this on the Arduino IDE, get the necessary libraries directly from the library manager in Arduino IDE.
