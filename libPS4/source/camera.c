#include "../include/kernel.h"
#include "../include/module.h"
#include "../include/camera.h"

int libCamera;

int (*sceCameraOpen)(int userid, int type, int index, void*);
int (*sceCameraClose)(int handle);
int (*sceCameraIsAttached)(int index);
int (*sceCameraGetFrameData)(int handle, SceCameraFrameData* frame);
int (*sceCameraStart)(int handle, SceCameraStartParameter* param);
int (*sceCameraStop)(int handle);
int (*sceCameraGetDeviceInfo)(int handle, SceCameraDeviceInfo* info);
int (*sceCameraGetDeviceConfig)(int handle, SceCameraConfig* config);
int (*sceCameraGetConfig)(int handle, SceCameraConfig* config);
int (*sceCameraSetConfig)(int handle, SceCameraConfig* config);

void initCamera(void) {
    if (libCamera) {
        return;
    }

    libCamera = sceKernelLoadStartModule("libSceCamera.sprx", 0, 0, 0, NULL, NULL);

    getFunctionByName(libCamera, "sceCameraOpen", &sceCameraOpen);
    getFunctionByName(libCamera, "sceCameraClose", &sceCameraClose);
    getFunctionByName(libCamera, "sceCameraIsAttached", &sceCameraIsAttached);
    getFunctionByName(libCamera, "sceCameraGetFrameData", &sceCameraGetFrameData);
    getFunctionByName(libCamera, "sceCameraStart", &sceCameraStart);
    getFunctionByName(libCamera, "sceCameraStop", &sceCameraStop);
    getFunctionByName(libCamera, "sceCameraGetDeviceInfo", &sceCameraGetDeviceInfo);
    getFunctionByName(libCamera, "sceCameraGetDeviceConfig", &sceCameraGetDeviceConfig);
    getFunctionByName(libCamera, "sceCameraGetConfig", &sceCameraGetConfig);
    getFunctionByName(libCamera, "sceCameraSetConfig", &sceCameraSetConfig);
}
void unloadCameraMod() {
    unloadModule(libCamera);
}
