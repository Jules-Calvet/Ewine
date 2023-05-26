################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../IR/Src/app_IR.c \
../IR/Src/mlx90614.c 

OBJS += \
./IR/Src/app_IR.o \
./IR/Src/mlx90614.o 

C_DEPS += \
./IR/Src/app_IR.d \
./IR/Src/mlx90614.d 


# Each subdirectory must supply rules for building sources it contributes
IR/Src/%.o IR/Src/%.su IR/Src/%.cyclo: ../IR/Src/%.c IR/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32WB55xx -c -I../Core/Inc -I../Drivers/STM32WBxx_HAL_Driver/Inc -I../Drivers/STM32WBxx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32WBxx/Include -I../Drivers/CMSIS/Include -I../TOF/App -I../TOF/Target -I../Drivers/BSP/STM32WBxx_Nucleo -I../Drivers/BSP/Components/Common -I../Drivers/BSP/Components/vl53l1cb/modules -I../Drivers/BSP/Components/vl53l1cb/porting -I../Drivers/BSP/Components/vl53l1cb -I../Drivers/BSP/53L1A2 -I"C:/Users/Justin/Desktop/Ewine/Ewine/STM32Projects/Complete/Complete_Ewine_03/ESP32/Inc" -I"C:/Users/Justin/Desktop/Ewine/Ewine/STM32Projects/Complete/Complete_Ewine_03/library_IoT/Inc" -I"C:/Users/Justin/Desktop/Ewine/Ewine/STM32Projects/Complete/Complete_Ewine_03/IR/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-IR-2f-Src

clean-IR-2f-Src:
	-$(RM) ./IR/Src/app_IR.cyclo ./IR/Src/app_IR.d ./IR/Src/app_IR.o ./IR/Src/app_IR.su ./IR/Src/mlx90614.cyclo ./IR/Src/mlx90614.d ./IR/Src/mlx90614.o ./IR/Src/mlx90614.su

.PHONY: clean-IR-2f-Src

