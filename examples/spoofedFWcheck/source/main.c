#include "ps4.h"

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
  
  /* DO SOMETHING */
  return 0;
}
