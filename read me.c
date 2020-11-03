此程序调试过，能够使电机运行。

解释：
  HAL的库的调用：

HAL_TIM_Base_Init这个已经在CubeMX自动生成和调用了，不用再管了。

HAL_TIM_Base_Start_IT需要自己写代码启动中断模式

HAL_TIM_Base_Start_IT(tim_baseHandle);
HAL_TIM_Base_Start(tim_baseHandle);


另外计数器溢出回调函数需要自己添加HAL_TIM_PeriodElapsedCallback