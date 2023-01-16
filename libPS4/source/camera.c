#include "kernel.h"
#include "module.h"

#include "camera.h"

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

  getFunctionAddressByName(libCamera, "sceCameraOpen", &sceCameraOpen);
  getFunctionAddressByName(libCamera, "sceCameraClose", &sceCameraClose);
  getFunctionAddressByName(libCamera, "sceCameraIsAttached", &sceCameraIsAttached);
  getFunctionAddressByName(libCamera, "sceCameraGetFrameData", &sceCameraGetFrameData);
  getFunctionAddressByName(libCamera, "sceCameraStart", &sceCameraStart);
  getFunctionAddressByName(libCamera, "sceCameraStop", &sceCameraStop);
  getFunctionAddressByName(libCamera, "sceCameraGetDeviceInfo", &sceCameraGetDeviceInfo);
  getFunctionAddressByName(libCamera, "sceCameraGetDeviceConfig", &sceCameraGetDeviceConfig);
  getFunctionAddressByName(libCamera, "sceCameraGetConfig", &sceCameraGetConfig);
  getFunctionAddressByName(libCamera, "sceCameraSetConfig", &sceCameraSetConfig);
}
void unloadCameraMod() {
  if (libCamera > 0)
    unloadModule(libCamera);
}