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

uint8_t roiCenter[4][4] = {
    {145, 177, 209, 241},
    {149, 181, 213, 245},
    {110, 78, 46, 14},
    {106, 74, 42, 18}
};

uint16_t roiX = 4;
uint16_t roiY = 4;

uint16_t distances[4][4];


/*
 *  ======== mainThread ========
 */
void *mainThread(void *arg0)
{
    Watchdog_Handle watchdogHandle;
    Watchdog_Params params;
    Watchdog_init();

    GPIO_init();

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
    I2C_init();
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

    //Predefined values = 15, 20, 33, 50, 100(default), 200, 500.
    VL53L1X_SetTimingBudgetInMs(i2c1,100);
    // Intermeasurement is always less than timing budget
    VL53L1X_SetInterMeasurementInMs(i2c1,97);
    VL53L1X_SetROI(i2c1,roiX,roiY);

    /* Display Timing Budget */
    uint16_t timingBudget;
    VL53L1X_GetTimingBudgetInMs(i2c1,&timingBudget);
    Display_printf(display, 0, 0, "Timing Budget in ms: %d" ,timingBudget);

    uint16_t interMeasurement;
    VL53L1X_GetInterMeasurementInMs(i2c1,&interMeasurement);
    Display_printf(display, 0, 0, "InterMeasurement in ms : %d" ,interMeasurement);



    GPIO_write(CONFIG_GPIO_LED_0, CONFIG_GPIO_LED_ON);

    Display_printf(display, 0, 0, "Program Fixed Check\n");

    VL53L1X_StartRanging(i2c1);

    while (1) {

        Watchdog_clear(watchdogHandle);
        uint8_t dataReady = false;

        int x,y;

        for(y=0; y<4; y++)
        {
            for(x=0; x<4; x++)
            {
                VL53L1X_SetROICenter(i2c1,roiCenter[y][x]);
                Watchdog_clear(watchdogHandle);

                dataReady = false;
                while(!dataReady)
                {
                    VL53L1X_CheckForDataReady(i2c1,&dataReady);
                    usleep(5);
                }
                VL53L1X_GetDistance(i2c1,&distances[y][x]);

                // After reading the results reset the interrupt to be able to take another measurement
                VL53L1X_ClearInterrupt(i2c1);
            }

        }

        Display_printf(display, 0, 0, " %d %d %d %d \n %d %d %d %d \n %d %d %d %d \n %d %d %d %d \n" ,distances[0][0],distances[0][1],distances[0][2],distances[0][3],distances[1][0],distances[1][1],distances[1][2],distances[1][3],distances[2][0],distances[2][1],distances[2][2],distances[2][3],distances[3][0],distances[3][1],distances[3][2],distances[3][3]);



        /* Toggle LED */
        GPIO_toggle(CONFIG_GPIO_LED_0);
    }

    return 0;
}
