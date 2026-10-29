// control.c
#include "config.h"
#include "control.h"

static int prev_error = 0;

void Control_Reset(void) {
    prev_error = 0;
}

int Control_Update(int error) {
    // ⚠️ 变量声明放最前面
    int derivative;
    int output;

    derivative = error - prev_error;
    prev_error = error;

    output = KP * error + KD * derivative;

    if (output >  OUTPUT_MAX) output =  OUTPUT_MAX;
    if (output < -OUTPUT_MAX) output = -OUTPUT_MAX;

    return output;
}