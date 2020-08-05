#include "stm32_button.h"
#include <stdbool.h>

typedef struct{
	GPIO_TypeDef* GPIOx;
	uint16_t GPIO_Pin;
    uint32_t _timeHold;
    bool _pullUP;
    bool _isClick;
    uint8_t _stateButton;
    uint8_t _buttonStateOld;
    bool _buttonStateWas;
    bool _buttonStateNowLong;
    uint32_t _msButtonState;
}Button_t;

Button_t button[BUTTONS_NUMBER]={{}};

void Button_init(Button_num num,GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint32_t timeHold, bool pullUP) {
	button[num].GPIOx = GPIOx;
	button[num].GPIO_Pin = GPIO_Pin;
	button[num]._pullUP = pullUP;
	button[num]._timeHold = timeHold;
	button[num]._msButtonState = 0;
	button[num]._buttonStateOld = ON_RELEASE;
	button[num]._isClick = false;
}

void Button_read(Button_num num) {
	button[num]._stateButton = false;
	button[num]._isClick = false;
    bool buttonStateNow = !HAL_GPIO_ReadPin(button[num].GPIOx,button[num].GPIO_Pin);
    if (!button[num]._pullUP) {
        buttonStateNow = !buttonStateNow;
    }
    if (buttonStateNow && buttonStateNow != button[num]._buttonStateWas && HAL_GetTick() - button[num]._msButtonState > DEBOUNCE_TIME) {
    	button[num]._buttonStateNowLong = false;
    	button[num]._msButtonState = HAL_GetTick();
    	button[num]._stateButton = ON_PRESS;
    	button[num]._buttonStateOld =  button[num]._stateButton;
    }
    if (!buttonStateNow && buttonStateNow != button[num]._buttonStateWas && HAL_GetTick() - button[num]._msButtonState > DEBOUNCE_TIME) {
    	button[num]._msButtonState = HAL_GetTick();
    	button[num]._stateButton = ON_RELEASE;
    }
    if (buttonStateNow && !button[num]._buttonStateNowLong && HAL_GetTick() - button[num]._msButtonState > button[num]._timeHold) {
    	button[num]._buttonStateNowLong = true;
    	button[num]._msButtonState = HAL_GetTick();
    	button[num]._stateButton = ON_PRESS_LONG;
    	button[num]._buttonStateOld =  button[num]._stateButton;
    }
    if (button[num]._stateButton == ON_RELEASE && button[num]._buttonStateOld == ON_PRESS ) {
    	button[num]._buttonStateOld = ON_RELEASE;
    	button[num]._isClick = true;
    }
    button[num]._buttonStateWas = buttonStateNow;
}

bool Button_justPressed(Button_num num) {
    return button[num]._stateButton == ON_PRESS ? 1 : 0;
}

bool Button_justReleased(Button_num num) {
    return button[num]._stateButton == ON_RELEASE ? 1 : 0;
}

bool Button_isHold(Button_num num) {
    return button[num]._stateButton == ON_PRESS_LONG ? 1 : 0;
}

bool Button_isClick(Button_num num) {
    return button[num]._isClick;
}
