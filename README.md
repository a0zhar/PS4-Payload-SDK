# ps4-payload-sdk 
**An open source SDK for writing payloads for the PlayStation 4.
<br> Supports firmware 3.50 - 9.00.**
<br><br>


# CHANGELOG 

Update v0.3  (Friday, January 13, 2023)
- Code Improvements: 
  - In the getUserIDList function, I moved the initialization of the user service to the beginning of the function, and added a return value to check if the function succeeded or failed.
  - In the getUserID function, I added a check to see if the getUserIDList function failed, and return -1 if it did.
  - I also changed the loop to iterate over all users in the userId array, not just the first one.
  - I changed the `const char*` parameter in both _DebugLog and getFileSize function, to `char*`.

- Upcoming in next Update!
  - Simple Socket Communication between your PC and PS4. As well as a socket server that runs on your PC to receive messages being sent from the PS4.
  - A PS4-compatible recursive directory iterator implementation.
  - Firmware spoofing, similar to GoldHEN, with ASCII-letter support
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
