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
#include "../../User/button.h"
#include "../../User/display188.h"
#include "gpio.h"
#include "tim.h"
#include "adc.h"
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
		bool display_gear;
    int power_percent;
    int delay;
		unsigned int adc;
		float power_v;
} mainModel_t;

mainModel_t model = {
    .state = 0,
};

static void sw0_down_callback(void* btn);
/* USER CODE END Variables */
osThreadId powerHandle;
uint32_t powerBuffer[ 128 ];
osStaticThreadDef_t powerControlBlock;
osThreadId displayHandle;
uint32_t displayBuffer[ 128 ];
osStaticThreadDef_t displayControlBlock;
osThreadId gearHandle;
uint32_t gearBuffer[ 128 ];
osStaticThreadDef_t gearControlBlock;
osThreadId keyHandle;
uint32_t keyBuffer[ 128 ];
osStaticThreadDef_t keyControlBlock;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void PowerTask(void const * argument);
void DisplayTask02(void const * argument);
void GearTask03(void const * argument);
void KeyTask04(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

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
  /* definition and creation of power */
  osThreadStaticDef(power, PowerTask, osPriorityNormal, 0, 128, powerBuffer, &powerControlBlock);
  powerHandle = osThreadCreate(osThread(power), NULL);

  /* definition and creation of display */
  osThreadStaticDef(display, DisplayTask02, osPriorityLow, 0, 128, displayBuffer, &displayControlBlock);
  displayHandle = osThreadCreate(osThread(display), NULL);

  /* definition and creation of gear */
  osThreadStaticDef(gear, GearTask03, osPriorityLow, 0, 128, gearBuffer, &gearControlBlock);
  gearHandle = osThreadCreate(osThread(gear), NULL);

  /* definition and creation of key */
  osThreadStaticDef(key, KeyTask04, osPriorityLow, 0, 128, keyBuffer, &keyControlBlock);
  keyHandle = osThreadCreate(osThread(key), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
    /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_PowerTask */
/**
  * @brief  Function implementing the power thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_PowerTask */
void PowerTask(void const * argument)
{
  /* USER CODE BEGIN PowerTask */
	HAL_ADC_Start_DMA(&hadc1,(unsigned int *)&model.adc, 1);
  /* Infinite loop */
  for(;;)
  {
		model.power_v = model.adc/66535.0*3.3*10;
		model.power_percent = model.power_v / 21.0 * 100;
    osDelay(20);
  }
  /* USER CODE END PowerTask */
}

/* USER CODE BEGIN Header_DisplayTask02 */
/**
 * @brief Function implementing the display thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_DisplayTask02 */
void DisplayTask02(void const * argument)
{
  /* USER CODE BEGIN DisplayTask02 */
    uint32_t          tickFrequency = pdMS_TO_TICKS(20);
    volatile uint32_t tickCount;
    display188_var.LED1_DRIVER = set_PB3;
    display188_var.LED2_DRIVER = set_PB4;
    display188_var.LED3_DRIVER = set_PB5;
    display188_var.LED4_DRIVER = set_PB6;
    display188_var.LED5_DRIVER = set_PB7;
    /* Infinite loop */
    for (;;)
    {
        tickCount = osKernelGetTickCount();
        if(model.display_gear == true)
				{
					model.delay ++ ;
					display188_var.digit_all = model.display_gear;
				}
				else
				{
					display188_var.digit_all = model.power_percent;
					
				}
				if(model.delay > 3000/20)
				{
					model.display_gear = false;
				}
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
void GearTask03(void const * argument)
{
  /* USER CODE BEGIN GearTask03 */
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
    /* Infinite loop */
    for (;;)
    {
        if (model.gear == 1)
        {
            __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 100);
            __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 100);
        }
        else if (model.gear == 2)
        {
            __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 200);
            __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 200);
        }
        else if (model.gear == 3)
        {
            __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 300);
            __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 300);
        }
        else if (model.gear == 4)
        {
            __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 400);
            __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 400);
        }
        else if (model.gear == 5)
        {
            __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 500);
            __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 500);
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
void KeyTask04(void const * argument)
{
  /* USER CODE BEGIN KeyTask04 */
    Button_Create("sw0", &sw0, get_PA15);
    sw0.Button_Trigger_Level = 0;
    Button_Attach(&sw0, BUTTON_DOWN, sw0_down_callback);
    /* Infinite loop */
    for (;;)
    {
        Button_Process();
        osDelay(pdMS_TO_TICKS(20));
    }
  /* USER CODE END KeyTask04 */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
static void sw0_down_callback(void* btn) 
{
	model.gear++;
	if(model.gear > 5)
	{
		model.gear = 0;
	}
	model.display_gear = true;
	model.delay = 0;
	
}
/* USER CODE END Application */

