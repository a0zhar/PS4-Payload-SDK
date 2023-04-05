# Changelog

- Update v0.5.5 (12/03/2023)
  - What's New:
    #### SocketPrintf
    <span style="color:blue">some *blue* text</span>
    - Here are some of the changes that have been made:
      - The buffer size calculation and allocation have been combined into a single step. By allocating the buffer dynamically only if it's too large to fit on the stack, we can avoid the overhead of dynamic allocation for most messages.
      - The buffer size is now adjusted to exclude the null terminator, so that it matches the actual size of the formatted message.
      - The socket connection is opened and closed each time a message is sent, to ensure that the socket is properly cleaned up.
      - The function now uses a stack-allocated buffer of size 1024 for small messages, and only allocates memory on the heap for larger messages. This can help reduce memory fragmentation and allocation overhead for frequent small messages.
      - The function now checks if the message buffer was successfully allocated before attempting to send the message, and deallocates the buffer if it was allocated on the heap.

- Update v0.5.4 (09/03/2023)
  - What's New:
    - **dump.c:** `decrypt_dir`:
      - Now has a MAX_PATH_LEN constant defined to store the maximum length of the path string, whereas the previous version hardcoded the value 1024.
      - Now uses snprintf instead of sprintf to avoid buffer overflows when creating the source and destination path strings.
      - Now has improved comments that explain the purpose of the code and provide more context.
      - Now uses continue to skip processing the current directory (".") and parent directory ("..") entries, which makes the code more readable and easier to follow.
      - Now has better error handling by returning if the source directory cannot be opened or the destination directory cannot be created.
      - Now a function signature that specifies const for the \_Source and \_Destination parameters, which indicates that these parameters are not modified by the function. The previous version did not have this const specifier.
        <br>
    - **dump.c** `waitForApplication`:
      - Changed function name from wait_for_app to waitForApplication.
      - Changed input parameter from char _title_id to const char_ titleID to make it a read-only input parameter.
      - Changed variable name from dir to pfsDir to make it more descriptive of its purpose
      - Removed redundant else block after continue.
      - Renamed res variable to retVal for consistency with the name of the function's return value.
      - Added comments to clarify the purpose of each code block.
        <br>
- Update v0.5.3 (06/03/2023)
  - What's New:
    - Renamed the parameters of function pointers defined inside libc.c and libc.h to match the C standard.
    - Added macros representing the Syscalls, making it easier to read the syscall name instead of its numeric value.
    - Enhanced sendNotification to accept arguments and formatted strings, such as `sendNotification("hello %s 1 + 1 is %d", "world", 2).`
    - Refactored `memset_s` as a function pointer, rather than using the locally created function.
    - Added notifications.h, which contains dumped notification icons and notification types defined as macros, thanks to OSM.
    - **linker.x**:
      - The code is formatted consistently, with each section and phdr block indented and surrounded by braces.
      - The phdr names are separated from the PT_LOAD type with whitespace for improved readability.
      - The /DISCARD/ section is indented consistently with the other section definitions.
      - The semicolons after each section and phdr definition are removed, as they are not necessary.
    - Removed **mutex.h** as it didnt contain any code
    - **network.c:**
      - SckRecv changes
        - The initial allocation of retval to sizeof(char) \* 1 is unnecessary and wasteful, so it has been removed.
        - A do-while loop is used instead of a while loop to simplify the logic and reduce code duplication.
        - The loop condition now checks the value of plen at the end of each iteration rather than the beginning.
        - The memset call is unnecessary because the rbuf array is fully overwritten in each iteration of the loop.
        - The if-else block for reallocating the memory has been simplified using a single assignment statement.
        - The return value of sceNetRecv is checked to handle the case where it returns a negative value, indicating an error. In this case, the function frees the memory allocated so far and returns NULL.
        - memcpy is used instead of a loop to copy the contents of rbuf into retval.
  - Upcoming:
    - Feature which will automatically unload loaded modules when no longer needed to clean up better.
    - Improving the overall organization and cleanliness of the codebase by merging files with similar content and using more informative filenames.

Update v0.5.2 (Tuesday, Jan 31, 2023)

- **Whats new?**

  - "replace_string" has been renamed to "replaceString"
  - "split_string" has been renamed to "splitString"
  - countInstances, has been added! returns the number of instances of a word found in string

- **Improvements:**

  - **compareFiles:**
    - Changed both parameters from "char*" to "const char*" since the function dont modify them anyways
  - **replaceString:**

    - Now replaces all instances of the word, instead of first instance.
    - Now uses dynamic allocation to handle strings of any size.
    - Now more readable, easier to understand, and more maintainable.

  - **splitString:**
    - Now version is more concise and efficient, using a single loop to count the number of tokens and allocate memory for the output array.
    - New version splits the string in a single pass, improving performance.
    - New version is more readable, easier to understand, and more maintainable.

Update v0.5.1 (Sunday, Jan 29, 2023)

- Whats new?
  - Renamed symlink_exists to symlinkExist
  - Renamed touch_file to touchFile
  - Changed the "char*" parameter in symlinkExist, and touchFile to "const char*" because the function doesn't modify the variable anyways.
  - Refactored the getFileSize function and changed it to be off_t instead of int
  - Removed the debugging_log function as it is unnecessary.
  - Added the vsnprintf function.

Update v0.5 (Saturday, Jan 28, 2023)

- Whats new?
  - The S_IS\* macros:
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
    - the function signature was changed to take a const char* instead of a char*. This makes sure the function does not modify the input string, and makes it more robust.

Update v0.4 (Monday, January 16, 2023)

- Whats new?

  - Cleanup functions aid in cleaning up after the payload has done its thing; this includes, for example, unloading modules, closing socket connections, and more. So far these files have cleanup function implemented: `libc.c, kernel.c, sysutil.c, network.c, camera.c, pthread.c`
    <br>
  - A new macro has been introduced that allows for sending messages between PS4 and PC using Socket. This macro is more efficient and less wasteful with memory thanks to dynamic allocation. Include the following at the beginning of the file you want to use the macro in:

    ```C
    #define IS_DEBUGGING     // define this if u want to use the macro
    #define SERVER_IP   ""   // The IP of the server (receiving messages)
    #define SERVER_PORT 0000 // The port of that server
    #include "sockcomm.h"    // contains the macro
    // REST OF YOUR CODE BELOW
    ```

    <br>

  - New macros that represent the file type of a directory entry have been added! The purpose of these is to replace the S_IS\* macros. The following are new: `_DT_BLK, _DT_CHR, _DT_DIR, _DT_FIFO, _DT_LNK, _DT_REG, _DT_SOCK`
    <br>

Update v0.3 (Friday, January 13, 2023)

- Code Improvements:

  - In the getUserIDList function, I moved the initialization of the user service to the beginning of the function, and added a return value to check if the function succeeded or failed.
  - In the getUserID function, I added a check to see if the getUserIDList function failed, and return -1 if it did.
  - I also changed the loop to iterate over all users in the userId array, not just the first one.
  - I changed the `const char*` parameter in both \_DebugLog and getFileSize function, to `char*`.
  - I Refactored the Base64_encode/base64_decode functions.
  - To Improve readability of the code, i renamed `SckRecvf` to `SckRecvDataToFile`. And both of them has been refactored.
    <br>

    - **In the `SckRecvDataToFile` function, the variable `destfile` is now named `filepath`, and the variable `fid` is now named `file_descriptor`. The variable `plen` is now named `bytes_received` which makes it more clear what the variable represents.**
      <br>

    - **In the `SckRecv` function, the variable data is now named more descriptively as data and the `rbuf` is now named `buffer`. The variable `plen` is now named `bytes_received` which makes it more clear what the variable represents. The variable `retval` is now named `data` which is more descriptive and more meaningful.**

- New stuff!
  - I added the `cleanupNetwork()` function, to unload the network modules used by the payload. **It is important to note that, unloading a module that is being used by the payload can cause undefined behavior and crashes. So make sure all the resources allocated by the modules are freed properly before unloading them.**
  -
- Upcoming in next Update!
  - Simple Socket Communication between your PC and PS4. As well as a socket server that runs on your PC to receive messages being sent from the PS4.
  - A PS4-compatible recursive directory iterator implementation.
  - Firmware spoofing, similar to GoldHEN, with ASCII-letter support
  - Timestamp string, which would be returned as `[hh:mm:ss]`

---

Update v0.1 (Sunday, January 8, 2023)

- Whats new?
  - A new function has been added! \_DebugLog(): logs information into the file.
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
