################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/BSP/Components/vl53l1cb/vl53l1cb.c 

OBJS += \
./Drivers/BSP/Components/vl53l1cb/vl53l1cb.o 

C_DEPS += \
./Drivers/BSP/Components/vl53l1cb/vl53l1cb.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP/Components/vl53l1cb/%.o Drivers/BSP/Components/vl53l1cb/%.su Drivers/BSP/Components/vl53l1cb/%.cyclo: ../Drivers/BSP/Components/vl53l1cb/%.c Drivers/BSP/Components/vl53l1cb/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32WB55xx -c -I../Core/Inc -I../X-CUBE-TOF1/Target -I../Drivers/STM32WBxx_HAL_Driver/Inc -I../Drivers/STM32WBxx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32WBxx/Include -I../Drivers/CMSIS/Include -I../Drivers/BSP/Components/Common -I../Drivers/BSP/Components/vl53l1cb/modules -I../Drivers/BSP/Components/vl53l1cb/porting -I../Drivers/BSP/Components/vl53l1cb -I../Drivers/BSP/53L1A2 -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-BSP-2f-Components-2f-vl53l1cb

clean-Drivers-2f-BSP-2f-Components-2f-vl53l1cb:
	-$(RM) ./Drivers/BSP/Components/vl53l1cb/vl53l1cb.cyclo ./Drivers/BSP/Components/vl53l1cb/vl53l1cb.d ./Drivers/BSP/Components/vl53l1cb/vl53l1cb.o ./Drivers/BSP/Components/vl53l1cb/vl53l1cb.su

.PHONY: clean-Drivers-2f-BSP-2f-Components-2f-vl53l1cb

