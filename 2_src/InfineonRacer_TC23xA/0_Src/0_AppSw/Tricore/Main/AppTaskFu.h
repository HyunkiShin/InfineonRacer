#ifndef APPTASKFU_H_
#define APPTASKFU_H_

#include <Ifx_Types.h>

#include "BasicTsl1401.h"
#include "BasicPort.h"
#include "BasicGtmTom.h"
#include "BasicVadcBgScan.h"
#include "BasicGpt12Enc.h"
#include "AsclinShellInterface.h"
#include "tft_app.h"
#include "Perf_Meas.h"
#include "InfineonRacer.h"

IFX_EXTERN boolean task_flag_1m;
IFX_EXTERN boolean task_flag_10m;
IFX_EXTERN boolean task_flag_100m;
IFX_EXTERN boolean task_flag_1000m;

void appTaskfu_init(void);
void appTaskfu_1ms(void);
void appTaskfu_10ms(void);
void appTaskfu_100ms(void);
void appTaskfu_1000ms(void);
void appTaskfu_idle(void);
void appIsrCb_1ms(void);

#endif /* APPTASKFU_H_ */
