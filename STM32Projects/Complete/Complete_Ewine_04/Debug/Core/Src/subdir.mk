################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/app_debug.c \
../Core/Src/app_entry.c \
../Core/Src/bsp.c \
../Core/Src/dma.c \
../Core/Src/gpio.c \
../Core/Src/hw_timerserver.c \
../Core/Src/hw_uart.c \
../Core/Src/i2c.c \
../Core/Src/ipcc.c \
../Core/Src/main.c \
../Core/Src/parson.c \
../Core/Src/rf.c \
../Core/Src/rtc.c \
../Core/Src/stm32_lpm_if.c \
../Core/Src/stm32wbxx_hal_msp.c \
../Core/Src/stm32wbxx_it.c \
../Core/Src/stm32wbxx_nucleo_bus.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32wbxx.c \
../Core/Src/usart.c \
../Core/Src/usb.c 

OBJS += \
./Core/Src/app_debug.o \
./Core/Src/app_entry.o \
./Core/Src/bsp.o \
./Core/Src/dma.o \
./Core/Src/gpio.o \
./Core/Src/hw_timerserver.o \
./Core/Src/hw_uart.o \
./Core/Src/i2c.o \
./Core/Src/ipcc.o \
./Core/Src/main.o \
./Core/Src/parson.o \
./Core/Src/rf.o \
./Core/Src/rtc.o \
./Core/Src/stm32_lpm_if.o \
./Core/Src/stm32wbxx_hal_msp.o \
./Core/Src/stm32wbxx_it.o \
./Core/Src/stm32wbxx_nucleo_bus.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32wbxx.o \
./Core/Src/usart.o \
./Core/Src/usb.o 

C_DEPS += \
./Core/Src/app_debug.d \
./Core/Src/app_entry.d \
./Core/Src/bsp.d \
./Core/Src/dma.d \
./Core/Src/gpio.d \
./Core/Src/hw_timerserver.d \
./Core/Src/hw_uart.d \
./Core/Src/i2c.d \
./Core/Src/ipcc.d \
./Core/Src/main.d \
./Core/Src/parson.d \
./Core/Src/rf.d \
./Core/Src/rtc.d \
./Core/Src/stm32_lpm_if.d \
./Core/Src/stm32wbxx_hal_msp.d \
./Core/Src/stm32wbxx_it.d \
./Core/Src/stm32wbxx_nucleo_bus.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32wbxx.d \
./Core/Src/usart.d \
./Core/Src/usb.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32WB55xx -c -I../Core/Inc -I../STM32_WPAN/App -I../Drivers/STM32WBxx_HAL_Driver/Inc -I../Drivers/STM32WBxx_HAL_Driver/Inc/Legacy -I../Utilities/lpm/tiny_lpm -I../Middlewares/ST/STM32_WPAN -I../Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread -I../Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/tl -I../Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/shci -I../Middlewares/ST/STM32_WPAN/utilities -I../Middlewares/ST/STM32_WPAN/ble/core -I../Middlewares/ST/STM32_WPAN/ble/core/auto -I../Middlewares/ST/STM32_WPAN/ble/core/template -I../Middlewares/ST/STM32_WPAN/ble/svc/Inc -I../Middlewares/ST/STM32_WPAN/ble/svc/Src -I../Drivers/CMSIS/Device/ST/STM32WBxx/Include -I../Utilities/sequencer -I../Middlewares/ST/STM32_WPAN/ble -I../Drivers/CMSIS/Include -I"C:/Users/Justin/Desktop/Ewine/Ewine/STM32Projects/Complete/Complete_Ewine_04/library_IoT/Inc" -I"C:/Users/Justin/Desktop/Ewine/Ewine/STM32Projects/Complete/Complete_Ewine_04/ESP32/Inc" -I../TOF/App -I../TOF/Target -I../Drivers/BSP/STM32WBxx_Nucleo -I../Drivers/BSP/Components/Common -I../Drivers/BSP/Components/vl53l1cb/modules -I../Drivers/BSP/Components/vl53l1cb/porting -I../Drivers/BSP/Components/vl53l1cb -I../Drivers/BSP/53L1A2 -I"C:/Users/Justin/Desktop/Ewine/Ewine/STM32Projects/Complete/Complete_Ewine_04/IR/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/app_debug.cyclo ./Core/Src/app_debug.d ./Core/Src/app_debug.o ./Core/Src/app_debug.su ./Core/Src/app_entry.cyclo ./Core/Src/app_entry.d ./Core/Src/app_entry.o ./Core/Src/app_entry.su ./Core/Src/bsp.cyclo ./Core/Src/bsp.d ./Core/Src/bsp.o ./Core/Src/bsp.su ./Core/Src/dma.cyclo ./Core/Src/dma.d ./Core/Src/dma.o ./Core/Src/dma.su ./Core/Src/gpio.cyclo ./Core/Src/gpio.d ./Core/Src/gpio.o ./Core/Src/gpio.su ./Core/Src/hw_timerserver.cyclo ./Core/Src/hw_timerserver.d ./Core/Src/hw_timerserver.o ./Core/Src/hw_timerserver.su ./Core/Src/hw_uart.cyclo ./Core/Src/hw_uart.d ./Core/Src/hw_uart.o ./Core/Src/hw_uart.su ./Core/Src/i2c.cyclo ./Core/Src/i2c.d ./Core/Src/i2c.o ./Core/Src/i2c.su ./Core/Src/ipcc.cyclo ./Core/Src/ipcc.d ./Core/Src/ipcc.o ./Core/Src/ipcc.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/parson.cyclo ./Core/Src/parson.d ./Core/Src/parson.o ./Core/Src/parson.su ./Core/Src/rf.cyclo ./Core/Src/rf.d ./Core/Src/rf.o ./Core/Src/rf.su ./Core/Src/rtc.cyclo ./Core/Src/rtc.d ./Core/Src/rtc.o ./Core/Src/rtc.su ./Core/Src/stm32_lpm_if.cyclo ./Core/Src/stm32_lpm_if.d ./Core/Src/stm32_lpm_if.o ./Core/Src/stm32_lpm_if.su ./Core/Src/stm32wbxx_hal_msp.cyclo ./Core/Src/stm32wbxx_hal_msp.d ./Core/Src/stm32wbxx_hal_msp.o ./Core/Src/stm32wbxx_hal_msp.su ./Core/Src/stm32wbxx_it.cyclo ./Core/Src/stm32wbxx_it.d ./Core/Src/stm32wbxx_it.o ./Core/Src/stm32wbxx_it.su ./Core/Src/stm32wbxx_nucleo_bus.cyclo ./Core/Src/stm32wbxx_nucleo_bus.d ./Core/Src/stm32wbxx_nucleo_bus.o ./Core/Src/stm32wbxx_nucleo_bus.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32wbxx.cyclo ./Core/Src/system_stm32wbxx.d ./Core/Src/system_stm32wbxx.o ./Core/Src/system_stm32wbxx.su ./Core/Src/usart.cyclo ./Core/Src/usart.d ./Core/Src/usart.o ./Core/Src/usart.su ./Core/Src/usb.cyclo ./Core/Src/usb.d ./Core/Src/usb.o ./Core/Src/usb.su

.PHONY: clean-Core-2f-Src

