/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "gpio.h"
#include "tim.h"
#include "../../User/display188.h"
#include "../../User/button.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
typedef struct mainModel
{
	int state;
	int gear;
	int power;
	int delay;
}mainModel_t;

mainModel_t model = 
{
	.state = 0,
};

static void sw0_down_callback(void *btn);
/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for display */
osThreadId_t displayHandle;
const osThreadAttr_t display_attributes = {
  .name = "display",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow1,
};
/* Definitions for gear */
osThreadId_t gearHandle;
const osThreadAttr_t gear_attributes = {
  .name = "gear",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for key */
osThreadId_t keyHandle;
const osThreadAttr_t key_attributes = {
  .name = "key",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void DisplayTask02(void *argument);
void GearTask03(void *argument);
void KeyTask04(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of display */
  displayHandle = osThreadNew(DisplayTask02, NULL, &display_attributes);

  /* creation of gear */
  gearHandle = osThreadNew(GearTask03, NULL, &gear_attributes);

  /* creation of key */
  keyHandle = osThreadNew(KeyTask04, NULL, &key_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_DisplayTask02 */
/**
* @brief Function implementing the display thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_DisplayTask02 */
void DisplayTask02(void *argument)
{
  /* USER CODE BEGIN DisplayTask02 */
	uint32_t tickFrequency = pdMS_TO_TICKS(20);
	volatile uint32_t tickCount;
	display188_var.LED1_DRIVER = set_PB3;
	display188_var.LED2_DRIVER = set_PB4;
	display188_var.LED3_DRIVER = set_PB5;
	display188_var.LED4_DRIVER = set_PB6;
	display188_var.LED5_DRIVER = set_PB7;
  /* Infinite loop */
  for(;;)
  {
		tickCount = osKernelGetTickCount();
		 display188_refresh_handle();
     osDelayUntil(tickCount + tickFrequency);
  }
  /* USER CODE END DisplayTask02 */
}

/* USER CODE BEGIN Header_GearTask03 */
/**
* @brief Function implementing the gear thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_GearTask03 */
void GearTask03(void *argument)
{
  /* USER CODE BEGIN GearTask03 */
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_3);
  /* Infinite loop */
  for(;;)
  {
		if(model.gear == 1)
		{
			
		}
		else if(model.gear == 2)
		{
		
		}
		osDelay(pdMS_TO_TICKS(20));
  }
  /* USER CODE END GearTask03 */
}

/* USER CODE BEGIN Header_KeyTask04 */
/**
* @brief Function implementing the key thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_KeyTask04 */
void KeyTask04(void *argument)
{
  /* USER CODE BEGIN KeyTask04 */
  Button_Create("sw0",&sw0,get_PA15);
	sw0.Button_Trigger_Level = 0;
  Button_Attach(&sw0,BUTTON_DOWN,sw0_down_callback);
  /* Infinite loop */
  for(;;)
  {
		Button_Process();
    osDelay(pdMS_TO_TICKS(20));
  }
  /* USER CODE END KeyTask04 */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
static void sw0_down_callback(void *btn)
{



}
/* USER CODE END Application */
