/**
 * @file board.c
 * @brief
 *
 * Copyright (c) 2021 Bouffalolab team
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.  The
 * ASF licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 */

#include "hal_gpio.h"
#include "hal_clock.h"
#include "bl702_glb.h"
#include "bl702_config.h"

struct pin_mux_cfg {
    uint8_t pin;
    uint16_t func;
};

static const struct pin_mux_cfg af_pin_table[] = {
#ifdef CONFIG_GPIO0_FUNC
    { .pin = GPIO_PIN_0,
      .func = CONFIG_GPIO0_FUNC },
#endif
#ifdef CONFIG_GPIO1_FUNC
    { .pin = GPIO_PIN_1,
      .func = CONFIG_GPIO1_FUNC },
#endif
#ifdef CONFIG_GPIO2_FUNC
    { .pin = GPIO_PIN_2,
      .func = CONFIG_GPIO2_FUNC },
#endif
#ifdef CONFIG_GPIO3_FUNC
    { .pin = GPIO_PIN_3,
      .func = CONFIG_GPIO3_FUNC },
#endif
#ifdef CONFIG_GPIO4_FUNC
    { .pin = GPIO_PIN_4,
      .func = CONFIG_GPIO4_FUNC },
#endif
#ifdef CONFIG_GPIO5_FUNC
    { .pin = GPIO_PIN_5,
      .func = CONFIG_GPIO5_FUNC },
#endif
#ifdef CONFIG_GPIO6_FUNC
    { .pin = GPIO_PIN_6,
      .func = CONFIG_GPIO6_FUNC },
#endif
#ifdef CONFIG_GPIO7_FUNC
    { .pin = GPIO_PIN_7,
      .func = CONFIG_GPIO7_FUNC },
#endif
#ifdef CONFIG_GPIO8_FUNC
    { .pin = GPIO_PIN_8,
      .func = CONFIG_GPIO8_FUNC },
#endif
#ifdef CONFIG_GPIO9_FUNC
    { .pin = GPIO_PIN_9,
      .func = CONFIG_GPIO9_FUNC },
#endif
#ifdef CONFIG_GPIO10_FUNC
    { .pin = GPIO_PIN_10,
      .func = CONFIG_GPIO10_FUNC },
#endif
#ifdef CONFIG_GPIO11_FUNC
    { .pin = GPIO_PIN_11,
      .func = CONFIG_GPIO11_FUNC },
#endif
#ifdef CONFIG_GPIO12_FUNC
    { .pin = GPIO_PIN_12,
      .func = CONFIG_GPIO12_FUNC },
#endif
#ifdef CONFIG_GPIO13_FUNC
    { .pin = GPIO_PIN_13,
      .func = CONFIG_GPIO13_FUNC },
#endif
#ifdef CONFIG_GPIO14_FUNC
    { .pin = GPIO_PIN_14,
      .func = CONFIG_GPIO14_FUNC },
#endif
#ifdef CONFIG_GPIO15_FUNC
    { .pin = GPIO_PIN_15,
      .func = CONFIG_GPIO15_FUNC },
#endif
#ifdef CONFIG_GPIO16_FUNC
    { .pin = GPIO_PIN_16,
      .func = CONFIG_GPIO16_FUNC },
#endif
#ifdef CONFIG_GPIO17_FUNC
    { .pin = GPIO_PIN_17,
      .func = CONFIG_GPIO17_FUNC },
#endif
#ifdef CONFIG_GPIO18_FUNC
    { .pin = GPIO_PIN_18,
      .func = CONFIG_GPIO18_FUNC },
#endif
#ifdef CONFIG_GPIO19_FUNC
    { .pin = GPIO_PIN_19,
      .func = CONFIG_GPIO19_FUNC },
#endif
#ifdef CONFIG_GPIO20_FUNC
    { .pin = GPIO_PIN_20,
      .func = CONFIG_GPIO20_FUNC },
#endif
#ifdef CONFIG_GPIO21_FUNC
    { .pin = GPIO_PIN_21,
      .func = CONFIG_GPIO21_FUNC },
#endif
#ifdef CONFIG_GPIO22_FUNC
    { .pin = GPIO_PIN_22,
      .func = CONFIG_GPIO22_FUNC },
#endif
#ifdef CONFIG_GPIO23_FUNC
    { .pin = GPIO_PIN_23,
      .func = CONFIG_GPIO23_FUNC },
#endif
#ifdef CONFIG_GPIO24_FUNC
    { .pin = GPIO_PIN_24,
      .func = CONFIG_GPIO24_FUNC },
#endif
#ifdef CONFIG_GPIO25_FUNC
    { .pin = GPIO_PIN_25,
      .func = CONFIG_GPIO25_FUNC },
#endif
#ifdef CONFIG_GPIO26_FUNC
    { .pin = GPIO_PIN_26,
      .func = CONFIG_GPIO26_FUNC },
#endif
#ifdef CONFIG_GPIO27_FUNC
    { .pin = GPIO_PIN_27,
      .func = CONFIG_GPIO27_FUNC },
#endif
#ifdef CONFIG_GPIO28_FUNC
    { .pin = GPIO_PIN_28,
      .func = CONFIG_GPIO28_FUNC },
#endif
#ifdef CONFIG_GPIO29_FUNC
    { .pin = GPIO_PIN_29,
      .func = CONFIG_GPIO29_FUNC },
#endif
#ifdef CONFIG_GPIO30_FUNC
    { .pin = GPIO_PIN_30,
      .func = CONFIG_GPIO30_FUNC },
#endif
#ifdef CONFIG_GPIO31_FUNC
    { .pin = GPIO_PIN_31,
      .func = CONFIG_GPIO31_FUNC },
#endif
};

static void board_pin_mux_init(void)
{
    GLB_GPIO_Cfg_Type gpio_cfg;

    gpio_cfg.drive = 0;
    gpio_cfg.smtCtrl = 1;

    for (int i = 0; i < sizeof(af_pin_table) / sizeof(af_pin_table[0]); i++) {
        gpio_cfg.gpioMode = GPIO_MODE_AF;
        gpio_cfg.pullType = GPIO_PULL_UP;
        gpio_cfg.gpioPin = af_pin_table[i].pin;
        gpio_cfg.gpioFun = af_pin_table[i].func;

        /*if reset state*/
        if (af_pin_table[i].func == GPIO_FUN_UNUSED) {
            continue;
        } else if (af_pin_table[i].func == GPIO_FUN_PWM) {
            /*if pwm func*/
            gpio_cfg.pullType = GPIO_PULL_DOWN;
        } else if (af_pin_table[i].func == GPIO_FUN_QDEC) {
            /* if qdec a/b */
            gpio_cfg.pullType = GPIO_PULL_NONE;
            gpio_cfg.gpioMode = GPIO_MODE_INPUT;
            gpio_cfg.gpioFun = GPIO_FUN_QDEC;
        } else if (af_pin_table[i].func == GPIO_FUN_QDEC_LED) {
            /* if qdec led */
            gpio_cfg.pullType = GPIO_PULL_NONE;
            gpio_cfg.gpioMode = GPIO_MODE_OUTPUT;
            gpio_cfg.gpioFun = GPIO_FUN_QDEC;
        } else if ((af_pin_table[i].func == GPIO_FUN_USB) || (af_pin_table[i].func == GPIO_FUN_DAC) || (af_pin_table[i].func == GPIO_FUN_ADC)) {
            /*if analog func , for usb、adc、dac*/
            gpio_cfg.gpioFun = GPIO_FUN_ANALOG;
            gpio_cfg.gpioMode = GPIO_MODE_ANALOG;
            gpio_cfg.pullType = GPIO_PULL_NONE;
        } else if ((af_pin_table[i].func & 0x70) == 0x70) {
            /*if uart func*/
            gpio_cfg.gpioFun = GPIO_FUN_UART;
            uint8_t sig = af_pin_table[i].func & 0x07;
            /*link to one uart sig*/
            GLB_UART_Fun_Sel((gpio_cfg.gpioPin % 8), sig);
        } else if (af_pin_table[i].func == GPIO_FUN_CLK_OUT) {
            if (af_pin_table[i].pin % 2) {
                /*odd gpio output clock*/
                GLB_Set_Chip_Out_1_CLK_Sel(GLB_CHIP_CLK_OUT_I2S_REF_CLK);
            } else {
                /*even gpio output clock*/
                GLB_Set_Chip_Out_0_CLK_Sel(GLB_CHIP_CLK_OUT_I2S_REF_CLK);
            }
        } else if ((af_pin_table[i].func == GPIO_FUN_GPIO_INPUT_UP) || (af_pin_table[i].func == GPIO_FUN_GPIO_EXTI_FALLING_EDGE) || (af_pin_table[i].func == GPIO_FUN_GPIO_EXTI_LOW_LEVEL)) {
            /*if common gpio func,include input、output and exti*/
            gpio_cfg.gpioFun = GPIO_FUN_GPIO;
            gpio_cfg.gpioMode = GPIO_MODE_INPUT;
            gpio_cfg.pullType = GPIO_PULL_UP;
            if (af_pin_table[i].func == GPIO_FUN_GPIO_EXTI_FALLING_EDGE) {
                GLB_Set_GPIO_IntMod(af_pin_table[i].pin, GLB_GPIO_INT_CONTROL_ASYNC, GLB_GPIO_INT_TRIG_NEG_PULSE);
            } else if (af_pin_table[i].func == GPIO_FUN_GPIO_EXTI_LOW_LEVEL) {
                GLB_Set_GPIO_IntMod(af_pin_table[i].pin, GLB_GPIO_INT_CONTROL_ASYNC, GLB_GPIO_INT_TRIG_NEG_LEVEL);
            }
        } else if ((af_pin_table[i].func == GPIO_FUN_GPIO_INPUT_DOWN) || (af_pin_table[i].func == GPIO_FUN_GPIO_EXTI_RISING_EDGE) || (af_pin_table[i].func == GPIO_FUN_GPIO_EXTI_HIGH_LEVEL)) {
            gpio_cfg.gpioFun = GPIO_FUN_GPIO;
            gpio_cfg.gpioMode = GPIO_MODE_INPUT;
            gpio_cfg.pullType = GPIO_PULL_DOWN;
            if (af_pin_table[i].func == GPIO_FUN_GPIO_EXTI_RISING_EDGE) {
                GLB_Set_GPIO_IntMod(af_pin_table[i].pin, GLB_GPIO_INT_CONTROL_ASYNC, GLB_GPIO_INT_TRIG_POS_PULSE);
            } else if (af_pin_table[i].func == GPIO_FUN_GPIO_EXTI_HIGH_LEVEL) {
                GLB_Set_GPIO_IntMod(af_pin_table[i].pin, GLB_GPIO_INT_CONTROL_ASYNC, GLB_GPIO_INT_TRIG_POS_LEVEL);
            }
        } else if (af_pin_table[i].func == GPIO_FUN_GPIO_INPUT_NONE) {
            gpio_cfg.gpioFun = GPIO_FUN_GPIO;
            gpio_cfg.gpioMode = GPIO_MODE_INPUT;
            gpio_cfg.pullType = GPIO_PULL_NONE;
        } else if (af_pin_table[i].func == GPIO_FUN_GPIO_OUTPUT_UP) {
            gpio_cfg.gpioFun = GPIO_FUN_GPIO;
            gpio_cfg.gpioMode = GPIO_MODE_OUTPUT;
            gpio_cfg.pullType = GPIO_PULL_UP;
        } else if (af_pin_table[i].func == GPIO_FUN_GPIO_OUTPUT_DOWN) {
            gpio_cfg.gpioFun = GPIO_FUN_GPIO;
            gpio_cfg.gpioMode = GPIO_MODE_OUTPUT;
            gpio_cfg.pullType = GPIO_PULL_DOWN;
        } else if (af_pin_table[i].func == GPIO_FUN_GPIO_OUTPUT_NONE) {
            gpio_cfg.gpioFun = GPIO_FUN_GPIO;
            gpio_cfg.gpioMode = GPIO_MODE_OUTPUT;
            gpio_cfg.pullType = GPIO_PULL_NONE;
        }
        GLB_GPIO_Init(&gpio_cfg);
    }
}

static void board_clock_init(void)
{
    system_clock_init();
    peripheral_clock_init();
}

void bl_show_info(void)
{
    MSG("\r\n");
    MSG("  ____               __  __      _       _       _     \r\n");
    MSG(" |  _ \\             / _|/ _|    | |     | |     | |    \r\n");
    MSG(" | |_) | ___  _   _| |_| |_ __ _| | ___ | | __ _| |__  \r\n");
    MSG(" |  _ < / _ \\| | | |  _|  _/ _` | |/ _ \\| |/ _` | '_ \\ \r\n");
    MSG(" | |_) | (_) | |_| | | | || (_| | | (_) | | (_| | |_) |\r\n");
    MSG(" |____/ \\___/ \\__,_|_| |_| \\__,_|_|\\___/|_|\\__,_|_.__/ \r\n");
    MSG("\r\n");
    MSG("Build:%s,%s\r\n", __TIME__, __DATE__);
    MSG("Copyright (c) 2021 Bouffalolab team\r\n");

#if 0
    MSG("root clock:%dM\r\n", system_clock_get(SYSTEM_CLOCK_ROOT_CLOCK) / 1000000);
    MSG("fclk clock:%dM\r\n", system_clock_get(SYSTEM_CLOCK_FCLK) / 1000000);
    MSG("bclk clock:%dM\r\n", system_clock_get(SYSTEM_CLOCK_BCLK) / 1000000);

    MSG("uart clock:%dM\r\n", peripheral_clock_get(PERIPHERAL_CLOCK_UART) / 1000000);
    MSG("spi clock:%dM\r\n", peripheral_clock_get(PERIPHERAL_CLOCK_SPI) / 1000000);
    MSG("i2c clock:%dM\r\n", peripheral_clock_get(PERIPHERAL_CLOCK_I2C) / 1000000);
#endif
}

void board_init(void)
{
    board_clock_init();
    board_pin_mux_init();
}

int bflb_get_board_config(uint8_t func, uint8_t *pinlist)
{
    uint16_t i = 0;
    int len = 0;

    for (i = 0; i < sizeof(af_pin_table) / sizeof(struct pin_mux_cfg); i++) {
        if (af_pin_table[i].func == func) {
            pinlist[len] = af_pin_table[i].pin;
            len++;
        }
    }
    return len;
}
