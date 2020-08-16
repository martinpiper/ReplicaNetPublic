Example Android NDK

This example requires a machine with internet access.

This demonstration shows:
How to use the ReplicaNet SDK with the Android NDK (Using Windows as the host OS)
Use the AdvertiseClient

This uses Microsoft Visual Studio 2017 with Android NDK extensions installed.

RNGLue.cpp/h	Contains ReplicaNet glue code
Font.cpp/h		Contains simple bitmap font drawing code
main.cpp		Has been tweaked from the original native activity NDK example to call into RNGlue functions and render bitmap font information


To start the emulator:
start android avd

adb devices
adb install -r bin\ReplicaNetTest1-debug.apk


To get debug:
adb logcat -s replicanet
