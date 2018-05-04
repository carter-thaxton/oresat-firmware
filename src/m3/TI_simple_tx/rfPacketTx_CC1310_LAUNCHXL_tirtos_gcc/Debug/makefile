################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/ti/ccsv8/tools/compiler/gcc-arm-none-eabi-7-2017-q4-major-win32

GEN_OPTS += C:/Users/Malay/workspace_v8/tirtos_builds_CC1310_LAUNCHXL_release_gcc/Debug/configPkg/compiler.opt
GEN_OPTS__FLAG := @"C:/Users/Malay/workspace_v8/tirtos_builds_CC1310_LAUNCHXL_release_gcc/Debug/configPkg/compiler.opt"
GEN_CMDS += C:/Users/Malay/workspace_v8/tirtos_builds_CC1310_LAUNCHXL_release_gcc/Debug/configPkg/linker.cmd
GEN_CMDS__FLAG := -Wl,-T"C:/Users/Malay/workspace_v8/tirtos_builds_CC1310_LAUNCHXL_release_gcc/Debug/configPkg/linker.cmd"

ORDERED_OBJS += \
"./CC1310_LAUNCHXL.o" \
"./ccfg.o" \
"./main_tirtos.o" \
"./rfPacketTx.o" \
"./smartrf_settings/smartrf_settings.o" \
-Wl,-T"../CC1310_LAUNCHXL_TIRTOS.lds" \
-l:display.am3g \
-l:grlib.a \
-l:rf_multiMode_cc13x0.am3g \
-l:drivers_cc13x0.am3g \
-l:pdm_cc13x0.am3g \
-l:dpl_cc13x0.am3g \
$(GEN_CMDS__FLAG) \
-l:driverlib.lib \
-lgcc \
-lc \
-lm \
-lnosys \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include smartrf_settings/subdir_vars.mk
-include subdir_rules.mk
-include smartrf_settings/subdir_rules.mk
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
rfPacketTx_CC1310_LAUNCHXL_tirtos_gcc.out \

EXE_OUTPUTS__QUOTED += \
"rfPacketTx_CC1310_LAUNCHXL_tirtos_gcc.out" \

BIN_OUTPUTS += \
rfPacketTx_CC1310_LAUNCHXL_tirtos_gcc.hex \

BIN_OUTPUTS__QUOTED += \
"rfPacketTx_CC1310_LAUNCHXL_tirtos_gcc.hex" \


# All Target
all: $(OBJS) $(LDS_SRCS) $(GEN_CMDS)
	@$(MAKE) --no-print-directory -Onone "rfPacketTx_CC1310_LAUNCHXL_tirtos_gcc.out"

dependents:
	-cd C:/Users/Malay/workspace_v8/tirtos_builds_CC1310_LAUNCHXL_release_gcc/Debug && $(MAKE) all

# Tool invocations
rfPacketTx_CC1310_LAUNCHXL_tirtos_gcc.out: $(OBJS) $(LDS_SRCS) $(GEN_CMDS) C:/Users/Malay/workspace_v8/tirtos_builds_CC1310_LAUNCHXL_release_gcc/Debug/libtirtos_builds_CC1310_LAUNCHXL_release_gcc.a
	@echo 'Building target: "$@"'
	@echo 'Invoking: GNU Linker'
	"C:/ti/ccsv8/tools/compiler/gcc-arm-none-eabi-7-2017-q4-major-win32/bin/arm-none-eabi-gcc.exe" -DDeviceFamily_CC13X0 -DCCFG_FORCE_VDDR_HH=0 -ffunction-sections -fdata-sections -g -gdwarf-3 -gstrict-dwarf -Wall -std=c99 -march=armv7-m -mthumb -Wl,-Map,"rfPacketTx_CC1310_LAUNCHXL_tirtos_gcc.map" -nostartfiles -static -Wl,--gc-sections -L"C:/ti/simplelink_cc13x0_sdk_2_10_00_36/source/ti/display/lib" -L"C:/ti/simplelink_cc13x0_sdk_2_10_00_36/source/ti/grlib/lib/gcc/m3" -L"C:/ti/simplelink_cc13x0_sdk_2_10_00_36/source/ti/drivers/rf/lib" -L"C:/ti/simplelink_cc13x0_sdk_2_10_00_36/source/ti/drivers/lib" -L"C:/ti/simplelink_cc13x0_sdk_2_10_00_36/source/ti/drivers/pdm/lib" -L"C:/ti/simplelink_cc13x0_sdk_2_10_00_36/kernel/tirtos/packages/ti/dpl/lib" -L"C:/ti/simplelink_cc13x0_sdk_2_10_00_36/source/ti/devices/cc13x0/driverlib/bin/gcc" -L"C:/ti/simplelink_cc13x0_sdk_2_10_00_36/kernel/tirtos/packages/gnu/targets/arm/libs/install-native/arm-none-eabi/lib/thumb/v7-m" -L"C:/ti/ccsv8/tools/compiler/gcc-arm-none-eabi-7-2017-q4-major-win32/arm-none-eabi/lib" -march=armv7-m -mthumb --specs=nano.specs -o"rfPacketTx_CC1310_LAUNCHXL_tirtos_gcc.out" $(ORDERED_OBJS)
	@echo 'Finished building target: "$@"'
	@echo ' '

rfPacketTx_CC1310_LAUNCHXL_tirtos_gcc.hex: $(EXE_OUTPUTS)
	@echo 'Building files: $(strip $(EXE_OUTPUTS__QUOTED))'
	@echo 'Invoking: GNU Objcopy Utility'
	"C:/ti/ccsv8/tools/compiler/gcc-arm-none-eabi-7-2017-q4-major-win32/bin/arm-none-eabi-objcopy.exe"  $(EXE_OUTPUTS__QUOTED) "rfPacketTx_CC1310_LAUNCHXL_tirtos_gcc.hex"
	@echo 'Finished building: $(strip $(EXE_OUTPUTS__QUOTED))'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(BIN_OUTPUTS__QUOTED)$(EXE_OUTPUTS__QUOTED)
	-$(RM) "CC1310_LAUNCHXL.o" "ccfg.o" "main_tirtos.o" "rfPacketTx.o" "smartrf_settings\smartrf_settings.o" 
	-$(RM) "CC1310_LAUNCHXL.d" "ccfg.d" "main_tirtos.d" "rfPacketTx.d" "smartrf_settings\smartrf_settings.d" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:
C:/Users/Malay/workspace_v8/tirtos_builds_CC1310_LAUNCHXL_release_gcc/Debug/libtirtos_builds_CC1310_LAUNCHXL_release_gcc.a:

-include ../makefile.targets

