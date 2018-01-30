/**
 * \file IfxEray_Eray.h
 * \brief ERAY ERAY details
 * \ingroup IfxLld_Eray
 *
 * \version iLLD_1_0_0_11_0
 * \copyright Copyright (c) 2013 Infineon Technologies AG. All rights reserved.
 *
 *
 *                                 IMPORTANT NOTICE
 *
 *
 * Infineon Technologies AG (Infineon) is supplying this file for use
 * exclusively with Infineon's microcontroller products. This file can be freely
 * distributed within development tools that are supporting such microcontroller
 * products.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * INFINEON SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 * OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 * \defgroup IfxLld_Eray_Eray_Usage How to use the ERAY Interface driver?
 * \ingroup IfxLld_Eray
 *
 * \section IfxLld_Eray_Eray_Preparation Preparation
 * \subsection IfxLld_Eray_Eray_Include Include Files
 *
 * Include following header file into your C code:
 *
 * \code
 * #include <Eray/Eray/IfxEray_Eray.h>
 * #include <Scu/Std/IfxScuWdt.h>
 * #include <eray_commands.h>
 * \endcode
 *
 *
 *
 * \subsection IfxLld_Eray_Eray_Defines Defines
 * \code
 *
 * // Number of tested communication cycles
 * #define NUMBER_ERAY_COMM_CYCLES 8
 * #define NO_OF_SLOTS                        2
 * \endcode
 *
 * \subsection IfxLld_Eray_Eray_Variables Variables
 *
 * \code
 * // global variables
 * static IfxEray_Eray eray;
 * volatile unsigned receivedDataCounter;
 * uint32 receivedData[NUMBER_ERAY_COMM_CYCLES][NO_OF_SLOTS];
 * \endcode
 *
 * \subsection IfxLld_Eray_Eray_Interrupt Interrupt Handler Installation
 * Define priorities for the Interrrupt handler. This is normally done in the Ifx_IntPrioDef.h file:
 * \code
 * #define IFX_ERAY_INT0_PRIO 1
 * #define IFX_ERAY_INT1_PRIO 2
 * #define IFX_ERAY_NDAT0_PRIO 3
 * #define IFX_ERAY_NDAT1_PRIO 4
 * #define IFX_ERAY_MBSC0_PRIO 5
 * #define IFX_ERAY_MBSC1_PRIO 6
 * \endcode
 *
 * Add the interrupt service routine to your C code. It has to call the ERAY interrupt handler:
 * \code
 * // ISR routines for interrupt handling
 * void ERAY_irqInt0Handler();
 * void ERAY_irqInt1Handler();
 * void ERAY_irqNdat0Handler();
 * void ERAY_irqNdat1Handler();
 * void ERAY_irqMbsc0Handler();
 * void ERAY_irqMbsc1Handler();
 *
 * IFX_INTERRUPT(eray0Int0ISR, 0, IFX_ERAY_INT0_PRIO)
 * {
 *     ERAY_irqInt0Handler();
 * }
 * IFX_INTERRUPT(eray0Int1ISR, 0, IFX_ERAY_INT1_PRIO)
 * {
 *     ERAY_irqInt1Handler();
 * }
 * IFX_INTERRUPT(eray0Ndat0ISR, 0, IFX_ERAY_NDAT0_PRIO)
 * {
 *     ERAY_irqNdat0Handler();
 * }
 * IFX_INTERRUPT(eray0Ndat1ISR, 0, IFX_ERAY_NDAT1_PRIO)
 * {
 *     ERAY_irqNdat1Handler();
 * }
 * IFX_INTERRUPT(eray0Mbsc0ISR, 0, IFX_ERAY_MBSC0_PRIO)
 * {
 *     ERAY_irqMbsc0Handler();
 * }
 * IFX_INTERRUPT(eray0Mbsc1ISR, 0, IFX_ERAY_MBSC1_PRIO)
 * {
 *     ERAY_irqMbsc1Handler();
 * }
 * \endcode
 *
 * Finally install the interrupt handlers in your initialisation function:
 * \code
 *     // install interrupt handler
 *     IfxCpu_Irq_installInterruptHandler(&eray0Int0ISR, IFX_ERAY_INT0_PRIO);
 *     IfxCpu_Irq_installInterruptHandler(&eray0Int1ISR, IFX_ERAY_INT1_PRIO);
 *     IfxCpu_Irq_installInterruptHandler(&eray0Ndat0ISR, IFX_ERAY_NDAT0_PRIO);
 *     IfxCpu_Irq_installInterruptHandler(&eray0Ndat1ISR, IFX_ERAY_NDAT1_PRIO);
 *     IfxCpu_Irq_installInterruptHandler(&eray0Mbsc0ISR, IFX_ERAY_MBSC0_PRIO);
 *     IfxCpu_Irq_installInterruptHandler(&eray0Mbsc1ISR, IFX_ERAY_MBSC1_PRIO);
 *
 *     // enable all cpu0 interrupts
 *     IfxCpu_enableInterrupts();
 * \endcode
 *
 *
 * \subsection IfxLld_Eray_Eray_Init Module Initialisation
 *
 * First ensure that the ERAY PLL is configured to output a 80 MHz clock for the ERAY module:
 * \code
 *     // initialize Eray PLL
 *     //clib_ver_printf("Initialize ERAY PLL\n");
 *     {
 *         IfxScuCcu_ErayPllConfig ErayPllConfig;
 *         IfxScuCcu_initErayPllConfig(&ErayPllConfig);
 *         result |= IfxScuCcu_initErayPll(&ErayPllConfig);
 *     }
 * \endcode
 *
 * Thereafter initialize the module:
 * \code
 *     // create module config
 *     IfxEray_Eray_Config erayModuleConfig;
 * #if TEST_ERAY1
 *     //clib_ver_printf("Initialize ERAY1\n");
 *     IfxEray_Eray_initModuleConfig(&erayModuleConfig, &MODULE_ERAY1);
 * #else
 *     //clib_ver_printf("Initialize ERAY0\n");
 *     IfxEray_Eray_initModuleConfig(&erayModuleConfig, &MODULE_ERAY0);
 * #endif
 *
 *     // ISR priorities
 *     erayModuleConfig.interrupt.int0Priority  = IFX_ERAY_INT0_PRIO;
 *     erayModuleConfig.interrupt.int1Priority  = IFX_ERAY_INT1_PRIO;
 *     erayModuleConfig.interrupt.ndat0Priority = IFX_ERAY_NDAT0_PRIO;
 *     erayModuleConfig.interrupt.ndat1Priority = IFX_ERAY_NDAT1_PRIO;
 *     erayModuleConfig.interrupt.mbsc0Priority = IFX_ERAY_MBSC0_PRIO;
 *     erayModuleConfig.interrupt.mbsc1Priority = IFX_ERAY_MBSC1_PRIO;
 *
 *     // init module
 *     IfxEray_Eray_initModule( &eray, &erayModuleConfig );
 * \endcode
 *
 * \subsection IfxLld_Eray_Eray_Node_Init Node Initialisation
 *
 * The node initialisation can be done in the same function.
 * \code
 *     // get ERAY node default configuration
 *     IfxEray_Eray_NodeConfig nodeConfig;
 *     IfxEray_Eray_Node_initConfig(&nodeConfig);
 *
 *     // configuration changes for interaction with external testbench element
 *     {
 *         // only for simulation: start external ERAY node
 *         {
 *             TbeMessage reply;
 *
 *             // start ERay Testbench Element
 *             sendMessage(MSG_ID_TESTBENCH_ERAY, CMD_ERAY_TBE_START, 0 );
 *             receiveMessageBlockingFrom(MSG_ID_TESTBENCH_ERAY, reply);
 *         }
 *
 *         {
 *             // SUCC1
 *             IfxEray_Eray_Succ1Config *succ1 = &nodeConfig.controllerConfig.succ1Config;
 *
 *             succ1->clockSyncErrorHalt = FALSE;
 *         }
 *
 *         {
 *             // PRTC1
 *             IfxEray_Eray_Prtc1Control *prtc1 = &nodeConfig.controllerConfig.prtc1Control;
 *
 *             prtc1->transmissionStartTime       = 10;
 *             prtc1->collisionAvoidanceDuration  = 0;
 *             prtc1->strobePosition              = IfxEray_StrobePosition_5;
 *             prtc1->baudrate                    = IfxEray_Baudrate_10;
 *             prtc1->receiveWakeupTestDuration   = 76;
 *             prtc1->transmitWakeupRepetitions   = 2;
 *         }
 *
 *         {
 *             // PRTC2
 *             IfxEray_Eray_Prtc2Control *prtc2 = &nodeConfig.controllerConfig.prtc2Control;
 *
 *             prtc2->receiveWakeupIdleTime     = 18;
 *             prtc2->receiveWakeupLowTime      = 18;
 *             prtc2->transmitWakeupIdleTime    = 180;
 *             prtc2->transmitWakeupLowTime     = 60;
 *         }
 *
 *         {
 *             // SUCC2
 *             IfxEray_Eray_Succ2Config *succ2 = &nodeConfig.controllerConfig.succ2Config;
 *             succ2->listenTimeOut = 2500;
 *             succ2->listenTimeOutNoise = IfxEray_ListenTimeOutNoise_16;
 *         }
 *
 *         {
 *             // GTU
 *             IfxEray_Eray_GTUConfig *gtu = &nodeConfig.controllerConfig.gtuConfig;
 *
 *             gtu->gtu01Config.microticksPerCycle = 3000;
 *             gtu->gtu02Config.macroticksPerCycle = 50;
 *             gtu->gtu02Config.maxSyncFrames = 15;
 *             gtu->gtu03Config.channelAMicrotickInitialOffset = 0;
 *             gtu->gtu03Config.channelBMicrotickInitialOffset = 0;
 *             gtu->gtu03Config.channelAMacrotickInitialOffset = 2;
 *             gtu->gtu03Config.channelBMacrotickInitialOffset = 2;
 *             gtu->gtu04Config.networkStartIdleTime = 0x2f;
 *             gtu->gtu04Config.correctionOffset     = 0x30;
 *             gtu->gtu05Config.channelAReceptionDelay = 0x4;
 *             gtu->gtu05Config.channelBReceptionDelay = 0x4;
 *             gtu->gtu05Config.clusterDrift           = 1;
 *             gtu->gtu05Config.decodingCorrection     = 0x34;
 *             gtu->gtu06Config.acceptedStartupDeviation = 0x81;
 *             gtu->gtu06Config.maxDriftOffset           = 0x31;
 *             gtu->gtu07Config.staticSlotLength = 0x10;
 *             gtu->gtu07Config.staticSlotsCount = 0x2;
 *             gtu->gtu08Config.dynamicSlotLength = 2;
 *             gtu->gtu08Config.dynamicSlotCount  = 2;
 *             gtu->gtu09Config.idleDynamicSlots   = IfxEray_IdleDynamicSlots_1; // bug: not set by IfxEray_setDynamicSlots
 *             gtu->gtu09Config.staticActionPoint  = 1;
 *             gtu->gtu09Config.dynamicActionPoint = 1;
 *             gtu->gtu10Config.maxOffsetCorrection = 0xcd;
 *             gtu->gtu10Config.maxRateCorrection   = 0x31;
 *             gtu->gtu11Config.externalOffsetCorrection = IfxEray_ExternalOffsetCorrection_0;
 *             gtu->gtu11Config.externalRateCorrection   = IfxEray_ExternalRateCorrection_0;
 *             gtu->gtu11Config.externalOffset           = IfxEray_ExternalOffset_noCorrection;
 *             gtu->gtu11Config.externalRate             = IfxEray_ExternalRate_noCorrection;
 *         }
 *     }
 *
 *     // Messages
 *     nodeConfig.messageRAMConfig.numberOfMessageBuffers = 2;
 *     nodeConfig.messageRAMConfig.firstDynamicBuffer = 0x40;
 *     nodeConfig.messageRAMConfig.fifoBufferStartIndex = 0x40;
 *
 *     // Frame header
 *     IfxEray_Header header[2] = {
 *         // fid  cyc  cha    chb     buffer direction                   ppit   transmission mode                     mbi    pl   dp   startupfr syncfr
 *         {  1,   1,   TRUE,  TRUE,   IfxEray_BufferDirection_transmit,  TRUE,  IfxEray_TransmissionMode_continuous, TRUE,    4,  0x30,  TRUE,   TRUE  },
 *         {  2,   1,   TRUE,  TRUE,   IfxEray_BufferDirection_receive,   FALSE, IfxEray_TransmissionMode_continuous, TRUE,    4,  0x40,  FALSE,  FALSE },
 *     };
 *
 *     // slot buffer
 *     IfxEray_SlotConfig slot[2] = {
 *         // header  data   stxrh   buffNum
 *         {  TRUE,   TRUE,  TRUE,   0   },
 *         {  TRUE,   FALSE, FALSE,  1   },
 *     };
 *
 *     for(int i=0; i < 2; ++i) {
 *         nodeConfig.messageRAMConfig.header[i]      = &header[i];
 *         nodeConfig.messageRAMConfig.slotControl[i] = &slot[i];
 *         nodeConfig.messageRAMConfig.data[i]        = NULL_PTR;
 *     }
 *
 * #if TEST_ERAY1
 *     // channel A pins
 *     const IfxEray_Eray_NodeA nodeAPins = {
 *         IfxPort_InputMode_pullDown, IfxPort_OutputMode_pushPull,
 *         IfxPort_PadDriver_cmosAutomotiveSpeed3, IfxPort_OutputMode_pushPull,
 *         &IfxEray1_RXDA0_P14_8_IN, &IfxEray1_TXDA_P14_10_OUT,
 *         &IfxEray1_TXENA_P14_9_OUT
 *     };
 *
 *     // channel B pins
 *     const IfxEray_Eray_NodeB nodeBPins = {
 *         IfxPort_InputMode_pullDown, IfxPort_OutputMode_pushPull,
 *         IfxPort_PadDriver_cmosAutomotiveSpeed3, IfxPort_OutputMode_pushPull,
 *         &IfxEray1_RXDB0_P14_7_IN, &IfxEray1_TXDB_P14_5_OUT,
 *         &IfxEray1_TXENB_P14_6_OUT
 *     };
 * #else
 *     // channel A pins
 *     const IfxEray_Eray_NodeA nodeAPins = {
 *         IfxPort_InputMode_pullDown, IfxPort_OutputMode_pushPull,
 *         IfxPort_PadDriver_cmosAutomotiveSpeed3, IfxPort_OutputMode_pushPull,
 *         &IfxEray0_RXDA0_P14_8_IN, &IfxEray0_TXDA_P14_10_OUT,
 *         &IfxEray0_TXENA_P14_9_OUT
 *     };
 *
 *     // channel B pins
 *     const IfxEray_Eray_NodeB nodeBPins = {
 *         IfxPort_InputMode_pullDown, IfxPort_OutputMode_pushPull,
 *         IfxPort_PadDriver_cmosAutomotiveSpeed3, IfxPort_OutputMode_pushPull,
 *         &IfxEray0_RXDB0_P14_7_IN, &IfxEray0_TXDB_P14_5_OUT,
 *         &IfxEray0_TXENB_P14_6_OUT
 *     };
 * #endif
 *
 *     const IfxEray_Eray_Pins pins = { (IfxEray_Eray_NodeA *)&nodeAPins, (IfxEray_Eray_NodeB *)&nodeBPins };
 *     nodeConfig.pins = (IfxEray_Eray_Pins *)&pins;
 *
 *     // ERAY node initialisation with supplied configuration
 *     IfxEray_Eray_Node_init(&eray, &nodeConfig);
 *
 *     // set interrupt outputs for data and message buffers
 *     IfxEray_Eray_setNewDataInterruptDestination(&eray, 0, 0); // New Data #0 -> NDAT0
 *     IfxEray_Eray_setNewDataInterruptDestination(&eray, 1, 1); // New Data #1 -> NDAT1
 *     IfxEray_Eray_setMessageBufferInterruptDestination(&eray, 0, 0); // Message Buffer #0 -> MBSC0
 *     IfxEray_Eray_setMessageBufferInterruptDestination(&eray, 1, 1); // Message Buffer #1 -> MBSC1
 *
 *     //clib_ver_printf("Send wakeup pattern\n");
 *
 *     // send wakeup pattern on default configured channel ( IfxEray_Channel_a )
 *     IfxEray_PocState pocState;
 *     do {
 *         pocState = IfxEray_Eray_getPocState( &eray );
 *         if (   pocState != IfxEray_PocState_wakeupListen ) {
 *             while ( !IfxEray_Eray_changePocState( &eray, IfxEray_PocCommand_wakeup ));
 *         }
 *     } while ( pocState != IfxEray_PocState_wakeupListen );
 *
 *     // allow node to cold-start and wait until cold-start is successful
 *     while( !IfxEray_Eray_allowColdStart(&eray) );
 *
 *     // start the communication in cluster and wait until communication is successfull
 *     while( !IfxEray_Eray_startCommunication(&eray) );
 *
 *     // wait until communication Controller enters NORMAL_ACTIVE or NORMAL_PASSIVE state, exit if communication Controller enters HALT state.
 *     do {
 *         pocState = IfxEray_Eray_getPocState( &eray );
 *         // if communication Controller enters HALT state, break the loop.
 *         if( pocState == IfxEray_PocState_halt) {
 *             break;
 *         }
 *     } while( !((pocState == IfxEray_PocState_normalActive) || (pocState == IfxEray_PocState_normalPassive)) );
 *
 *     // enter ALL_SLOTS mode when Communication Controller is in NORMAL_ACTIVE or NORMAL_PASSIVE state only.
 *     if ( pocState == IfxEray_PocState_normalActive || pocState == IfxEray_PocState_normalPassive ) {
 *         while ( !IfxEray_Eray_setPocAllSlots(&eray) );
 *     }
 * \endcode
 *
 * \subsection IfxLld_Eray_Eray_TxRx Transactions
 *
 * \code
 *     unsigned prevReceivedDataCounter = 0;
 *     while( 1 ) {
 *         if( receivedDataCounter != prevReceivedDataCounter ) {
 *             prevReceivedDataCounter = receivedDataCounter;
 *             //clib_ver_printf("Received %d packets", receivedDataCounter);
 *
 *             if( receivedDataCounter >= NUMBER_ERAY_COMM_CYCLES )
 *                 break;
 *         }
 *     };
 * \endcode
 *
 *
 * \subsection IfxLld_Eray_Eray_ISR Interrupt Service Handlers
 *
 * The ISR has to be implemented in the application. Following templates can be used to react on the events:
 *
 * \code
 * void ERAY_irqInt0Handler()
 * {
 *     //clib_ver_note(1); // for debugging in simulation: notify Int0
 *
 *     Ifx_ERAY_EIR ErrIntStat = IfxEray_Eray_getErrorInterrupts( &eray );
 *     Ifx_ERAY_SIR StatusIntStat = IfxEray_Eray_getStatusInterrupts( &eray );
 *
 *     if(StatusIntStat.B.SDS)
 *     {
 *         // SDS must be cleared for getting the next interrupt
 *         IfxEray_Eray_clearStatusFlag( &eray, IfxEray_ClearStatusFlag_sds );
 *     }
 *     if(StatusIntStat.B.CYCS)
 *     {
 *         // CYCS must be cleared for getting the next interrupt
 *         IfxEray_Eray_clearStatusFlag( &eray, IfxEray_ClearStatusFlag_cycs );
 *     }
 *     if(StatusIntStat.B.SUCS)
 *     {
 *         // SUCS must be cleared for getting the next interrupt
 *         IfxEray_Eray_clearStatusFlag( &eray, IfxEray_ClearStatusFlag_sucs );
 *     }
 *     if(StatusIntStat.B.TIBC)
 *     {
 *         // TIBC must be cleared for getting the next interrupt
 *         IfxEray_Eray_clearStatusFlag( &eray, IfxEray_ClearStatusFlag_tibc );
 *     }
 *     if(ErrIntStat.B.MHF)
 *     {
 *         // clear flag MHF
 *         IfxEray_Eray_clearErrorFlag( &eray, IfxEray_ClearErrorFlag_mhf );
 *     }
 * }
 *
 * void ERAY_irqInt1Handler()
 * {
 *     //clib_ver_note(2); // for debugging in simulation: notify Int1
 *
 *     // TOBC must be cleared for getting the next interrupt
 *      IfxEray_Eray_clearStatusFlag( &eray, IfxEray_ClearStatusFlag_tobc );
 * }
 *
 * void ERAY_irqNdat0Handler()
 * {
 *     //clib_ver_note(3); // for debugging in simulation: notify Ndat0
 * }
 *
 * void ERAY_irqNdat1Handler()
 * {
 *     //clib_ver_note(4); // for debugging in simulation: notify Ndat1
 *
 *     // message received in slot1 buffer?
 *     if( IfxEray_Eray_getNewDataInterruptStatus(&eray, 1) == 1 )
 *     {
 *         // read data
 *         uint32 data[2];
 *
 *         {
 *             IfxEray_Eray_ReceiveControl config;
 *             config.headerReceived   = TRUE;
 *             config.dataReceived     = TRUE;
 *             config.receiveRequested = TRUE;
 *             config.swapRequested    = TRUE;
 *             config.bufferIndex      = 1;
 *
 *             IfxEray_Eray_receiveFrame(&eray, &config);
 *             IfxEray_Eray_readData(&eray, data, 2*2);
 *         }
 *
 *         // display received data (Here received data has to be displayed)
 *         //clib_ver_data(data[0]);
 *         //clib_ver_data(data[1]);
 *
 *         // store received data
 *         if( receivedDataCounter < NUMBER_ERAY_COMM_CYCLES ) {
 *             receivedData[receivedDataCounter][0] = data[0];
 *             receivedData[receivedDataCounter][1] = data[1];
 *         }
 *         ++receivedDataCounter;
 *
 *         // put some new data into slot0
 *         ++data[0];
 *         ++data[1];
 *
 *         {
 *             IfxEray_Eray_TransmitControl config;
 *             config.headerTransfered  = FALSE;
 *             config.dataTransfered    = TRUE;
 *             config.transferRequested = TRUE;
 *             config.bufferIndex       = 0;
 *
 *             IfxEray_Eray_writeData(&eray, data, 2*2);
 *             IfxEray_Eray_transmitFrame(&eray, &config);
 *         }
 *     }
 * }
 *
 * void ERAY_irqMbsc0Handler()
 * {
 *     //clib_ver_note(5); // for debugging in simulation: notify Mbsc0
 * }
 *
 * void ERAY_irqMbsc1Handler()
 * {
 *     //clib_ver_note(6); // for debugging in simulation: notify Mbsc1
 * }
 * \endcode
 *
 * \defgroup IfxLld_Eray_Eray ERAY
 * \ingroup IfxLld_Eray
 * \defgroup IfxLld_Eray_Eray_Structures Data Structures
 * \ingroup IfxLld_Eray_Eray
 * \defgroup IfxLld_Eray_Eray_Module Module Initialisation Functions
 * \ingroup IfxLld_Eray_Eray
 * \defgroup IfxLld_Eray_Eray_Node Node Initialisation Functions
 * \ingroup IfxLld_Eray_Eray
 * \defgroup IfxLld_Eray_Eray_Operative Operative Functions
 * \ingroup IfxLld_Eray_Eray
 * \defgroup IfxLld_Eray_Eray_Status Status Functions
 * \ingroup IfxLld_Eray_Eray
 * \defgroup IfxLld_Eray_Eray_Interrupt Interrupt Functions
 * \ingroup IfxLld_Eray_Eray
 */

#ifndef IFXERAY_ERAY_H
#define IFXERAY_ERAY_H 1

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/

#include "Eray/Std/IfxEray.h"
#include "Cpu/Std/IfxCpu.h"
#include "Scu/Std/IfxScuWdt.h"
#include "Scu/Std/IfxScuCcu.h"

/******************************************************************************/
/*-----------------------------Data Structures--------------------------------*/
/******************************************************************************/

/** \addtogroup IfxLld_Eray_Eray_Structures
 * \{ */
/** \brief GTU01 configuration.
 */
typedef struct
{
    uint32 microticksPerCycle;     /**< \brief Duration of the communication cycle in Microticks. */
} IfxEray_Eray_Gtu01Config;

/** \brief GTU02 configuration.
 */
typedef struct
{
    uint16 macroticksPerCycle;     /**< \brief Duration of the communication cycle in Macroticks. */
    uint8  maxSyncFrames;          /**< \brief Maximum number of sync frames in a cluster. */
} IfxEray_Eray_Gtu02Config;

/** \brief GTU03 configuration.
 */
typedef struct
{
    uint8 channelAMicrotickInitialOffset;     /**< \brief difference between adjacent time reference points on channel A in microticks. */
    uint8 channelBMicrotickInitialOffset;     /**< \brief difference between adjacent time reference points on channel B in microticks. */
    uint8 channelAMacrotickInitialOffset;     /**< \brief difference between two adjacent static slots on channel A in macroticks. */
    uint8 channelBMacrotickInitialOffset;     /**< \brief difference between two adjacent static slots on channel B in macroticks. */
} IfxEray_Eray_Gtu03Config;

/** \brief GTU04 configuration.
 */
typedef struct
{
    uint16 networkStartIdleTime;     /**< \brief starting point of Network Idle Time Phase. */
    uint16 correctionOffset;         /**< \brief offset correction start point. */
} IfxEray_Eray_Gtu04Config;

/** \brief GTU05 configuration.
 */
typedef struct
{
    uint8 channelAReceptionDelay;     /**< \brief reception delay on channel A. */
    uint8 channelBReceptionDelay;     /**< \brief reception delay on channel B. */
    uint8 clusterDrift;               /**< \brief cluster drift damping value used in clock synchronization. */
    uint8 decodingCorrection;         /**< \brief decoding correction to determine the primary time reference point. */
} IfxEray_Eray_Gtu05Config;

/** \brief GTU06 configuration.
 */
typedef struct
{
    uint16 acceptedStartupDeviation;     /**< \brief expanded range of measured deviation for startup Frames during integration. */
    uint16 maxDriftOffset;               /**< \brief maximum drift offset between two nodes. */
} IfxEray_Eray_Gtu06Config;

/** \brief GTU07 configuration.
 */
typedef struct
{
    uint16 staticSlotLength;     /**< \brief duration of static slot in macroticks. */
    uint16 staticSlotsCount;     /**< \brief number of static slots in a communication cycle. */
} IfxEray_Eray_Gtu07Config;

/** \brief GTU08 configuration.
 */
typedef struct
{
    uint8  dynamicSlotLength;     /**< \brief duration of dynamic slot in macroticks. */
    uint16 dynamicSlotCount;      /**< \brief number of dynamic slots in a communication cycle. */
} IfxEray_Eray_Gtu08Config;

/** \brief GTU09 configuration.
 */
typedef struct
{
    uint8                    staticActionPoint;      /**< \brief static slots and symbol window action point. */
    uint8                    dynamicActionPoint;     /**< \brief dynamic slots action point. */
    IfxEray_IdleDynamicSlots idleDynamicSlots;       /**< \brief duration of dynamic slot idle phase. */
} IfxEray_Eray_Gtu09Config;

/** \brief GTU10 configuration.
 */
typedef struct
{
    uint16 maxOffsetCorrection;     /**< \brief maximum offset correction to be applied by the internal clock synchronization algorithm. */
    uint16 maxRateCorrection;       /**< \brief maximum rate correction to be applied by the internal clock synchronization algorithm. */
} IfxEray_Eray_Gtu10Config;

/** \brief GTU11 configuration.
 */
typedef struct
{
    IfxEray_ExternalOffsetCorrection externalOffsetCorrection;     /**< \brief External clock offset correction value. */
    IfxEray_ExternalRateCorrection   externalRateCorrection;       /**< \brief External clock rate correction value. */
    IfxEray_ExternalOffset           externalOffset;               /**< \brief External offset correction control. */
    IfxEray_ExternalRate             externalRate;                 /**< \brief External rate correction control. */
} IfxEray_Eray_Gtu11Config;

/** \} */

/** \addtogroup IfxLld_Eray_Eray_Structures
 * \{ */
/** \brief Gloabl Timing Unit configuration structure.
 */
typedef struct
{
    IfxEray_Eray_Gtu01Config gtu01Config;     /**< \brief GTU01 configuration. */
    IfxEray_Eray_Gtu02Config gtu02Config;     /**< \brief GTU02 configuration. */
    IfxEray_Eray_Gtu03Config gtu03Config;     /**< \brief GTU03 configuration. */
    IfxEray_Eray_Gtu04Config gtu04Config;     /**< \brief GTU04 configuration. */
    IfxEray_Eray_Gtu05Config gtu05Config;     /**< \brief GTU05 configuration. */
    IfxEray_Eray_Gtu06Config gtu06Config;     /**< \brief GTU06 configuration. */
    IfxEray_Eray_Gtu07Config gtu07Config;     /**< \brief GTU07 configuration. */
    IfxEray_Eray_Gtu08Config gtu08Config;     /**< \brief GTU08 configuration. */
    IfxEray_Eray_Gtu09Config gtu09Config;     /**< \brief GTU09 configuration. */
    IfxEray_Eray_Gtu10Config gtu10Config;     /**< \brief GTU10 configuration. */
    IfxEray_Eray_Gtu11Config gtu11Config;     /**< \brief GTU11 configuration. */
} IfxEray_Eray_GTUConfig;

/** \brief Pins configuration structure for Node A.
 */
typedef struct
{
    IfxPort_InputMode       rxInMode;        /**< \brief the RX pin input mode which should be configured */
    IfxPort_OutputMode      txOutMode;       /**< \brief the TX pin output mode which should be configured. */
    IfxPort_PadDriver       pinDriver;       /**< \brief the pad driver mode which should be configured. */
    IfxPort_OutputMode      txEnOutMode;     /**< \brief the TXEN pin output mode which should be configured. */
    const IfxEray_Rxd_In   *rxIn;            /**< \brief the RX Pin which should be configured. */
    const IfxEray_Txd_Out  *txOut;           /**< \brief the TX Pin which should be configured. */
    const IfxEray_Txen_Out *txEnOut;         /**< \brief the TXEN Pin which should be configured. */
} IfxEray_Eray_NodeA;

/** \brief Pins configuration structure for Node B.
 */
typedef struct
{
    IfxPort_InputMode       rxInMode;        /**< \brief the RX pin input mode which should be configured */
    IfxPort_OutputMode      txOutMode;       /**< \brief the TX pin output mode which should be configured. */
    IfxPort_PadDriver       pinDriver;       /**< \brief the pad driver mode which should be configured. */
    IfxPort_OutputMode      txEnOutMode;     /**< \brief the TXEN pin output mode which should be configured. */
    const IfxEray_Rxd_In   *rxIn;            /**< \brief the RX Pin which should be configured. */
    const IfxEray_Txd_Out  *txOut;           /**< \brief the TX Pin which should be configured. */
    const IfxEray_Txen_Out *txEnOut;         /**< \brief the TXEN Pin which should be configured. */
} IfxEray_Eray_NodeB;

/** \brief Protocol operation control properties.
 */
typedef struct
{
    uint8                  collisionAvoidanceDuration;     /**< \brief accepted duration of collision avoidance symbol. */
    IfxEray_StrobePosition strobePosition;                 /**< \brief sample count value for strobing. */
    IfxEray_Baudrate       baudrate;                       /**< \brief baud rate on the flexray bus. */
    uint8                  receiveWakeupTestDuration;      /**< \brief duration of received wakeup pattern in bit times. */
    uint8                  transmitWakeupRepetitions;      /**< \brief duration of transmitted wakeup pattern in bit times. */
    uint16                 transmissionStartTime;          /**< \brief duration of transmission start time. */
} IfxEray_Eray_Prtc1Control;

/** \brief Wakeup symbol control properties.
 */
typedef struct
{
    uint8 receiveWakeupIdleTime;      /**< \brief duration of received wakeup symbol idle phase. */
    uint8 receiveWakeupLowTime;       /**< \brief duration of received wakeup symbol low phase. */
    uint8 transmitWakeupIdleTime;     /**< \brief duration of transmit wakeup symbol idle phase. */
    uint8 transmitWakeupLowTime;      /**< \brief duration of transmit wakeup symbol low phase. */
} IfxEray_Eray_Prtc2Control;

/** \brief communication controller control properties.
 */
typedef struct
{
    boolean                      startupFrameTransmitted;         /**< \brief whether the key slot is used to transmit startup Frames. */
    boolean                      syncFrameTransmitted;            /**< \brief whether the key slot is used to transmit SYNC Frames. */
    uint8                        maxColdStartAttempts;            /**< \brief maximum number of attempts that a cold starting node is permitted. */
    uint8                        numberOfCyclePairsForActive;     /**< \brief number of even / odd cycle pairs that must have valid clock correction terms. */
    IfxEray_WakeupChannel        wakeupPatternChannel;            /**< \brief Wakeup pattern carry channel */
    IfxEray_TransmissionSlotMode transmissionSlotMode;            /**< \brief Initial transmission mode. */
    boolean                      clockSyncErrorHalt;              /**< \brief transition to HALT state due to a clock synchronization error. */
    boolean                      channelASymbolTransmitted;       /**< \brief selects channel A for MTS symbol transmission. */
    boolean                      channelBSymbolTransmitted;       /**< \brief selects channel B for MTS symbol transmission */
    boolean                      channelAConnectedNode;           /**< \brief whether the node is connected to channel A. */
    boolean                      channelBConnectedNode;           /**< \brief whether the node is connected to channel B. */
} IfxEray_Eray_Succ1Config;

/** \brief Communication listen timeout properties.
 */
typedef struct
{
    uint32                     listenTimeOut;          /**< \brief wakeup or startup listen timeout in microticks. */
    IfxEray_ListenTimeOutNoise listenTimeOutNoise;     /**< \brief upper limit for startup and wakeup listen timeout in the presence of noise. */
} IfxEray_Eray_Succ2Config;

/** \brief Clock correction fail properties.
 */
typedef struct
{
    uint8 clockCorrectionCyclesPassive;     /**< \brief maximum number of cycles missing clock correction lead active to passive state. */
    uint8 clockCorrectionCyclesHalt;        /**< \brief maximum number of cycles missing clock correctionlead to active or passive to halt. */
} IfxEray_Eray_Succ3Config;

/** \} */

/** \addtogroup IfxLld_Eray_Eray_Structures
 * \{ */
/** \brief Communication Controller configuration structure.
 */
typedef struct
{
    uint32                    networkVectorLength;         /**< \brief length of network management vector. */
    uint8                     staticFramepayload;          /**< \brief payload length of static frame in double bytes. */
    uint8                     latestTransmissionStart;     /**< \brief number of dynamic slots before inhibit frame transmission in dynamic segment. */
    IfxEray_Eray_GTUConfig    gtuConfig;                   /**< \brief gloabl timing unit configuration structure. */
    IfxEray_Eray_Succ1Config  succ1Config;                 /**< \brief communication controller control properties. */
    IfxEray_Eray_Succ2Config  succ2Config;                 /**< \brief communication listen timeout properties. */
    IfxEray_Eray_Succ3Config  succ3Config;                 /**< \brief clock correction fail properties. */
    IfxEray_Eray_Prtc1Control prtc1Control;                /**< \brief protocol operation control properties. */
    IfxEray_Eray_Prtc2Control prtc2Control;                /**< \brief wakeup symbol control properties. */
} IfxEray_Eray_ControllerConfig;

/** \brief Interrupt control properties.
 */
typedef struct
{
    uint16     int0Priority;         /**< \brief the interrupt line 0 priority. Always 1 since all interrupts are handled at a time. */
    uint16     int1Priority;         /**< \brief the interrupt line 1 priority. Always 1 since all interrupts are handled at a time. */
    uint16     tint0Priority;        /**< \brief the timer interrupt 0 priority. Always 1 since all interrupts are handled at a time. */
    uint16     tint1Priority;        /**< \brief the timer interrupt 1 priority. Always 1 since all interrupts are handled at a time. */
    uint16     ndat0Priority;        /**< \brief the new data interrupt 0 priority. Always 1 since all interrupts are handled at a time. */
    uint16     ndat1Priority;        /**< \brief the new data interrupt 1 priority. Always 1 since all interrupts are handled at a time. */
    uint16     mbsc0Priority;        /**< \brief the message buffer status changed interrupt 0 priority. Always 1 since all interrupts are handled at a time. */
    uint16     mbsc1Priority;        /**< \brief the message buffer status changed interrupt 1 priority. Always 1 since all interrupts are handled at a time. */
    uint16     ibusyPriority;        /**< \brief the inputput buffer interrupt priority. Always 1 since all interrupts are handled at a time. */
    uint16     obusyPriority;        /**< \brief the output buffer interrupt priority. Always 1 since all interrupts are handled at a time. */
    IfxSrc_Tos int0IsrProvider;      /**< \brief the interrupt line 0 service provider. CPU or DMA. */
    IfxSrc_Tos int1IsrProvider;      /**< \brief the interrupt line 1 service provider. CPU or DMA. */
    IfxSrc_Tos tint0IsrProvider;     /**< \brief the timer interrupt line 0 service provider. CPU or DMA. */
    IfxSrc_Tos tint1IsrProvider;     /**< \brief the timer interrupt line 1 service provider. CPU or DMA. */
    IfxSrc_Tos ndat0IsrProvider;     /**< \brief the new data interrupt 0 service provider. CPU or DMA. */
    IfxSrc_Tos ndat1IsrProvider;     /**< \brief the new data interrupt 1 service provider. CPU or DMA. */
    IfxSrc_Tos mbsc0IsrProvider;     /**< \brief the message buffer status changed interrupt 0 service provider. CPU or DMA. */
    IfxSrc_Tos mbsc1IsrProvider;     /**< \brief the message buffer status changed interrupt 1 service provider. CPU or DMA. */
    IfxSrc_Tos ibusyIsrProvider;     /**< \brief the input buffer interrupt service provider. CPU or DMA. */
    IfxSrc_Tos obusyIsrProvider;     /**< \brief the output buffer interrupt service provider. CPU or DMA. */
} IfxEray_Eray_Interrupt;

/** \brief Message RAM configuration structure.
 */
typedef struct
{
    uint8                  firstDynamicBuffer;         /**< \brief first dynamic buffer index. */
    uint8                  numberOfMessageBuffers;     /**< \brief last configured buffer. */
    uint32                 fifoBufferStartIndex;       /**< \brief message buffers assigned FIFO. */
    uint8                  fifoDepth;                  /**< \brief fifo critical level. */
    uint16                 frameIdFilter;              /**< \brief Frame ID  used for rejection filtering. */
    IfxEray_ReceiveChannel receiveChannel;             /**< \brief FIFO receive channel. */
    uint16                 rejectedFrameId;            /**< \brief frame ID to be rejected by the FIFO. */
    uint8                  filteredCycleNumber;        /**< \brief the cycle set to which Frame ID and channel rejection filter are applied. */
    boolean                staticFifoDisabled;         /**< \brief fifo is not used in static segment. */
    boolean                fifoNullFramesRejected;     /**< \brief whether null frames stored in fifo or not. */
    boolean                bufferReconfigEnabled;      /**< \brief whether reconfiguration of message buffers is enabled or not. */
    boolean                fifoConfigured;             /**< \brief Whether receive FIFO configured or not. */
    IfxEray_Header        *header[128];                /**< \brief Header section of message buffer. */
    IfxEray_SlotConfig    *slotControl[128];           /**< \brief Transmit control properties. */
    uint32                *data[128];                  /**< \brief data section of message buffer. */
} IfxEray_Eray_MessageRAMConfig;

/** \} */

/** \addtogroup IfxLld_Eray_Eray_Node
 * \{ */
/** \brief Pins configuration structure.
 */
typedef struct
{
    const IfxEray_Eray_NodeA *nodeAPins;     /**< \brief Pins configuration structure for Node A. */
    const IfxEray_Eray_NodeB *nodeBPins;     /**< \brief Pins configuration structure for Node B. */
} IfxEray_Eray_Pins;

/** \} */

/** \addtogroup IfxLld_Eray_Eray_Structures
 * \{ */
/** \brief ERAY Module handle.
 */
typedef struct
{
    Ifx_ERAY *eray;     /**< \brief pointer to ERAY module registers. */
} IfxEray_Eray;

/** \brief Reconfigurable buffer structure.
 */
typedef struct
{
    IfxEray_Header     *header;          /**< \brief header of a reconfigurable buffer. */
    uint32              data[64];        /**< \brief data section of a reconfigurable buffer. */
    IfxEray_SlotConfig *slotControl;     /**< \brief message buffer configuration in message RAM. */
} IfxEray_Eray_BufferReconfig;

/** \brief Module configuration structure
 */
typedef struct
{
    Ifx_ERAY              *module;        /**< \brief pointer to ERAY module registers. */
    IfxEray_Eray_Interrupt interrupt;     /**< \brief Interrupt control properties. */
} IfxEray_Eray_Config;

/** \brief Node configuration structure.
 */
typedef struct
{
    IfxEray_Eray_MessageRAMConfig messageRAMConfig;     /**< \brief Message RAM configuration structure. */
    IfxEray_Eray_ControllerConfig controllerConfig;     /**< \brief Communication Controller configuration structure. */
    const IfxEray_Eray_Pins      *pins;                 /**< \brief Pins configuration structure. */
} IfxEray_Eray_NodeConfig;

/** \brief Receive control properties structure.
 */
typedef struct
{
    boolean headerReceived;       /**< \brief whether header selected for transfer from Message RAM to Output Buffer or not. */
    boolean dataReceived;         /**< \brief whether data selected for transfer from Message RAM to Output Buffer or not. */
    boolean swapRequested;        /**< \brief whether output buffer shadow and output buffer host are swapped or not. */
    boolean receiveRequested;     /**< \brief  */
    uint8   bufferIndex;          /**< \brief buffer index in the Message RAM. */
} IfxEray_Eray_ReceiveControl;

/** \brief Received Frame.
 */
typedef struct
{
    IfxEray_ReceivedHeader header;       /**< \brief received header in a frame. */
    uint32                 data[64];     /**< \brief received data in a frame. */
} IfxEray_Eray_ReceivedFrame;

/** \brief Transfer control in a slot.
 */
typedef struct
{
    boolean headerTransfered;      /**< \brief whether header section of frame is transfered or not. */
    boolean dataTransfered;        /**< \brief whether data section of message buffer is transfered or not. */
    boolean transferRequested;     /**< \brief whether frame in a slot is transfered or not. */
    uint8   bufferIndex;           /**< \brief buffer index which gives slot for transfers. */
} IfxEray_Eray_TransmitControl;

/** \} */

/** \addtogroup IfxLld_Eray_Eray_Module
 * \{ */

/******************************************************************************/
/*-------------------------Inline Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief De-initialises the ERAY module.
 * \param eray pointer to module handle.
 * \return None
 *
 * For usage exapmle see : \ref IfxLld_Eray_Eray_Usage
 *
 */
IFX_INLINE void IfxEray_Eray_deInitModule(IfxEray_Eray *eray);

/******************************************************************************/
/*-------------------------Global Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief Initialises the ERAY module with supplied configuration.
 * \param eray pointer to ERAY Module handle.
 * \param config pointer to module configuration structure.
 * \return None
 *
 * For usage exapmle see : \ref IfxLld_Eray_Eray_Usage
 *
 */
IFX_EXTERN void IfxEray_Eray_initModule(IfxEray_Eray *eray, const IfxEray_Eray_Config *config);

/** \brief Inialises the default module configuration buffer.
 * \param config pointer to module configuration structure.
 * \param eray pointer ERAY module registers.
 * \return None
 *
 * For usage exapmle see : \ref IfxLld_Eray_Eray_Usage
 *
 */
IFX_EXTERN void IfxEray_Eray_initModuleConfig(IfxEray_Eray_Config *config, Ifx_ERAY *eray);

/** \} */

/** \addtogroup IfxLld_Eray_Eray_Node
 * \{ */

/******************************************************************************/
/*-------------------------Global Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief Initialises the Node with supplied configuration.
 * \param eray pointer to ERAY Module handle.
 * \param config pointer to node configuration structure.
 * \return None
 *
 * For usage exapmle see : \ref IfxLld_Eray_Eray_Usage
 *
 */
IFX_EXTERN void IfxEray_Eray_Node_init(IfxEray_Eray *eray, const IfxEray_Eray_NodeConfig *config);

/** \brief Initialises the default node configuration buffer.
 * \param config pointer to node configuration structure.
 * \return None
 *
 * For usage exapmle see : \ref IfxLld_Eray_Eray_Usage
 *
 */
IFX_EXTERN void IfxEray_Eray_Node_initConfig(IfxEray_Eray_NodeConfig *config);

/** \} */

/** \addtogroup IfxLld_Eray_Eray_Operative
 * \{ */

/******************************************************************************/
/*-------------------------Inline Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief Allows node to enter the cold startup state.
 * \param eray pointer to ERAY Module handle.
 * \return TRUE if cold start successful otherwise FALSE.
 *
 * For usage exapmle see : \ref IfxLld_Eray_Eray_Usage
 *
 */
IFX_INLINE boolean IfxEray_Eray_allowColdStart(IfxEray_Eray *eray);

/** \brief Reconfiguring the buffer or changing the slot.
 * \param eray pointer to module handle.
 * \param reconfigBuffer pointer reconfigurable buffer structure.
 * \return None
 *
 * For usage exapmle see : \ref IfxLld_Eray_Eray_Usage
 *
 */
IFX_INLINE void IfxEray_Eray_bufferReconfig(IfxEray_Eray *eray, IfxEray_Eray_BufferReconfig *reconfigBuffer);

/** \brief Changes the POC state and return status.
 * \param eray pointer to module handle.
 * \param PocCommand POC command to change state.
 * \return TRUE if command accepted.
 */
IFX_INLINE boolean IfxEray_Eray_changePocState(IfxEray_Eray *eray, IfxEray_PocCommand PocCommand);

/** \brief Reads the received data from output registers.
 * \param eray pointer to module handle.
 * \param data pointer to received data buffer.
 * \param payloadLength payload length received in a frame.
 * \return None
 */
IFX_INLINE void IfxEray_Eray_readData(IfxEray_Eray *eray, uint32 *data, uint8 payloadLength);

/** \brief reads the frame received in a buffer.
 * \param eray pointer to module handle.
 * \param frame received frame in a buffer.
 * \param maxPayloadLength maximum payload length received in a frame.
 * \return None
 *
 * For usage exapmle see : \ref IfxLld_Eray_Eray_Usage
 *
 */
IFX_INLINE void IfxEray_Eray_readFrame(IfxEray_Eray *eray, IfxEray_Eray_ReceivedFrame *frame, Ifx_SizeT maxPayloadLength);

/** \brief Sets the Controller state to ALL Slots mode.
 * \param eray pointer to module handle.
 * \return TRUE if All slots mode is successful otherwise FALSE.
 *
 * For usage exapmle see : \ref IfxLld_Eray_Eray_Usage
 *
 */
IFX_INLINE boolean IfxEray_Eray_setPocAllSlots(IfxEray_Eray *eray);

/** \brief Runs the communication controller.
 * \param eray pointer to ERAY Module handle.
 * \return TRUE if communication started otherwise FALSE.
 *
 * For usage exapmle see : \ref IfxLld_Eray_Eray_Usage
 *
 */
IFX_INLINE boolean IfxEray_Eray_startCommunication(IfxEray_Eray *eray);

/** \brief Wakeups the channel in a cluster.
 * \param eray pointer to ERAY Module handle.
 * \param channel wakeup channel Id.
 * \return None
 *
 * For usage exapmle see : \ref IfxLld_Eray_Eray_Usage
 *
 */
IFX_INLINE void IfxEray_Eray_wakeUpChannel(IfxEray_Eray *eray, IfxEray_Channel channel);

/** \brief Wakeups the node in a cluster.
 * \param eray pointer to ERAY Module handle.
 * \return TRUE if cluster is wakeup otherwise FALSE.
 *
 * For usage exapmle see : \ref IfxLld_Eray_Eray_Usage
 *
 */
IFX_INLINE boolean IfxEray_Eray_wakeUpCluster(IfxEray_Eray *eray);

/** \brief Writes data section of a frame to input data registers.
 * \param eray pointer to module handle.
 * \param data data section in frame.
 * \param payloadLength payload length configured for slot buffer.
 * \return None
 *
 * For usage exapmle see : \ref IfxLld_Eray_Eray_Usage
 *
 */
IFX_INLINE void IfxEray_Eray_writeData(IfxEray_Eray *eray, uint32 *data, uint8 payloadLength);

/******************************************************************************/
/*-------------------------Global Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief Transfers frame from fifo in message RAM to Output buffer.
 * \param eray pointer to ERAY Module handle.
 * \param config pointer to receive control structure.
 * \return None
 *
 * For usage exapmle see : \ref IfxLld_Eray_Eray_Usage
 *
 */
IFX_EXTERN void IfxEray_Eray_receiveFifoFrame(IfxEray_Eray *eray, IfxEray_Eray_ReceiveControl *config);

/** \brief Transfers header and data from message buffer to output buffer.
 * \param eray pointer to ERAY Module handle.
 * \param config pointer to receive control structure.
 * \return None
 *
 * For usage exapmle see : \ref IfxLld_Eray_Eray_Usage
 *
 */
IFX_EXTERN void IfxEray_Eray_receiveFrame(IfxEray_Eray *eray, IfxEray_Eray_ReceiveControl *config);

/** \brief Transfers the frame in given slot.
 * \param eray pointer to module handle.
 * \param transmitControl transmits frame in a given slot.
 * \return None
 *
 * For usage exapmle see : \ref IfxLld_Eray_Eray_Usage
 *
 */
IFX_EXTERN void IfxEray_Eray_transmitFrame(IfxEray_Eray *eray, IfxEray_Eray_TransmitControl *transmitControl);

/** \} */

/** \addtogroup IfxLld_Eray_Eray_Status
 * \{ */

/******************************************************************************/
/*-------------------------Inline Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief Gets the current node controller POC state.
 * \param eray pointer to module handle.
 * \return current POC state.
 */
IFX_INLINE IfxEray_PocState IfxEray_Eray_getPocState(IfxEray_Eray *eray);

/** \brief Gets the received wakeup pattern channel.
 * \param eray pointer to module handle.
 * \return received wakeup pattern channel.
 *
 * For usage exapmle see : \ref IfxLld_Eray_Eray_Usage
 *
 */
IFX_INLINE IfxEray_WakeupChannel IfxEray_Eray_getWakeupPatternReceivedChannel(IfxEray_Eray *eray);

/** \} */

/** \addtogroup IfxLld_Eray_Eray_Interrupt
 * \{ */

/******************************************************************************/
/*-------------------------Inline Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief clears the error interrupt flag requested.
 * \param eray pointer to module handle.
 * \param errorFlag error flag to be cleared.
 * \return None
 */
IFX_INLINE void IfxEray_Eray_clearErrorFlag(IfxEray_Eray *eray, IfxEray_ClearErrorFlag errorFlag);

/** \brief Clears the status interrupt flag requested.
 * \param eray pointer to module handle.
 * \param statusFlag status flag to be cleared.
 * \return None
 */
IFX_INLINE void IfxEray_Eray_clearStatusFlag(IfxEray_Eray *eray, IfxEray_ClearStatusFlag statusFlag);

/** \brief Gets the error interrupt status.
 * \param eray pointer to module handle.
 * \return error interrupt status.
 */
IFX_INLINE Ifx_ERAY_EIR IfxEray_Eray_getErrorInterrupts(IfxEray_Eray *eray);

/** \brief Gets the message buffer interrupt status.
 * \param eray pointer to module handle.
 * \param messageBuffer message buffer to which interrupt status be checked.
 * \return message buffer interrupt status.
 */
IFX_INLINE uint8 IfxEray_Eray_getMessageBufferInterruptStatus(IfxEray_Eray *eray, uint8 messageBuffer);

/** \brief Gets the new data interrupt buffers status.
 * \param eray pointer to module handle.
 * \param ndat ndat buffer to which interrupt status be checked.
 * \return ndat buffer interrupt status.
 */
IFX_INLINE uint8 IfxEray_Eray_getNewDataInterruptStatus(IfxEray_Eray *eray, uint8 ndat);

/** \brief Gets the node status interrupts.
 * \param eray pointer to module handle.
 * \return node status interrupts.
 */
IFX_INLINE Ifx_ERAY_SIR IfxEray_Eray_getStatusInterrupts(IfxEray_Eray *eray);

/** \brief Enables the message buffer interrupt line.
 * \param eray pointer to module handle.
 * \param messageBuffer message buffer interrupt which should be configured
 * \param messageBufferDestination selects MBSC0 or MBSC1 interrupt output
 * \return None
 *
 * For usage exapmle see : \ref IfxLld_Eray_Eray_Usage
 *
 */
IFX_INLINE void IfxEray_Eray_setMessageBufferInterruptDestination(IfxEray_Eray *eray, uint8 messageBuffer, uint8 messageBufferDestination);

/** \brief Enables the NDAT buffer interrupt line.
 * \param eray pointer to module handle.
 * \param ndat message buffer number configured to which ndat interrupt line to be set.
 * \param ndatDestination selects NDAT0 or NDAT1 interrupt output
 * \return None
 *
 * For usage exapmle see : \ref IfxLld_Eray_Eray_Usage
 *
 */
IFX_INLINE void IfxEray_Eray_setNewDataInterruptDestination(IfxEray_Eray *eray, uint8 ndat, uint8 ndatDestination);

/** \} */

/******************************************************************************/
/*---------------------Inline Function Implementations------------------------*/
/******************************************************************************/

IFX_INLINE boolean IfxEray_Eray_allowColdStart(IfxEray_Eray *eray)
{
    return IfxEray_changePocState(eray->eray, IfxEray_PocCommand_coldStart);
}


IFX_INLINE void IfxEray_Eray_bufferReconfig(IfxEray_Eray *eray, IfxEray_Eray_BufferReconfig *reconfigBuffer)
{
    IfxEray_setSlot(eray->eray, reconfigBuffer->header, reconfigBuffer->data, reconfigBuffer->slotControl);
}


IFX_INLINE boolean IfxEray_Eray_changePocState(IfxEray_Eray *eray, IfxEray_PocCommand PocCommand)
{
    boolean status = IfxEray_changePocState(eray->eray, PocCommand);
    return status;
}


IFX_INLINE void IfxEray_Eray_clearErrorFlag(IfxEray_Eray *eray, IfxEray_ClearErrorFlag errorFlag)
{
    IfxEray_clearErrorFlag(eray->eray, errorFlag);
}


IFX_INLINE void IfxEray_Eray_clearStatusFlag(IfxEray_Eray *eray, IfxEray_ClearStatusFlag statusFlag)
{
    IfxEray_clearStatusFlag(eray->eray, statusFlag);
}


IFX_INLINE void IfxEray_Eray_deInitModule(IfxEray_Eray *eray)
{
    //resets the kernel
    IfxEray_resetModule(eray->eray);
}


IFX_INLINE Ifx_ERAY_EIR IfxEray_Eray_getErrorInterrupts(IfxEray_Eray *eray)
{
    Ifx_ERAY_EIR interruptFlags;
    interruptFlags = IfxEray_getErrorInterrupts(eray->eray);
    return interruptFlags;
}


IFX_INLINE uint8 IfxEray_Eray_getMessageBufferInterruptStatus(IfxEray_Eray *eray, uint8 messageBuffer)
{
    uint8 messageBufferInterrupt = IfxEray_getMessageBufferInterruptStatus(eray->eray, messageBuffer);
    return messageBufferInterrupt;
}


IFX_INLINE uint8 IfxEray_Eray_getNewDataInterruptStatus(IfxEray_Eray *eray, uint8 ndat)
{
    uint8 ndatInterrupt = IfxEray_getNewDataInterruptStatus(eray->eray, ndat);
    return ndatInterrupt;
}


IFX_INLINE IfxEray_PocState IfxEray_Eray_getPocState(IfxEray_Eray *eray)
{
    return IfxEray_getPocState(eray->eray);
}


IFX_INLINE Ifx_ERAY_SIR IfxEray_Eray_getStatusInterrupts(IfxEray_Eray *eray)
{
    Ifx_ERAY_SIR statusInterrupts = IfxEray_getStatusInterrupts(eray->eray);
    return statusInterrupts;
}


IFX_INLINE IfxEray_WakeupChannel IfxEray_Eray_getWakeupPatternReceivedChannel(IfxEray_Eray *eray)
{
    IfxEray_WakeupChannel rxWakeupChannel;
    rxWakeupChannel = IfxEray_getWakeupPatternReceivedChannel(eray->eray);

    return rxWakeupChannel;
}


IFX_INLINE void IfxEray_Eray_readData(IfxEray_Eray *eray, uint32 *data, uint8 payloadLength)
{
    IfxEray_readData(eray->eray, data, payloadLength);
}


IFX_INLINE void IfxEray_Eray_readFrame(IfxEray_Eray *eray, IfxEray_Eray_ReceivedFrame *frame, Ifx_SizeT maxPayloadLength)
{
    IfxEray_readFrame(eray->eray, &(frame->header), frame->data, maxPayloadLength);
}


IFX_INLINE void IfxEray_Eray_setMessageBufferInterruptDestination(IfxEray_Eray *eray, uint8 messageBuffer, uint8 messageBufferDestination)
{
    IfxEray_setMessageBufferInterruptDestination(eray->eray, messageBuffer, messageBufferDestination);
}


IFX_INLINE void IfxEray_Eray_setNewDataInterruptDestination(IfxEray_Eray *eray, uint8 ndat, uint8 ndatDestination)
{
    IfxEray_setNewDataInterruptDestination(eray->eray, ndat, ndatDestination);
}


IFX_INLINE boolean IfxEray_Eray_setPocAllSlots(IfxEray_Eray *eray)
{
    return IfxEray_changePocState(eray->eray, IfxEray_PocCommand_allSlots);
}


IFX_INLINE boolean IfxEray_Eray_startCommunication(IfxEray_Eray *eray)
{
    return IfxEray_changePocState(eray->eray, IfxEray_PocCommand_run);
}


IFX_INLINE void IfxEray_Eray_wakeUpChannel(IfxEray_Eray *eray, IfxEray_Channel channel)
{
    Ifx_ERAY *eraySFR = eray->eray;
    boolean   result  = IfxEray_changePocState(eraySFR, IfxEray_PocCommand_config);

    if (result == TRUE)
    {
        eraySFR->SUCC1.B.WUCS = channel;
        IfxEray_setPocReady(eray->eray);
    }
}


IFX_INLINE boolean IfxEray_Eray_wakeUpCluster(IfxEray_Eray *eray)
{
    return IfxEray_changePocState(eray->eray, IfxEray_PocCommand_wakeup);
}


IFX_INLINE void IfxEray_Eray_writeData(IfxEray_Eray *eray, uint32 *data, uint8 payloadLength)
{
    IfxEray_writeData(eray->eray, data, payloadLength);
}


#endif /* IFXERAY_ERAY_H */
