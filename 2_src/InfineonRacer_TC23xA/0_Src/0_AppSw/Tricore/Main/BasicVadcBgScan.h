/**
 * \file BasicVadcBgScan.h
 * \brief  BasicVadcBgScan
 *
 * \version iLLD_Demos_1_0_0_11_0
 *
 * \defgroup BasicVadcBgScan Demo Source
 * \ingroup BasicVadcBgScan
 */

#ifndef BASICVADCBGSCAN_H
#define BASICVADCBGSCAN_H 1

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/

#include <Vadc/Std/IfxVadc.h>
#include <Vadc/Adc/IfxVadc_Adc.h>
/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/
#define USR_ADC_CHN_15_IDX		0
#define USR_ADC_CHN_16_IDX		1
#define USR_ADC_CHN_20_IDX		2
#define USR_ADC_CHN_21_IDX		3

#define IR_getChn15()	IR_AdcResult[USR_ADC_CHN_15_IDX]
#define IR_getChn16()	IR_AdcResult[USR_ADC_CHN_16_IDX]
#define IR_getChn20()	IR_AdcResult[USR_ADC_CHN_20_IDX]
#define IR_getChn21()	IR_AdcResult[USR_ADC_CHN_21_IDX]
/******************************************************************************/
/*--------------------------------Enumerations--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-----------------------------Data Structures--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*------------------------------Global variables------------------------------*/
/******************************************************************************/
IFX_EXTERN float32 IR_AdcResult[];

	/* Usage: IR_AdcResult[USR_ADC_CHN_15_IDX] */

/******************************************************************************/
/*-------------------------Function Prototypes--------------------------------*/
/******************************************************************************/
IFX_EXTERN void BasicVadcBgScan_init(void);
IFX_EXTERN void BasicVadcBgScan_run(void);

#endif
