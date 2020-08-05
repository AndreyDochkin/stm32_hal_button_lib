#ifndef _BUTTON_H_
#define _BUTTON_H_
#ifdef __cplusplus
extern "C" {
#endif
#include <main.h>
#include <stdbool.h>


typedef enum{
	btn_1=0,
	btn_2
}Button_num;

enum{
	PULL_UP = true,
	PULL_DOWN = false
}

#define BUTTONS_NUMBER   50

#define DEBOUNCE_TIME   50
#define ON_PRESS        1
#define ON_PRESS_LONG   2
#define ON_RELEASE      3

void Button_init(Button_num num,GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint32_t timeHold, bool pullUP);
void Button_read(Button_num num);
bool Button_justPressed(Button_num num);
bool Button_justReleased(Button_num num);
bool Button_isHold(Button_num num);
bool Button_isClick(Button_num num);

#ifdef __cplusplus
}
#endif

#endif
