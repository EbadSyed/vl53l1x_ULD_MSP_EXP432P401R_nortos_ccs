/*
 * Copyright (c) 2018-2019, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *  ======== display.c ========
 */
#include <stdint.h>
#include <unistd.h>

/* TI-Drivers Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/Watchdog.h>
#include <ti/drivers/watchdog/WatchdogMSP432.h>

/* Driver Configuration */
#include "ti_drivers_config.h"

#include "core/VL53L1X_api.h"


/*
 *  ======== mainThread ========
 */
void *mainThread(void *arg0)
{
    Watchdog_Handle watchdogHandle;
    Watchdog_Params params;
    Watchdog_init();

    GPIO_init();
    I2C_init();
    Display_init();

    /* Open a Watchdog driver instance */
    Watchdog_Params_init(&params);
    params.resetMode = Watchdog_RESET_ON;

    watchdogHandle = Watchdog_open(CONFIG_WATCHDOG_0, &params);
    if (watchdogHandle == NULL) {
        /* Error opening Watchdog */
        while (1) {}
    }

    /* Open the HOST display for output */
    display = Display_open(Display_Type_UART, NULL);
    if (display == NULL) {
        while (1);
    }

    Display_printf(display, 0, 0, "System Reset \n");

    /* Create I2C for usage */
    I2C_Params_init(&i2cParams);
    i2cParams.bitRate = I2C_1000kHz;

    i2c1 = I2C_open(CONFIG_I2C_1, &i2cParams);
    if (i2c1 == NULL) {
        while (1);
    }


    VL53L1X_SensorInit(i2c1);
    Watchdog_clear(watchdogHandle);
    Display_printf(display, 0, 0, "Sensor Initialized \n");

    /* Short Distance Mode, Change to 2 or comment out for Long Mode) */
    VL53L1X_SetDistanceMode(i2c1,1);

    /* Change Timing Budget if Needed */

//    VL53L1X_SetTimingBudgetInMs(i2c1,50);
//    VL53L1X_SetInterMeasurementInMs(i2c1,47);

    /* Display Timing Budget */
    uint16_t timingBudget;
    VL53L1X_GetTimingBudgetInMs(i2c1,&timingBudget);
    Display_printf(display, 0, 0, "Timing Budget in ms: %d" ,timingBudget);

    uint16_t interMeasurement;
    VL53L1X_GetInterMeasurementInMs(i2c1,&interMeasurement);
    Display_printf(display, 0, 0, "InterMeasurement in ms : %d" ,interMeasurement);

    VL53L1X_StartRanging(i2c1);

    GPIO_write(CONFIG_GPIO_LED_0, CONFIG_GPIO_LED_ON);

    Display_printf(display, 0, 0, "Program Fixed Check\n");


    while (1) {

        Watchdog_clear(watchdogHandle);
        uint8_t dataReady = false;

        while(!dataReady) {
            VL53L1X_CheckForDataReady(i2c1,&dataReady);
            usleep(5);

        }

        // Get the results
        uint16_t distance;
        VL53L1X_GetDistance(i2c1,&distance);
        Display_printf(display, 0, 0, "Distance in mm: %d" ,distance);

        // After reading the results reset the interrupt to be able to take another measurement
        VL53L1X_ClearInterrupt(i2c1);

        /* Toggle LED */
        GPIO_toggle(CONFIG_GPIO_LED_0);
    }

    return 0;
}
