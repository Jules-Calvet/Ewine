################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/ST/STM32_WPAN/ble/svc/Src/svc_ctl.c 

OBJS += \
./Middlewares/ST/STM32_WPAN/ble/svc/Src/svc_ctl.o 

C_DEPS += \
./Middlewares/ST/STM32_WPAN/ble/svc/Src/svc_ctl.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/ST/STM32_WPAN/ble/svc/Src/%.o Middlewares/ST/STM32_WPAN/ble/svc/Src/%.su Middlewares/ST/STM32_WPAN/ble/svc/Src/%.cyclo: ../Middlewares/ST/STM32_WPAN/ble/svc/Src/%.c Middlewares/ST/STM32_WPAN/ble/svc/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32WB55xx -c -I../Core/Inc -I../STM32_WPAN/App -I../Drivers/STM32WBxx_HAL_Driver/Inc -I../Drivers/STM32WBxx_HAL_Driver/Inc/Legacy -I../Utilities/lpm/tiny_lpm -I../Middlewares/ST/STM32_WPAN -I../Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread -I../Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/tl -I../Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/shci -I../Middlewares/ST/STM32_WPAN/utilities -I../Middlewares/ST/STM32_WPAN/ble/core -I../Middlewares/ST/STM32_WPAN/ble/core/auto -I../Middlewares/ST/STM32_WPAN/ble/core/template -I../Middlewares/ST/STM32_WPAN/ble/svc/Inc -I../Middlewares/ST/STM32_WPAN/ble/svc/Src -I../Drivers/CMSIS/Device/ST/STM32WBxx/Include -I../Utilities/sequencer -I../Middlewares/ST/STM32_WPAN/ble -I../Drivers/CMSIS/Include -I"C:/Users/Justin/Desktop/Ewine/Ewine/STM32Projects/Complete/Complete_Ewine_04/library_IoT/Inc" -I"C:/Users/Justin/Desktop/Ewine/Ewine/STM32Projects/Complete/Complete_Ewine_04/ESP32/Inc" -I../TOF/App -I../TOF/Target -I../Drivers/BSP/STM32WBxx_Nucleo -I../Drivers/BSP/Components/Common -I../Drivers/BSP/Components/vl53l1cb/modules -I../Drivers/BSP/Components/vl53l1cb/porting -I../Drivers/BSP/Components/vl53l1cb -I../Drivers/BSP/53L1A2 -I"C:/Users/Justin/Desktop/Ewine/Ewine/STM32Projects/Complete/Complete_Ewine_04/IR/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Middlewares-2f-ST-2f-STM32_WPAN-2f-ble-2f-svc-2f-Src

clean-Middlewares-2f-ST-2f-STM32_WPAN-2f-ble-2f-svc-2f-Src:
	-$(RM) ./Middlewares/ST/STM32_WPAN/ble/svc/Src/svc_ctl.cyclo ./Middlewares/ST/STM32_WPAN/ble/svc/Src/svc_ctl.d ./Middlewares/ST/STM32_WPAN/ble/svc/Src/svc_ctl.o ./Middlewares/ST/STM32_WPAN/ble/svc/Src/svc_ctl.su

.PHONY: clean-Middlewares-2f-ST-2f-STM32_WPAN-2f-ble-2f-svc-2f-Src

