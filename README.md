---
<p align="center">
<img src="logo.png">
<br>
Eyy, thanks for checking out my repo. Have a good day :)
</p>

   

# CHANGELOG

Update v0.5.1 (Sunday, Jan 29, 2023)
- Whats new?
  - Renamed symlink_exists to symlinkExist
  - Renamed touch_file to touchFile
  - Changed the "char*" parameter in symlinkExist, and touchFile to "const char*" because the function doesn't modify the variable anyways.
  - Refactored the getFileSize function and changed it to be off_t instead of int
  - Removed the debugging_log function as it is unnecessary.
  - Added the vsnprintf function.
---
Update v0.5 (Saturday, Jan 28, 2023) 
- Whats new?
  - The S_IS* macros:
    - Converted the "stat" macros from preprocessor directives to C functions
    - Added a mode_t m argument to the functions to take the file mode as input
    - Changed the return type of the functions from #define to int for better type safety
    - Updated the function names to be more descriptive and human-readable for improved readability and understanding
    - Improved maintainability and portability of the code.
    <br>
  - Added new macros for file opening options:
    - O_ASYNC: **sends a signal when data is ready**
    - O_DIRECT: **enables direct disk access**
    - O_NOFOLLOW: **does not follow links**
    - O_SYNC: **enables synchronous writes**
    - O_DSYNC: **enables synchronous writes of data only**
    - O_RSYNC: **enables synchronous reads**
    - O_NOATIME: **does not update the access time**
    - O_ACCMODE: **get the access mode**
    - O_DIRECTORY: **fails if the file is not a directory**
    - O_NOCTTY: **does not assign controlling terminal**
    - O_CLOEXEC: **set close-on-exec**
  <br>
- Improvements
  - **getFileSize has been optimized/improved**
    - the fstat() function is used to get the file size, which eliminates the need for an additional lseek() call. This can improve performance, as fstat() is typically faster than lseek().
    - The off_t type is used for the file size variable, which is specifically designed to hold file sizes, thus it can handle large files.
    - Error handling is added to check the return value of the open() function, so the function can return an error code if the file cannot be opened. 
    - The O_CLOEXEC flag is used when calling open() to automatically close the file descriptor when a new process is spawned. This can prevent file descriptor leaks and improve security.
    -  the function signature was changed to take a const char* instead of a char*. This makes sure the function does not modify the input string, and makes it more robust.
--- 
Update v0.4.1 (Jan 24, 2023)
- Whats new? 
  - The sendNotification function has been updated to include variable arguments, added a check for null icon and format, uses snprintf to calculate buffer size and write formatted message to buffer, uses strcpy to copy icon and message to SceNotificationRequest struct, and frees allocated memory after sending the notification request.
---
Update v0.4.0 (Jan 16, 2023)
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
