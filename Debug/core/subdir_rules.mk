################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
core/%.obj: ../core/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"/home/ebad/ti/ccs1020/ccs/tools/compiler/ti-cgt-arm_20.2.2.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="/home/ebad/workspace_v10/vl53l1x_ULD_MSP_EXP432P401R_nortos_ccs" --include_path="/home/ebad/workspace_v10/vl53l1x_ULD_MSP_EXP432P401R_nortos_ccs/core" --include_path="/home/ebad/workspace_v10/vl53l1x_ULD_MSP_EXP432P401R_nortos_ccs/platform" --include_path="/home/ebad/workspace_v10/vl53l1x_ULD_MSP_EXP432P401R_nortos_ccs/Debug" --include_path="/home/ebad/ti/simplelink_msp432p4_sdk_3_40_01_02/source" --include_path="/home/ebad/ti/simplelink_msp432p4_sdk_3_40_01_02/source/third_party/CMSIS/Include" --include_path="/home/ebad/ti/simplelink_msp432p4_sdk_3_40_01_02/kernel/nortos" --include_path="/home/ebad/ti/simplelink_msp432p4_sdk_3_40_01_02/kernel/nortos/posix" --include_path="/home/ebad/ti/ccs1020/ccs/tools/compiler/ti-cgt-arm_20.2.2.LTS/include" --advice:power=none --define=BOARD_DISPLAY_USE_UART_ANSI=1 --define=BOARD_DISPLAY_USE_LCD=1 -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --preproc_with_compile --preproc_dependency="core/$(basename $(<F)).d_raw" --include_path="/home/ebad/workspace_v10/vl53l1x_ULD_MSP_EXP432P401R_nortos_ccs/Debug/syscfg" --obj_directory="core" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '


