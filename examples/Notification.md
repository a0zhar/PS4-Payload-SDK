# How to send a simple PS4 notification using the library
## Before doing anything, you need to run update.sh before you can use the library. The update.sh file will build the library and then set the required global path variable. 

```C
#include "ps4.h"   // Include the ps4.h header, which includes all other relevant headers.
#include "icons.h" // Include names for dumped notification icons (ref: https://github.com/OSM-Made/PS4-Notify)

int PrepareEnvironment() {
  initKernel();
  initLibc();
  jailbreak();
  initSysUtil();
  return 0;
}

int _main(struct thread* td) {
  UNUSED(td);
  PrepareEnvironment();
 
  sendNotification(texDefaultIconTrophy, "Hello World");
  
  return 0;
}
```

### icons.h header
```C
#pragma once


#define texIconSystem "cxml://psnotification/tex_icon_system"                            //Playstation buttons
#define texIconBan "cxml://psnotification/tex_icon_ban"                                  //Circle with a slash
#define texDefaultIconNotification "cxml://psnotification/tex_default_icon_notification" //i in a chat bubble
#define texDefaultIconMessage "cxml://psnotification/tex_default_icon_message"
#define texDefaultIconFriend "cxml://psnotification/tex_default_icon_friend"
#define texDefaultIconTrophy "cxml://psnotification/tex_default_icon_trophy"
#define texDefaultIconDownload "cxml://psnotification/tex_default_icon_download"
#define texDefaultIconUpload16_9 "cxml://psnotification/tex_default_icon_upload_16_9"
#define texDefaultIconCloudClient "cxml://psnotification/tex_default_icon_cloud_client"   //ps now logo
#define texDefaultIconActivity "cxml://psnotification/tex_default_icon_activity"
#define texDefaultIconSmaps "cxml://psnotification/tex_default_icon_smaps"                //bullhorn
#define texDefaultIconShareplay "cxml://psnotification/tex_default_icon_shareplay"
#define texDefaultIconTips "cxml://psnotification/tex_default_icon_tips"
#define texDefaultIconEvents "cxml://psnotification/tex_default_icon_events"
#define texDefaultIconShareScreen "cxml://psnotification/tex_default_icon_share_screen"
#define texDefaultIconCommunity "cxml://psnotification/tex_default_icon_community"
#define texDefaultIconLfps "cxml://psnotification/tex_default_icon_lfps"
#define texDefaultIconTournament "cxml://psnotification/tex_default_icon_tournament"
#define texDefaultIconTeam "cxml://psnotification/tex_default_icon_team"
#define texDefaultAvatar "cxml://psnotification/tex_default_avatar"
#define texIconCapture "cxml://psnotification/tex_icon_capture"
#define texIconStartRec "cxml://psnotification/tex_icon_start_rec"
#define texIconStopRec "cxml://psnotification/tex_icon_stop_rec"
#define texIconLiveProhibited "cxml://psnotification/tex_icon_live_prohibited"
#define texIconLiveStart "cxml://psnotification/tex_icon_live_start"
#define texIconLoading "cxml://psnotification/tex_icon_loading"
#define texIconLoading16_9 "cxml://psnotification/tex_icon_loading_16_9"
#define texIconCountdown "cxml://psnotification/tex_icon_countdown"
#define texIconParty "cxml://psnotification/tex_icon_party"
#define texIconShareplay "cxml://psnotification/tex_icon_shareplay"
#define texIconBroadcast "cxml://psnotification/tex_icon_broadcast"
#define tex_icon_psnow_toast "cxml://psnotification/tex_icon_psnow_toast"
#define tex_audio_device_headphone "cxml://psnotification/tex_audio_device_headphone"
#define tex_audio_device_headset "cxml://psnotification/tex_audio_device_headset"
#define tex_audio_device_mic "cxml://psnotification/tex_audio_device_mic"
#define tex_audio_device_morpheus "cxml://psnotification/tex_audio_device_morpheus"
#define tex_device_battery_0 "cxml://psnotification/tex_device_battery_0"
#define tex_device_battery_1 "cxml://psnotification/tex_device_battery_1"
#define tex_device_battery_2 "cxml://psnotification/tex_device_battery_2"
#define tex_device_battery_3 "cxml://psnotification/tex_device_battery_3"
#define tex_device_battery_nocharge "cxml://psnotification/tex_device_battery_nocharge"
#define tex_device_comp_app "cxml://psnotification/tex_device_comp_app"
#define tex_device_controller "cxml://psnotification/tex_device_controller"
#define tex_device_jedi_usb "cxml://psnotification/tex_device_jedi_usb"
#define tex_device_blaster "cxml://psnotification/tex_device_blaster"
#define tex_device_headphone "cxml://psnotification/tex_device_headphone"
#define tex_device_headset "cxml://psnotification/tex_device_headset"
#define tex_device_keyboard "cxml://psnotification/tex_device_keyboard"
#define tex_device_mic "cxml://psnotification/tex_device_mic"
#define tex_device_morpheus "cxml://psnotification/tex_device_morpheus"
#define tex_device_mouse "cxml://psnotification/tex_device_mouse"
#define tex_device_move "cxml://psnotification/tex_device_move"               //ps move controller
#define tex_device_remote "cxml://psnotification/tex_device_remote"
#define tex_device_omit "cxml://psnotification/tex_device_omit"
#define tex_icon_connect "cxml://psnotification/tex_icon_connect"              //Weird blury thing
#define tex_icon_event_toast "cxml://psnotification/tex_icon_event_toast"          //Calendar
#define tex_morpheus_trophy_bronze "cxml://psnotification/tex_morpheus_trophy_bronze"
#define tex_morpheus_trophy_silver "cxml://psnotification/tex_morpheus_trophy_silver"
#define tex_morpheus_trophy_gold "cxml://psnotification/tex_morpheus_trophy_gold"
#define tex_morpheus_trophy_platinum "cxml://psnotification/tex_morpheus_trophy_platinum"
#define tex_icon_champions_league "cxml://psnotification/tex_icon_champions_league"     //The OG foot ball lol
```
