################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/BSP/53L1A2/53l1a2.c \
../Drivers/BSP/53L1A2/53l1a2_ranging_sensor.c 

OBJS += \
./Drivers/BSP/53L1A2/53l1a2.o \
./Drivers/BSP/53L1A2/53l1a2_ranging_sensor.o 

C_DEPS += \
./Drivers/BSP/53L1A2/53l1a2.d \
./Drivers/BSP/53L1A2/53l1a2_ranging_sensor.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP/53L1A2/%.o Drivers/BSP/53L1A2/%.su: ../Drivers/BSP/53L1A2/%.c Drivers/BSP/53L1A2/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32WB55xx -c -I../Core/Inc -I../X-CUBE-TOF1/Target -I../Drivers/STM32WBxx_HAL_Driver/Inc -I../Drivers/STM32WBxx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32WBxx/Include -I../Drivers/CMSIS/Include -I../Drivers/BSP/Components/Common -I../Drivers/BSP/Components/vl53l1cb/modules -I../Drivers/BSP/Components/vl53l1cb/porting -I../Drivers/BSP/Components/vl53l1cb -I../Drivers/BSP/53L1A2 -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-BSP-2f-53L1A2

clean-Drivers-2f-BSP-2f-53L1A2:
	-$(RM) ./Drivers/BSP/53L1A2/53l1a2.d ./Drivers/BSP/53L1A2/53l1a2.o ./Drivers/BSP/53L1A2/53l1a2.su ./Drivers/BSP/53L1A2/53l1a2_ranging_sensor.d ./Drivers/BSP/53L1A2/53l1a2_ranging_sensor.o ./Drivers/BSP/53L1A2/53l1a2_ranging_sensor.su

.PHONY: clean-Drivers-2f-BSP-2f-53L1A2

