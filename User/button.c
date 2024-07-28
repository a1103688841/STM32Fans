/******************************************
 * @Author:RNF
 * @Date:2024-07-28 00:24:30
 * @LastEditTime:2024-07-28 15:27:54
 * @LastEditors:XG
 * @Description:
 * @FilePath:\STM32Fans\User\button.c
 * @桃之夭夭
 ******************************************/
/**************************************************************************************************
* fuction	:                              button     
* file		:  button.c     
* brief		:     
* author	: rinanfang  3/2/2020
**************************************************************************************************/

#define __BUTTON_C__

/* ------------------------------------------- include ----------------------------------------- */
#include "button.h"


/* ------------------------------------- define/typedef/enum ----------------------------------- */


/* ------------------------------------------- variable ---------------------------------------- */
//Button's head
static struct button* Head_Button = NULL;





/* -------------------------------------------static funtion ----------------------------------------- */
//Name str copy
static char *StrnCopy(char *dst, const char *src, unsigned int n);
static void Button_Cycle_Process (button_t *btn);

//read sw level || sw event callback
////需要修改的函数声明
// static uint8_t 	Read_SW0_Level (void);

/* -------------------------------------------global funtion ----------------------------------------- */
//main button function
// extern void Button_Attach_ini (void);

/* -------------------------------------------modify funtion ----------------------------------------- */
//需要修改的函数


/* --------------------ini button node and attach button event callbak to button----------------------------------------- */
//按键链接初始化
// void Button_Attach_ini(void)
// {

//  	// Button_Create("SW0",&SW0,Read_SW0_Level);
// 	// Button_Attach(&SW0,BUTTON_DOWN,SW0_Down_CallBack);                       //call fuction attach
// 	// Button_Attach(&SW0,BUTTON_LONG,SW0_Long_CallBack);                       //call fuction attach
	
// }
// /* -------------------------------------------sw level ----------------------------------------- */
// //修改电平
// /* ----------------------------------------------------------------------------------------------------- */
// static uint8_t Read_SW0_Level(void)
// {
// 	return (GetSW0)?TRUE:FALSE;
// }

// /**************************************************************************************************
// * 函数:                                   SW0_Down_CallBack
// * 说明:  
// * 输入:  
// *		   
// * 关联变量: 
// * 返回:  
// * 注释作者:  日南方  1/25/2021
// **************************************************************************************************/
void SW0_Down_CallBack(void *btn)
{



}
/* ------------------------------------------- funtion ----------------------------------------- */
//内部函数
/* ------------------------------------------- funtion ----------------------------------------- */
/************************************************************
  * @brief   按键周期处理函数
  * @param   btn:处理的按键
  * @return  NULL
  * @author  jiejie
  * @github  https://github.com/jiejieTop
  * @date    2018-xx-xx
  * @version v1.0
  * @note    必须以一定周期调用此函数，建议周期为20~50ms
  ***********************************************************/
void Button_Cycle_Process(button_t *btn)
{
  uint8_t current_level = (uint8_t)btn->Read_Button_Level();
  //按键电平发生变化，消抖
    if((current_level != btn->Button_Last_Level)&&(++(btn->level_timercnt) > BUTTON_LEVEL_STABLE)) 
    {
        btn->Button_Last_Level = current_level; //更新当前按键电平
        btn->level_timercnt = 0;                 //确定了是按下
          
         //如果按键是没被按下的，改变按键状态为按下(首次按下/双击按下)
        if(btn->Button_State == NONE_TRIGGER && btn->Button_Last_Level == btn->Button_Trigger_Level)
        {
            btn->Button_State = BUTTON_DOWN;
        }
        //释放按键
        else if(btn->Button_State == BUTTON_DOWN)
        {
            btn->Button_State = BUTTON_UP;
        }
        //双击检测
   #if SINGLE_AND_DOUBLE_TRIGGER == 1
        else if(btn->Button_State == BUTTON_DOUBLE_WAIT)
        {
            btn->Button_State = BUTTON_DOWN;
        }
    #endif
    }
    else if(current_level == btn->Button_Last_Level)
    {
        //重置消抖寄存器
        btn->level_timercnt = 0;
    }
  
  switch(btn->Button_State)
  {
    case BUTTON_DOWN :            // 按下状态
    {
	    //按键按下电平和我们设置电平相一致时
        if(btn->Button_Last_Level == btn->Button_Trigger_Level) 
        {
            //连按
            #if CONTINUOS_TRIGGER     

            if(++(btn->cycle_timercnt) >= BUTTON_CONTINUOS_CYCLE)
            {
              btn->cycle_timercnt = 0;
              btn->Button_Trigger_Event = BUTTON_CONTINUOS; 
              TRIGGER_CB(BUTTON_CONTINUOS);    //连按
              //PRINT_DEBUG("连按");
            }
            
            #else	    
    	    //按下定时器计数
    	    btn->down_timecnt++;
    	    
    	    //单击
            if (btn->down_timecnt < BUTTON_LONG_TIME && btn->Button_Trigger_Event != BUTTON_DOUBLE_WAIT)
            {
                btn->Button_Trigger_Event = BUTTON_DOWN; 
            }
    	    //长按释放
            #if LONG_FREE_TRIGGER == 1
            else if(btn->Long_Time == BUTTON_LONG_TIME)     
            {
                btn->Button_Trigger_Event = BUTTON_LONG_FREE; 
            } 
            #else
    	    //长按
            else if(btn->down_timecnt == BUTTON_LONG_TIME)      //释放按键前更新触发事件为长按
            {
                btn->Button_Trigger_Event = BUTTON_LONG; 
                TRIGGER_CB(BUTTON_LONG);                //第一次长按
            }
            
            //长按后
            else if(btn->down_timecnt > BUTTON_LONG_TIME)
            {
                  if(++(btn->cycle_timercnt) >= BUTTON_LONG_CYCLE)    //连续触发长按的周期
                  {
                    btn->cycle_timercnt = 0;
                    btn->Button_Trigger_Event = BUTTON_LONG; 
                    TRIGGER_CB(BUTTON_LONG);                        //长按连续触发
                  }
             }
            #endif
            //更新按下计数器时间  
            if(btn->down_timecnt == 0xFF)  
            {
                //更新长按后
                btn->down_timecnt = BUTTON_LONG_TIME+1;
            }
        }
          
      #endif
    

      break;
    } 
    
    case BUTTON_UP :        // 弹起状态
    {
            //触发单击
            if(btn->Button_Trigger_Event == BUTTON_DOWN)  
            {
                //单击
            #if (SINGLE_AND_DOUBLE_TRIGGER == 0)
                TRIGGER_CB(BUTTON_DOWN);    
                btn->down_timecnt=0;
                btn->Button_State = NONE_TRIGGER;
                btn->Button_Trigger_Event = NONE_TRIGGER;
            #else
                //双击检测
                if(btn->Button_Trigger_Event = BUTTON_DOWN)
                {
                    btn->down_timecnt=0;
                    btn->cycle_timercnt = 0;
                    btn->double_timecnt = 0;
                    btn->Button_State = BUTTON_DOUBLE_WAIT;
                    btn->Button_Trigger_Event = BUTTON_DOUBLE_WAIT;
                }
                esle if(btn->Button_Trigger_Event = BUTTON_DOUBLE_WAIT)
                {
                    //双击检测
                    btn->down_timecnt=0;
                    btn->cycle_timercnt = 0;
                    btn->double_timecnt = 0;
                    TRIGGER_CB(BUTTON_DOUBLE); 
                    btn->Button_Last_State = BUTTON_DOUBLE;
                    btn->Button_State = NONE_TRIGGER;
                    btn->Button_Trigger_Event = NONE_TRIGGER;
                }
            #endif 
            }
            else if(btn->Button_Trigger_Event == BUTTON_CONTINUOS)
            {
                btn->down_timecnt = 0;
                btn->cycle_timercnt = 0;
                //在按下那边已经触发了
                btn->Button_State = NONE_TRIGGER;
                btn->Button_Trigger_Event = NONE_TRIGGER;

            }
            //长按
            else if(btn->Button_Trigger_Event == BUTTON_LONG)
            {
                TRIGGER_CB(BUTTON_LONG_FREE);
                btn->down_timecnt = 0;
                btn->cycle_timercnt = 0;
                //在按下那边已经触发了
                btn->Button_State = NONE_TRIGGER;
                btn->Button_Trigger_Event = NONE_TRIGGER;
            }
            #if LONG_FREE_TRIGGER == 1
            //长按释放
            else if(btn->Button_Trigger_Event == BUTTON_LONG_FREE)
            {
                TRIGGER_CB(BUTTON_LONG_FREE);
                btn->down_timecnt = 0;
                btn->cycle_timercnt = 0;
                //在按下那边已经触发了
                btn->Button_Last_State = BUTTON_LONG_FREE;
                btn->Button_State = NONE_TRIGGER;
                btn->Button_Trigger_Event = NONE_TRIGGER;
            }
            #endif
            #if CONTINUOS_TRIGGER
            else if(btn->Button_Trigger_Event == BUTTON_CONTINUOS)  //连按
            {
              btn->Long_Time = 0;
              TRIGGER_CB(BUTTON_CONTINUOS_FREE);    //连发释放
              btn->Button_State = NONE_TRIGGER;
              btn->Button_Last_State = BUTTON_CONTINUOS;
            } 
            #endif
      
      break;
    }
    //双击检测
    #if (SINGLE_AND_DOUBLE_TRIGGER == 1)
    case BUTTON_DOUBLE_WAIT :		//双击中间间隔的超时判断
    {
        btn->double_timecnt++;     //时间记录 
        //超时
        if(btn->double_timecnt>=BUTTON_DOUBLE_TIME)
        {
            btn->double_timecnt = 0;
            btn->Button_Trigger_Event = BUTTON_DOWN;
            TRIGGER_CB(BUTTON_DOWN);
            btn->Button_State = NONE_TRIGGER;
            btn->Button_Trigger_Event = NONE_TRIGGER;
        }
      break;
    }
    #endif

    
    default :
      break;
  }

}

/**************************************************************************************************
* function	:                                   Button_Process
* brief	:  			   scan each button and each button's each button event
* param	:  			   none
* return			:  none
* author			:  rinanfang  3/2/2020
**************************************************************************************************/
void Button_Process(void)
{
  struct button* pass_btn;
  for(pass_btn = Head_Button; pass_btn != NULL; pass_btn = pass_btn->Next)
  {
      Button_Cycle_Process(pass_btn);
  }
}

/**************************************************************************************************
* function	:                                   Add_Button
* brief	:   add button node to button linear list
* param	:  button node
* return			:  none
* author			:  rinanfang  3/2/2020
**************************************************************************************************/
void Add_Button(button_t* btn)
{
  struct button *pass_btn = Head_Button;
  
  while(pass_btn)
  {
    pass_btn = pass_btn->Next;
  }
  
  btn->Next = Head_Button;
  Head_Button = btn;
}

/**************************************************************************************************
* function	:                                   Button_Delete
* brief	:  delete button node from button linear list
* param	:  button node
* return			:  none
* author			:  rinanfang  3/2/2020
**************************************************************************************************/
void Button_Delete(button_t *btn)
{
  struct button** curr;
  for(curr = &Head_Button; *curr;) 
  {
    struct button* entry = *curr;
    if (entry == btn) 
    {
      *curr = entry->Next;
    } 
    else
    {
      curr = &entry->Next;
    }
  }
}
/**************************************************************************************************
* function	:                                   Button_Create
* brief	:  create a new button nod and add it to button linear list
* param	:  button name||button node||button level||button trigger level		
* return			:  TRUE FALSE
* author			:  rinanfang  3/2/2020
**************************************************************************************************/
 boolean_t Button_Create(const char *name,
                  button_t *btn, 
                  uint8_t(*read_btn_level)(void))
{
  if( btn == NULL)
  {
	  return FALSE;
  }
  //"string.h"
  memset(btn, 0, sizeof(struct button));  //
  StrnCopy(btn->Name, name, BTN_NAME_MAX);
  
  //set button param to no trigger status
  btn->Button_State = NONE_TRIGGER;           //按键状态
  btn->Button_Trigger_Event = NONE_TRIGGER;   //按键触发事件
  btn->Read_Button_Level = read_btn_level;    //按键读电平函数
//	  btn->Button_Trigger_Level = HIGHT;  //按键触发电平
//	  btn->Button_Last_Level = LOW; 			//按键当前电平
  btn->Button_Trigger_Level = HIGHT;  //按键触发电平
  btn->Button_Last_Level = btn->Read_Button_Level(); //按键当前电平
	//debug_printf("btn:%d",btn->Button_Last_Level);
  btn->level_timercnt = 0;
	
  //add to line tabe
  Add_Button(btn);          //add button line
  
  return TRUE;
}
/**************************************************************************************************
* function	:                                   Button_Attach
* brief	:		attach button event to  corresponding button node
* param	:  		button node||button event's array idex||button event's callback
* return			:  TRUE FALSE
* author			:  rinanfang  3/2/2020
**************************************************************************************************/
 boolean_t Button_Attach(button_t *btn,Button_Event btn_event,Button_CallBack btn_callback)
{
  uint8_t i=0;
  if( btn == NULL)
  {
	  return FALSE;
  }
  
  if(BUTTON_ALL_RIGGER == btn_event)
  {
    for( i = 0 ; i < number_of_event-1 ; i++)
      btn->CallBack_Function[i] = btn_callback; //
  }
  else
  {
    btn->CallBack_Function[btn_event] = btn_callback; //
  }
  return TRUE;
}

/**************************************************************************************************
* function	:                                   StrnCopy
* brief	:  
* param	:  dst str || src str || str line
* return			:  dst str 
* author			:  rinanfang  3/2/2020
**************************************************************************************************/
static char *StrnCopy(char *dst, const char *src, uint32_t n)
{
  if (n != 0)
  {
    char *d = dst;
    const char *s = src;
    do
    {
        if ((*d++ = *s++) == 0)
        {
            while (--n != 0)
                *d++ = 0;
            break;
        }
    } while (--n != 0);
  }
  return (dst);
}



