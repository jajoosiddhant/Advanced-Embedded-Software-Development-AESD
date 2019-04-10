################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.4.LTS

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./main.obj" \
"./startup_ccs.obj" \
"./drivers/buttons.obj" \
"./drivers/eth_client_lwip.obj" \
"./drivers/exosite_hal_lwip.obj" \
"./drivers/http.obj" \
"./drivers/pinout.obj" \
"./src/led.obj" \
"./src/logger.obj" \
"./src/temp.obj" \
"./third_party/FreeRTOS/Source/list.obj" \
"./third_party/FreeRTOS/Source/queue.obj" \
"./third_party/FreeRTOS/Source/tasks.obj" \
"./third_party/FreeRTOS/Source/timers.obj" \
"./third_party/FreeRTOS/Source/portable/CCS/ARM_CM4F/port.obj" \
"./third_party/FreeRTOS/Source/portable/CCS/ARM_CM4F/portasm.obj" \
"./third_party/FreeRTOS/Source/portable/MemMang/heap_1.obj" \
"./third_party/exosite/exosite.obj" \
"./third_party/exosite/exosite_meta.obj" \
"./utils/cmdline.obj" \
"./utils/flash_pb.obj" \
"./utils/locator.obj" \
"./utils/lwiplib.obj" \
"./utils/ringbuf.obj" \
"./utils/uartstdio.obj" \
"./utils/ustdlib.obj" \
"../senshub_iot_ccs.cmd" \
$(GEN_CMDS__FLAG) \
-llibc.a \
-l"C:/ti/TivaWare_C_Series-2.1.4.178/driverlib/ccs/Debug/driverlib.lib" \
-l"C:/ti/TivaWare_C_Series-2.1.4.178/sensorlib/ccs/Debug/sensorlib.lib" \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include drivers/subdir_vars.mk
-include src/subdir_vars.mk
-include third_party/FreeRTOS/Source/subdir_vars.mk
-include third_party/FreeRTOS/Source/portable/CCS/ARM_CM4F/subdir_vars.mk
-include third_party/FreeRTOS/Source/portable/MemMang/subdir_vars.mk
-include third_party/exosite/subdir_vars.mk
-include utils/subdir_vars.mk
-include subdir_rules.mk
-include drivers/subdir_rules.mk
-include src/subdir_rules.mk
-include third_party/FreeRTOS/Source/subdir_rules.mk
-include third_party/FreeRTOS/Source/portable/CCS/ARM_CM4F/subdir_rules.mk
-include third_party/FreeRTOS/Source/portable/MemMang/subdir_rules.mk
-include third_party/exosite/subdir_rules.mk
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
senshub_iot.out \

EXE_OUTPUTS__QUOTED += \
"senshub_iot.out" \

BIN_OUTPUTS += \
senshub_iot.bin \

BIN_OUTPUTS__QUOTED += \
"senshub_iot.bin" \


# All Target
all: senshub_iot.out

# Tool invocations
senshub_iot.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: "$@"'
	@echo 'Invoking: ARM Linker'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.4.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me -O0 --advice:power=all -g --gcc --define=ccs="ccs" --define=PART_TM4C1294NCPDT --define=TARGET_IS_TM4C129_RA0 --define=UART_BUFFERED --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --ual -z -m"senshub_iot_ccs.map" --stack_size=16384 --heap_size=0 -i"C:/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.4.LTS/lib" -i"C:/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.4.LTS/include" --reread_libs --diag_wrap=off --display_error_number --warn_sections --xml_link_info="senshub_iot_linkInfo.xml" --rom_model -o "senshub_iot.out" $(ORDERED_OBJS)
	@echo 'Finished building target: "$@"'
	@echo ' '
	@$(MAKE) --no-print-directory post-build

senshub_iot.bin: $(EXE_OUTPUTS)
	@echo 'Building files: $(strip $(EXE_OUTPUTS__QUOTED))'
	@echo 'Invoking: ARM Hex Utility'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.4.LTS/bin/armhex"  -o "senshub_iot.bin" $(EXE_OUTPUTS__QUOTED)
	@echo 'Finished building: $(strip $(EXE_OUTPUTS__QUOTED))'
	@echo ' '
	@$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(BIN_OUTPUTS__QUOTED)$(EXE_OUTPUTS__QUOTED)
	-$(RM) "main.obj" "startup_ccs.obj" "drivers\buttons.obj" "drivers\eth_client_lwip.obj" "drivers\exosite_hal_lwip.obj" "drivers\http.obj" "drivers\pinout.obj" "src\led.obj" "src\logger.obj" "src\temp.obj" "third_party\FreeRTOS\Source\list.obj" "third_party\FreeRTOS\Source\queue.obj" "third_party\FreeRTOS\Source\tasks.obj" "third_party\FreeRTOS\Source\timers.obj" "third_party\FreeRTOS\Source\portable\CCS\ARM_CM4F\port.obj" "third_party\FreeRTOS\Source\portable\CCS\ARM_CM4F\portasm.obj" "third_party\FreeRTOS\Source\portable\MemMang\heap_1.obj" "third_party\exosite\exosite.obj" "third_party\exosite\exosite_meta.obj" "utils\cmdline.obj" "utils\flash_pb.obj" "utils\locator.obj" "utils\lwiplib.obj" "utils\ringbuf.obj" "utils\uartstdio.obj" "utils\ustdlib.obj" 
	-$(RM) "main.d" "startup_ccs.d" "drivers\buttons.d" "drivers\eth_client_lwip.d" "drivers\exosite_hal_lwip.d" "drivers\http.d" "drivers\pinout.d" "src\led.d" "src\logger.d" "src\temp.d" "third_party\FreeRTOS\Source\list.d" "third_party\FreeRTOS\Source\queue.d" "third_party\FreeRTOS\Source\tasks.d" "third_party\FreeRTOS\Source\timers.d" "third_party\FreeRTOS\Source\portable\CCS\ARM_CM4F\port.d" "third_party\FreeRTOS\Source\portable\MemMang\heap_1.d" "third_party\exosite\exosite.d" "third_party\exosite\exosite_meta.d" "utils\cmdline.d" "utils\flash_pb.d" "utils\locator.d" "utils\lwiplib.d" "utils\ringbuf.d" "utils\uartstdio.d" "utils\ustdlib.d" 
	-$(RM) "third_party\FreeRTOS\Source\portable\CCS\ARM_CM4F\portasm.d" 
	-@echo 'Finished clean'
	-@echo ' '

post-build:
	-"C:/ti/ccsv8/utils/tiobj2bin/tiobj2bin" "senshub_iot.out" "senshub_iot.bin" "C:/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.4.LTS/bin/armofd" "C:/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.4.LTS/bin/armhex" "C:/ti/ccsv8/utils/tiobj2bin/mkhex4bin"
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

