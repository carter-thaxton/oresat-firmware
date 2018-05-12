################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
smartrf_settings/%.o: ../smartrf_settings/%.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Compiler'
	"" -c -mcpu=cortex-m3 -march=armv7-m -mthumb -mfloat-abi=soft -DDeviceFamily_CC13X0 -DCCFG_FORCE_VDDR_HH=0 -I"/home/malay/Documents/ORESAT/mygit/oresat-firmware/src/m3/CC1310_Simplex/rfPacketRx_CC1310_LAUNCHXL_tirtos_gcc" -I"/home/malay/ti/simplelink_cc13x0_sdk_2_10_00_36/source/ti/posix/gcc" -I"/home/malay/ti/simplelink_cc13x0_sdk_2_10_00_36/kernel/tirtos/packages/gnu/targets/arm/libs/install-native/arm-none-eabi/include/newlib-nano" -I"/home/malay/ti/simplelink_cc13x0_sdk_2_10_00_36/kernel/tirtos/packages/gnu/targets/arm/libs/install-native/arm-none-eabi/include" -I"/arm-none-eabi/include" -ffunction-sections -fdata-sections -g -gdwarf-3 -gstrict-dwarf -Wall -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o"$@" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '


