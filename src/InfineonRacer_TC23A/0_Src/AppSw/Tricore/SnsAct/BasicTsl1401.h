/**
 * \file BasicTsl1401.h
 * \brief BasicTsl1401
 *
 * \version iLLD_Demos_1_0_0_11_0
 *
 * \defgroup BasicTsl1401
 * \ingroup BasicTsl1401
 */

#ifndef BASICTSL1401_H
#define BASICTSL1401_H 1

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/
#include <Vadc/Std/IfxVadc.h>
#include <Vadc/Adc/IfxVadc_Adc.h>

/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*--------------------------------Enumerations--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-----------------------------Data Structures--------------------------------*/
/******************************************************************************/
typedef struct{
	uint32 adcResult[2][128];
}IR_LineScan_t;

/******************************************************************************/
/*------------------------------Global variables------------------------------*/
/******************************************************************************/
IFX_EXTERN IR_LineScan_t IR_LineScan;

/******************************************************************************/
/*-------------------------Function Prototypes--------------------------------*/
/******************************************************************************/
IFX_EXTERN void BasicLineScan_init(void);
IFX_EXTERN void BasicLineScan_run(void);

#endif
