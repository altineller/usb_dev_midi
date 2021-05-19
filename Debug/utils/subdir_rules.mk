################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
utils/uartstdio.obj: /home/can/ti/TivaWare_C_Series-2.2.0.295/utils/uartstdio.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"/home/can/ti/ccs1030/ccs/tools/compiler/ti-cgt-arm_20.2.5.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -O2 --include_path="/home/can/ti/ccs1030/ccs/tools/compiler/ti-cgt-arm_20.2.5.LTS/include" --include_path="/home/can/ti/TivaWare_C_Series-2.2.0.295" --include_path="/home/can/workspace_v10/usb_dev_midi/include" --include_path="/home/can/workspace_v10/usb_dev_midi/include/midi" --include_path="/home/can/workspace_v10/usb_dev_midi/include/usb_midi" --advice:power=all --define=ccs="ccs" --define=PART_TM4C123GH6PM --define=TARGET_IS_TM4C123_RB1 --define=UART_BUFFERED -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --ual --preproc_with_compile --preproc_dependency="utils/$(basename $(<F)).d_raw" --obj_directory="utils" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '

utils/ustdlib.obj: /home/can/ti/TivaWare_C_Series-2.2.0.295/utils/ustdlib.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"/home/can/ti/ccs1030/ccs/tools/compiler/ti-cgt-arm_20.2.5.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -O2 --include_path="/home/can/ti/ccs1030/ccs/tools/compiler/ti-cgt-arm_20.2.5.LTS/include" --include_path="/home/can/ti/TivaWare_C_Series-2.2.0.295" --include_path="/home/can/workspace_v10/usb_dev_midi/include" --include_path="/home/can/workspace_v10/usb_dev_midi/include/midi" --include_path="/home/can/workspace_v10/usb_dev_midi/include/usb_midi" --advice:power=all --define=ccs="ccs" --define=PART_TM4C123GH6PM --define=TARGET_IS_TM4C123_RB1 --define=UART_BUFFERED -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --ual --preproc_with_compile --preproc_dependency="utils/$(basename $(<F)).d_raw" --obj_directory="utils" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '


