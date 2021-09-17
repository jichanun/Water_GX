#include "bsp_tim.h"
#include "BSPconfig.h"
#include "stm32f4xx_hal.h"
u32 TIM3DMAMemoryBaseAddress,TIM3DMABufferSize;

void UserTim1Config(void)//摩擦轮
{
	//TIM1-CH2 CONFIG BSGIN
	LL_TIM_OC_InitTypeDef TIM_OC_InitStruct = {0};
	LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	TIM_OC_InitStruct.OCMode = LL_TIM_OCMODE_PWM1;
  TIM_OC_InitStruct.OCState = LL_TIM_OCSTATE_DISABLE;
  TIM_OC_InitStruct.OCNState = LL_TIM_OCSTATE_DISABLE;
  TIM_OC_InitStruct.CompareValue = 999;
  TIM_OC_InitStruct.OCPolarity = LL_TIM_OCPOLARITY_HIGH;
  TIM_OC_InitStruct.OCNPolarity = LL_TIM_OCPOLARITY_HIGH;
  TIM_OC_InitStruct.OCIdleState = LL_TIM_OCIDLESTATE_LOW;
  TIM_OC_InitStruct.OCNIdleState = LL_TIM_OCIDLESTATE_LOW;
	
	LL_TIM_OC_Init(TIM1, LL_TIM_CHANNEL_CH2, &TIM_OC_InitStruct);
	LL_TIM_OC_EnablePreload(TIM1, LL_TIM_CHANNEL_CH2);
	LL_TIM_OC_DisableFast(TIM1, LL_TIM_CHANNEL_CH2);
	
	GPIO_InitStruct.Pin = LL_GPIO_PIN_9;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_1;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	//TIM1-CH2 CONFIG END
	
	TIM1->CCER |=TIM_CCER_CC1E ;
	TIM1->CCER |=TIM_CCER_CC2E ;
	TIM1->CCER |=TIM_CCER_CC3E ;
	TIM1->CCER |=TIM_CCER_CC4E ;
	
	LL_TIM_EnableARRPreload(TIM1);
	LL_TIM_EnableAllOutputs(TIM1);
	
	LL_TIM_EnableCounter(TIM1); 
}
void ConfigTIM3DMA(u32 DMA_Memory0BaseAddr,u32 DMA_BufferSize)
{
	TIM3DMAMemoryBaseAddress=DMA_Memory0BaseAddr;
	TIM3DMABufferSize=DMA_BufferSize;
	
	TIM3ConfigEnable();
}

void TIM3ConfigEnable(void)
{
	LL_DMA_SetMemoryAddress(DMA1,LL_DMA_STREAM_4,(u32)TIM3DMAMemoryBaseAddress);
	LL_DMA_SetPeriphAddress(DMA1,LL_DMA_STREAM_4,(u32)&(TIM3->CCR1));
	LL_DMA_SetDataLength(DMA1,LL_DMA_STREAM_4,TIM3DMABufferSize);	
	LL_DMA_ClearFlag_TC7(DMA1);//清除中断标志
	LL_DMA_EnableIT_TC(DMA1, LL_DMA_STREAM_4);
	LL_TIM_EnableDMAReq_CC1(TIM3);//使能TIM1的CC3 DMA请求
	LL_TIM_EnableAllOutputs(TIM3);//使能TIM的输出
	LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_4);
	
	LL_TIM_CC_SetDMAReqTrigger(TIM3,LL_TIM_CCDMAREQUEST_CC);//设置TIM1 DMA请求触发器	
	LL_TIM_CC_EnableChannel(TIM3,LL_TIM_CHANNEL_CH1);//使能TIM1 的cc通道ch3	

}


void UserTim3Config(void)
{
	LL_TIM_EnableIT_UPDATE(TIM3);
	LL_TIM_EnableCounter(TIM3);
}

void UserTim8Config(void)
{
	TIM8->CCER |=TIM_CCER_CC3E ;
	TIM8->CCER |=TIM_CCER_CC4E ;

	LL_TIM_EnableARRPreload(TIM8);
	LL_TIM_EnableAllOutputs(TIM8);

	LL_TIM_EnableCounter(TIM8);

}
void USER_TIM1_2_Init(void)
{
  LL_TIM_InitTypeDef TIM_InitStruct = {0};
  LL_TIM_OC_InitTypeDef TIM_OC_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* Peripheral clock enable */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM1);
  /* TIM3 DMA Init */
  
  /* TIM3_CH1_TRIG Init */
  LL_DMA_SetChannelSelection(DMA2, LL_DMA_STREAM_6, LL_DMA_CHANNEL_0);

  LL_DMA_SetDataTransferDirection(DMA2, LL_DMA_STREAM_6, LL_DMA_DIRECTION_MEMORY_TO_PERIPH);

  LL_DMA_SetStreamPriorityLevel(DMA2, LL_DMA_STREAM_6, LL_DMA_PRIORITY_LOW);

  LL_DMA_SetMode(DMA2, LL_DMA_STREAM_6, LL_DMA_MODE_NORMAL);

  LL_DMA_SetPeriphIncMode(DMA2, LL_DMA_STREAM_6, LL_DMA_PERIPH_NOINCREMENT);

  LL_DMA_SetMemoryIncMode(DMA2, LL_DMA_STREAM_6, LL_DMA_MEMORY_INCREMENT);

  LL_DMA_SetPeriphSize(DMA2, LL_DMA_STREAM_6, LL_DMA_PDATAALIGN_WORD);

  LL_DMA_SetMemorySize(DMA2, LL_DMA_STREAM_6, LL_DMA_MDATAALIGN_WORD);

  LL_DMA_DisableFifoMode(DMA2, LL_DMA_STREAM_6);

  TIM_InitStruct.Prescaler = 0;
  TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
  TIM_InitStruct.Autoreload = 104;
  TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
  //LL_TIM_Init(TIM3, &TIM_InitStruct);
  LL_TIM_SetClockSource(TIM1, LL_TIM_CLOCKSOURCE_INTERNAL);
  LL_TIM_OC_EnablePreload(TIM1, LL_TIM_CHANNEL_CH2);
  TIM_OC_InitStruct.OCMode = LL_TIM_OCMODE_PWM1;
  TIM_OC_InitStruct.OCState = LL_TIM_OCSTATE_DISABLE;
  TIM_OC_InitStruct.OCNState = LL_TIM_OCSTATE_DISABLE;
  TIM_OC_InitStruct.CompareValue = 0;
  TIM_OC_InitStruct.OCPolarity = LL_TIM_OCPOLARITY_HIGH;
  LL_TIM_OC_Init(TIM1, LL_TIM_CHANNEL_CH2, &TIM_OC_InitStruct);
  LL_TIM_OC_DisableFast(TIM1, LL_TIM_CHANNEL_CH2);
  LL_TIM_SetTriggerOutput(TIM1, LL_TIM_TRGO_RESET);
  LL_TIM_DisableMasterSlaveMode(TIM1);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
	  //LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM3);

    /**TIM3 GPIO Configuration    
    PA9     ------> TIM3_CH1 
    */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_9;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_1;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

