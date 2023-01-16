# ps4-payload-sdk 
[](logo.png)
# CHANGELOG

Update v0.4 (Monday, January 16, 2023)
- Whats new?
  - Cleanup functions aid in cleaning up after the payload has done its thing; this includes, for example, unloading modules, closing socket connections, and more. So far these files have cleanup function implemented: `libc.c, kernel.c, sysutil.c, network.c, camera.c, pthread.c`
  <br>
  
  - A new macro `EasySckPrint` has been introduced that allows for sending messages from the PS4 to your PC. This macro is more efficient and less wasteful with memory thanks to dynamic allocation. Include the following at the beginning of the file where the macro is being used:
     ```C++
     #define IS_DEBUGGING     // define this if u want to use the macro
     #define SERVER_IP   ""   // The IP of the server (receiving messages)
     #define SERVER_PORT 0000 // The port of that server
     #include "sockcomm.h"    // contains the macro
     ```
     The macro can be used just like you would use "printf," for example:
     ```C++
     int _main(void) {
       ...
       initNetwork();
       char myName[] = { "Name" };
       EasySckPrint("my name is %s", myName); // sends "my name is Name" to pc.
       ...
       cleanupNet();
       return 0;
     }
     ```
  <br>
 
  - New macros that represent the file type of a directory entry have been added! `_DT_BLK, _DT_CHR, _DT_DIR, _DT_FIFO, _DT_LNK, _DT_REG, _DT_SOCK`
---
Update v0.3  (Friday, January 13, 2023)
- Code Improvements: 
  - In the getUserIDList function, I moved the initialization of the user service to the beginning of the function, and added a return value to check if the function succeeded or failed.
  - In the getUserID function, I added a check to see if the getUserIDList function failed, and return -1 if it did.
  - I also changed the loop to iterate over all users in the userId array, not just the first one.
  - I changed the `const char*` parameter in both _DebugLog and getFileSize function, to `char*`.
  - I Refactored the Base64_encode/base64_decode functions. 

- Upcoming in next Update!
  - Simple Socket Communication between your PC and PS4. As well as a socket server that runs on your PC to receive messages being sent from the PS4.
  - A PS4-compatible recursive directory iterator implementation.
  - Firmware spoofing, similar to GoldHEN, with ASCII-letter support
  - Timestamp string, which would be returned as `[hh:mm:ss]`
---
Update v0.1 (Sunday, January 8, 2023)
- Whats new?
  - A new function has been added! _DebugLog(): logs information into the file.
  - Renamed copy_dir and file_compare to copyDirectory and compareFiles.
  - Added more info to O_RDWR, O_WRONLY, O_RDONLY, and O_ACCMODE.
  - SupremePerms and newLinee have been added as new variables.
  - I rewrote the compareFiles() function.
---
Update v0.0 (Friday, January 6, 2023)
- Whats new? 
  - Added a new function, getFileSize(), which returns the size of a given file.
  - Rewrote the function file_compare();
  - Refactored the getUserID() function.
---
Credits:
https://github.com/Scene-Collective/ps4-payload-sdk
