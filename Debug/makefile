################################################################################
# Automatically-generated file. Do not edit!
################################################################################

CG_TOOL_ROOT := /home/can/ti/ccs1030/ccs/tools/compiler/ti-cgt-arm_20.2.5.LTS

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./main.obj" \
"./midi_usb_rx_task.obj" \
"./startup_ccs.obj" \
"./include/usb_midi/usb_midi_fifo.obj" \
"./include/usb_midi/usbmidi.obj" \
"./include/usb_midi/usbmidi_handlers.obj" \
"./utils/uartstdio.obj" \
"./utils/ustdlib.obj" \
"../usb_dev_bulk_ccs.cmd" \
$(GEN_CMDS__FLAG) \
-llibc.a \
-l"/home/can/ti/TivaWare_C_Series-2.2.0.295/driverlib/ccs/Debug/driverlib.lib" \
-l"/home/can/ti/TivaWare_C_Series-2.2.0.295/usblib/ccs/Debug/usblib.lib" \

-include ../makefile.init

RM := rm -rf
RMDIR := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include include/usb_midi/subdir_vars.mk
-include utils/subdir_vars.mk
-include subdir_rules.mk
-include include/usb_midi/subdir_rules.mk
-include utils/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(SV7A_DEPS)),)
-include $(SV7A_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
usb_dev_midi.out \

EXE_OUTPUTS__QUOTED += \
"usb_dev_midi.out" \

BIN_OUTPUTS += \
usb_dev_midi.bin \

BIN_OUTPUTS__QUOTED += \
"usb_dev_midi.bin" \


# All Target
all: usb_dev_midi.out

# Tool invocations
usb_dev_midi.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: "$@"'
	@echo 'Invoking: Arm Linker'
	"/home/can/ti/ccs1030/ccs/tools/compiler/ti-cgt-arm_20.2.5.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -O2 --advice:power=all --define=ccs="ccs" --define=PART_TM4C123GH6PM --define=TARGET_IS_TM4C123_RB1 --define=UART_BUFFERED -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --ual -z -m"usb_dev_bulk_ccs.map" --heap_size=0 --stack_size=1024 -i"/home/can/ti/ccs1030/ccs/tools/compiler/ti-cgt-arm_20.2.5.LTS/lib" -i"/home/can/ti/ccs1030/ccs/tools/compiler/ti-cgt-arm_20.2.5.LTS/include" --reread_libs --diag_wrap=off --display_error_number --warn_sections --xml_link_info="usb_dev_midi_linkInfo.xml" --rom_model -o "usb_dev_midi.out" $(ORDERED_OBJS)
	@echo 'Finished building target: "$@"'
	@echo ' '
	@$(MAKE) --no-print-directory post-build

usb_dev_midi.bin: $(EXE_OUTPUTS)
	@echo 'Building secondary target: "$@"'
	@echo 'Invoking: Arm Hex Utility'
	"/home/can/ti/ccs1030/ccs/tools/compiler/ti-cgt-arm_20.2.5.LTS/bin/armhex" --diag_wrap=off -o "usb_dev_midi.bin" $(EXE_OUTPUTS__QUOTED)
	@echo 'Finished building secondary target: "$@"'
	@echo ' '
	@$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(BIN_OUTPUTS__QUOTED)$(EXE_OUTPUTS__QUOTED)
	-$(RM) "main.obj" "midi_usb_rx_task.obj" "startup_ccs.obj" "include/usb_midi/usb_midi_fifo.obj" "include/usb_midi/usbmidi.obj" "include/usb_midi/usbmidi_handlers.obj" "utils/uartstdio.obj" "utils/ustdlib.obj" 
	-$(RM) "main.d" "midi_usb_rx_task.d" "startup_ccs.d" "include/usb_midi/usb_midi_fifo.d" "include/usb_midi/usbmidi.d" "include/usb_midi/usbmidi_handlers.d" "utils/uartstdio.d" "utils/ustdlib.d" 
	-@echo 'Finished clean'
	-@echo ' '

post-build:
	-"/home/can/ti/ccs1030/ccs/utils/tiobj2bin/tiobj2bin" "usb_dev_midi.out" "usb_dev_midi.bin" "/home/can/ti/ccs1030/ccs/tools/compiler/ti-cgt-arm_20.2.5.LTS/bin/armofd" "/home/can/ti/ccs1030/ccs/tools/compiler/ti-cgt-arm_20.2.5.LTS/bin/armhex" "/home/can/ti/ccs1030/ccs/utils/tiobj2bin/mkhex4bin"
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

