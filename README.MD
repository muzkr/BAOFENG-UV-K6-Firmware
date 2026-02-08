# SOURCE CODE FOR BAOFENG UV-K6x Radios-CNT7 UPLOAD
## Introduction

This repository contains the firmware source code for the Baofeng UV-K6x series of radios. The code is written in C and is intended to be compiled with the GCC compiler. The code is released under the BFPL-1.0 license.

## Building

Build the code with CMake and a recent version of gcc.

## Uploading

To upload the code to the radio, you will need to use a programming cable. You can find programming cables for the Baofeng UV-K6x series of radios on eBay or Amazon. Once you have the programming cable, you can upload the code by running the BFK6_Bootloader.exe program on a Windows computer.

To upload the code to the radio, follow these steps:
1. Power off your UV-K6 radio.
2. Connect the programming cable to the radio and the computer.
3. Press and hold 2 ORANGE side buttons on the radio, then turn on the radio, you will see the screen shows "UPDATE" and the radio will be in the update mode.
4. Run the BFK6_Bootloader.exe program on your computer.
5. Click the "..." button and select the output binary file from the ./out/ folder.
6. Select the COM port that the programming cable is connected to.
6. Click the large "按两个侧键开机进入升级(Power on with side buttons pressed to Enter Upgrade)" button to upload the code to the radio.

## License

The code is released under the BFPL-1.0 license. for more information, please see the LICENSE file.

## Copyright
Copyright © 2024 Baofeng. All rights reserved.

## Contributors
- BD4VOW ;  Starry Night Quintet- CNT7 DarkSoma OHMYTIME  X 
    - provided gcc, openocd and PlatformIO support.

The following repositories are highly valuable for BF K6 development:

- https://github.com/amoxu/platform-openradiox provides SVD definitions for KD32
- https://github.com/cnt7/tool-openocd-kd32.git offers an OpenOCD distribution that works well with KD32
