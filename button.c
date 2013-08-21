#include <stdio.h>

#include "gpio.h"

#include "button.h"

#define BTN_MASK 0x1F

/**
 * @brief Convert a raw value to a button enum value.
 */
static Button raw_to_button(uint8_t btn)
{
    switch (btn) {
        case 1:
            return Select;
        case 2:
            return Right;
        case 4:
            return Down;
        case 8:
            return Up;
        case 16:
            return Left;
        default:
            return Null;
    }
}

uint8_t btn_nblk_raw()
{
    return GPIO_read(PortA) & BTN_MASK;
}

Button btn_nblk()
{
    return raw_to_button(btn_nblk_raw());
}

uint8_t btn_blk_raw()
{
    uint8_t buf;
    while( (buf = btn_nblk_raw()) == 0 )
        ;
    return buf;
}

Button btn_blk()
{
    return raw_to_button(btn_blk_raw());
}

int btn_printf(Button button)
{
    switch(button) {
        case Select:
            return printf("Select\n");
            break;
        case Right:
            return printf("Right\n");
            break;
        case Down:
            return printf("Down\n");
            break;
        case Up:
            return printf("Up\n");
            break;
        case Left:
            return printf("Left\n");
            break;
        default:
            return printf("Null\n");
            break;
    }
}