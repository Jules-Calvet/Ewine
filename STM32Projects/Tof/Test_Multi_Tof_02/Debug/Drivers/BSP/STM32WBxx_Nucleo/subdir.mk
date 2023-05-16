################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/BSP/STM32WBxx_Nucleo/stm32wbxx_nucleo.c 

OBJS += \
./Drivers/BSP/STM32WBxx_Nucleo/stm32wbxx_nucleo.o 

C_DEPS += \
./Drivers/BSP/STM32WBxx_Nucleo/stm32wbxx_nucleo.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP/STM32WBxx_Nucleo/%.o Drivers/BSP/STM32WBxx_Nucleo/%.su Drivers/BSP/STM32WBxx_Nucleo/%.cyclo: ../Drivers/BSP/STM32WBxx_Nucleo/%.c Drivers/BSP/STM32WBxx_Nucleo/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32WB55xx -c -I../Core/Inc -I../Drivers/STM32WBxx_HAL_Driver/Inc -I../Drivers/STM32WBxx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32WBxx/Include -I../Drivers/CMSIS/Include -I../TOF/App -I../TOF/Target -I../Drivers/BSP/STM32WBxx_Nucleo -I../Drivers/BSP/Components/Common -I../Drivers/BSP/Components/vl53l1cb/modules -I../Drivers/BSP/Components/vl53l1cb/porting -I../Drivers/BSP/Components/vl53l1cb -I../Drivers/BSP/53L1A2 -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-BSP-2f-STM32WBxx_Nucleo

clean-Drivers-2f-BSP-2f-STM32WBxx_Nucleo:
	-$(RM) ./Drivers/BSP/STM32WBxx_Nucleo/stm32wbxx_nucleo.cyclo ./Drivers/BSP/STM32WBxx_Nucleo/stm32wbxx_nucleo.d ./Drivers/BSP/STM32WBxx_Nucleo/stm32wbxx_nucleo.o ./Drivers/BSP/STM32WBxx_Nucleo/stm32wbxx_nucleo.su

.PHONY: clean-Drivers-2f-BSP-2f-STM32WBxx_Nucleo

