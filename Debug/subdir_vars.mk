################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CMD_SRCS += \
../MSP_EXP432P401R_NoRTOS.cmd 

SYSCFG_SRCS += \
../VL53L1.syscfg 

C_SRCS += \
./syscfg/ti_drivers_config.c \
../main_nortos.c \
../vl53l1.c 

GEN_FILES += \
./syscfg/ti_drivers_config.c 

GEN_MISC_DIRS += \
./syscfg/ 

C_DEPS += \
./syscfg/ti_drivers_config.d \
./main_nortos.d \
./vl53l1.d 

OBJS += \
./syscfg/ti_drivers_config.obj \
./main_nortos.obj \
./vl53l1.obj 

GEN_MISC_FILES += \
./syscfg/ti_drivers_config.h \
./syscfg/syscfg_c.rov.xs 

GEN_MISC_DIRS__QUOTED += \
"syscfg/" 

OBJS__QUOTED += \
"syscfg/ti_drivers_config.obj" \
"main_nortos.obj" \
"vl53l1.obj" 

GEN_MISC_FILES__QUOTED += \
"syscfg/ti_drivers_config.h" \
"syscfg/syscfg_c.rov.xs" 

C_DEPS__QUOTED += \
"syscfg/ti_drivers_config.d" \
"main_nortos.d" \
"vl53l1.d" 

GEN_FILES__QUOTED += \
"syscfg/ti_drivers_config.c" 

SYSCFG_SRCS__QUOTED += \
"../VL53L1.syscfg" 

C_SRCS__QUOTED += \
"./syscfg/ti_drivers_config.c" \
"../main_nortos.c" \
"../vl53l1.c" 


