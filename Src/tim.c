/**
  ******************************************************************************
  * File Name          : TIM.c
  * Description        : This file provides code for the configuration
  *                      of the TIM instances.
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2019 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "tim.h"

/* USER CODE BEGIN 0 */
uint8_t  mdirection = stepmotor_direction ;

uint16_t  mcircle = stepmotor_circle  ;
uint8_t   mspeed= stepmotor_speed  ;
//uint8_t step = stepmotor_step;
/* USER CODE END 0 */

TIM_HandleTypeDef htim3;

/* TIM3 init function */
void MX_TIM3_Init(void)
{
  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;

  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 15;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 99;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* tim_baseHandle)
{

  if(tim_baseHandle->Instance==TIM3)
  {
  /* USER CODE BEGIN TIM3_MspInit 0 */

  /* USER CODE END TIM3_MspInit 0 */
    /* TIM3 clock enable */
    __HAL_RCC_TIM3_CLK_ENABLE();

    /* TIM3 interrupt Init */
    HAL_NVIC_SetPriority(TIM3_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM3_IRQn);
  /* USER CODE BEGIN TIM3_MspInit 1 */

  /* USER CODE END TIM3_MspInit 1 */
  }
}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* tim_baseHandle)
{

  if(tim_baseHandle->Instance==TIM3)
  {
  /* USER CODE BEGIN TIM3_MspDeInit 0 */

  /* USER CODE END TIM3_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM3_CLK_DISABLE();

    /* TIM3 interrupt Deinit */
    HAL_NVIC_DisableIRQ(TIM3_IRQn);
  /* USER CODE BEGIN TIM3_MspDeInit 1 */

  /* USER CODE END TIM3_MspDeInit 1 */
  }
} 

/* USER CODE BEGIN 1 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  static uint8_t  speed_count = 0; 
	static uint16_t pulse_count = 0;
	static uint8_t  step_count = stepmotor_step;
	static uint16_t i = 0;	
	
	//step_count
  if(htim==&htim3)	
    {
			 i++;
		 if(mcircle)
 	    {
				speed_count++;
				if(speed_count == mspeed)
				{
					if(mdirection) stepmotor_FWDpulse( );
 
					else stepmotor_REVpulse( );
					
					pulse_count++;

					speed_count=0;
		    }
	    	if(pulse_count>=4096)
		     {
					pulse_count=0;
					mcircle--;
			
		      }
			}
	    else 
				 {
					 A_OFF ; B_OFF ;C_OFF;  D_OFF;  

					
				 }		 
	   
   
		if(i>=8000)
			{
				HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_5);
				i = 0;
			}		 
   }	
}

static void stepmotor_FWDpulse( )  //FWD ,forward-rotating
{
   static uint8_t temp ;
					
			switch(temp)
				{
					case 0:		A_ON  ; B_OFF ;C_OFF; D_OFF; break;
					case 1:		A_ON  ; B_ON  ;C_OFF; D_OFF; break;	
					case 2:		A_OFF ; B_ON  ;C_OFF; D_OFF; break;	
					case 3:		A_OFF ; B_ON  ;C_ON;  D_OFF; break;
					case 4:		A_OFF ; B_OFF ;C_ON;  D_OFF; break;	
					case 5:		A_OFF ; B_OFF ;C_ON;  D_ON;  break;	
					case 6:		A_OFF ; B_OFF ;C_OFF; D_ON;  break;
					case 7:		A_ON  ; B_OFF ;C_OFF;  D_ON;  break;	
					default:  break;
				}
			temp++;
			if(temp==8) temp=0;
	}
static void stepmotor_REVpulse(  )  //REV ,reversal 
{
		static	uint8_t temp = 8;
	      temp--;
	   switch(temp)
				{
					case 0:		A_ON  ; B_OFF ;C_OFF; D_OFF; break;
					case 1:		A_ON  ; B_ON  ;C_OFF; D_OFF; break;	
					case 2:		A_OFF ; B_ON  ;C_OFF; D_OFF; break;	
					case 3:		A_OFF ; B_ON  ;C_ON;  D_OFF; break;
					case 4:		A_OFF ; B_OFF ;C_ON;  D_OFF; break;	
					case 5:		A_OFF ; B_OFF ;C_ON;  D_ON;  break;	
					case 6:		A_OFF ; B_OFF ;C_OFF; D_ON;  break;
					case 7:		A_ON  ; B_OFF ;C_OFF;  D_ON;  break;	
					default:  break;
		  	}
	 
				if(temp==0) temp=8;

}

void delay_ms(__IO uint32_t  nms)
{
 __IO uint32_t temp;
 SysTick->LOAD = 9000*nms;
 SysTick->VAL=0X00;//?????
 SysTick->CTRL=0X01;//??,???????,???????
 do
 {
  temp=SysTick->CTRL;//????????
 }while((temp&0x01)&&(!(temp&(1<<16))));//??????
    SysTick->CTRL=0x00; //?????
    SysTick->VAL =0X00; //?????
}
void Delay(__IO uint32_t nCount)
{
 for(; nCount != 0; nCount--);
	
}
/* USER CODE END 1 */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
