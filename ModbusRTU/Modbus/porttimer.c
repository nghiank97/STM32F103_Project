/*
 * FreeModbus Libary: BARE Port
 * Copyright (C) 2006 Christian Walter <wolti@sil.at>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id: porttimer.c,v 1.1 2006/08/22 21:35:13 wolti Exp $
 */

/* ----------------------- Platform includes --------------------------------*/
#include "port.h"
 
/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"
 
/* ----------------------- static functions ---------------------------------*/
//static void prvvTIMERExpiredISR( void );
 
/* -----------------------    variables     ---------------------------------*/
extern TIM_HandleTypeDef htim2;
#define _htim htim2
uint16_t timeout = 0;
uint16_t downcounter = 0;
 
/* ----------------------- Start implementation -----------------------------*/
BOOL
xMBPortTimersInit( USHORT usTim1Timerout50us )
{
  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 71;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 49;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

  timeout = usTim1Timerout50us;

  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    return FALSE;
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
	  return FALSE;
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
	  return FALSE;
  }
  return TRUE;
}
 
 
void
vMBPortTimersEnable(  )
{
  /* Enable the timer with the timeout passed to xMBPortTimersInit( ) */
  downcounter = timeout;
  HAL_TIM_Base_Start_IT(&_htim);
}
 
void
vMBPortTimersDisable(  )
{
  /* Disable any pending timers. */
  HAL_TIM_Base_Stop_IT(&_htim);
}
 
/* Create an ISR which is called whenever the timer has expired. This function
* must then call pxMBPortCBTimerExpired( ) to notify the protocol stack that
* the timer has expired.

static void prvvTIMERExpiredISR(void)
{
	pxMBPortCBTimerExpired() ;
}

*/
