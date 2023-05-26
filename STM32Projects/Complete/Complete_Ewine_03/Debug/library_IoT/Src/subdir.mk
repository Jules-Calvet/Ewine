################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../library_IoT/Src/MQTTConnectClient.c \
../library_IoT/Src/MQTTConnectServer.c \
../library_IoT/Src/MQTTDeserializePublish.c \
../library_IoT/Src/MQTTFormat.c \
../library_IoT/Src/MQTTPacket.c \
../library_IoT/Src/MQTTSerializePublish.c \
../library_IoT/Src/MQTTSubscribeClient.c \
../library_IoT/Src/MQTTSubscribeServer.c \
../library_IoT/Src/MQTTUnsubscribeClient.c \
../library_IoT/Src/MQTTUnsubscribeServer.c \
../library_IoT/Src/cJSON.c 

OBJS += \
./library_IoT/Src/MQTTConnectClient.o \
./library_IoT/Src/MQTTConnectServer.o \
./library_IoT/Src/MQTTDeserializePublish.o \
./library_IoT/Src/MQTTFormat.o \
./library_IoT/Src/MQTTPacket.o \
./library_IoT/Src/MQTTSerializePublish.o \
./library_IoT/Src/MQTTSubscribeClient.o \
./library_IoT/Src/MQTTSubscribeServer.o \
./library_IoT/Src/MQTTUnsubscribeClient.o \
./library_IoT/Src/MQTTUnsubscribeServer.o \
./library_IoT/Src/cJSON.o 

C_DEPS += \
./library_IoT/Src/MQTTConnectClient.d \
./library_IoT/Src/MQTTConnectServer.d \
./library_IoT/Src/MQTTDeserializePublish.d \
./library_IoT/Src/MQTTFormat.d \
./library_IoT/Src/MQTTPacket.d \
./library_IoT/Src/MQTTSerializePublish.d \
./library_IoT/Src/MQTTSubscribeClient.d \
./library_IoT/Src/MQTTSubscribeServer.d \
./library_IoT/Src/MQTTUnsubscribeClient.d \
./library_IoT/Src/MQTTUnsubscribeServer.d \
./library_IoT/Src/cJSON.d 


# Each subdirectory must supply rules for building sources it contributes
library_IoT/Src/%.o library_IoT/Src/%.su library_IoT/Src/%.cyclo: ../library_IoT/Src/%.c library_IoT/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32WB55xx -c -I../Core/Inc -I../Drivers/STM32WBxx_HAL_Driver/Inc -I../Drivers/STM32WBxx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32WBxx/Include -I../Drivers/CMSIS/Include -I../TOF/App -I../TOF/Target -I../Drivers/BSP/STM32WBxx_Nucleo -I../Drivers/BSP/Components/Common -I../Drivers/BSP/Components/vl53l1cb/modules -I../Drivers/BSP/Components/vl53l1cb/porting -I../Drivers/BSP/Components/vl53l1cb -I../Drivers/BSP/53L1A2 -I"C:/Users/Justin/Desktop/Ewine/Ewine/STM32Projects/Complete/Complete_Ewine_03/ESP32/Inc" -I"C:/Users/Justin/Desktop/Ewine/Ewine/STM32Projects/Complete/Complete_Ewine_03/library_IoT/Inc" -I"C:/Users/Justin/Desktop/Ewine/Ewine/STM32Projects/Complete/Complete_Ewine_03/IR/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-library_IoT-2f-Src

clean-library_IoT-2f-Src:
	-$(RM) ./library_IoT/Src/MQTTConnectClient.cyclo ./library_IoT/Src/MQTTConnectClient.d ./library_IoT/Src/MQTTConnectClient.o ./library_IoT/Src/MQTTConnectClient.su ./library_IoT/Src/MQTTConnectServer.cyclo ./library_IoT/Src/MQTTConnectServer.d ./library_IoT/Src/MQTTConnectServer.o ./library_IoT/Src/MQTTConnectServer.su ./library_IoT/Src/MQTTDeserializePublish.cyclo ./library_IoT/Src/MQTTDeserializePublish.d ./library_IoT/Src/MQTTDeserializePublish.o ./library_IoT/Src/MQTTDeserializePublish.su ./library_IoT/Src/MQTTFormat.cyclo ./library_IoT/Src/MQTTFormat.d ./library_IoT/Src/MQTTFormat.o ./library_IoT/Src/MQTTFormat.su ./library_IoT/Src/MQTTPacket.cyclo ./library_IoT/Src/MQTTPacket.d ./library_IoT/Src/MQTTPacket.o ./library_IoT/Src/MQTTPacket.su ./library_IoT/Src/MQTTSerializePublish.cyclo ./library_IoT/Src/MQTTSerializePublish.d ./library_IoT/Src/MQTTSerializePublish.o ./library_IoT/Src/MQTTSerializePublish.su ./library_IoT/Src/MQTTSubscribeClient.cyclo ./library_IoT/Src/MQTTSubscribeClient.d ./library_IoT/Src/MQTTSubscribeClient.o ./library_IoT/Src/MQTTSubscribeClient.su ./library_IoT/Src/MQTTSubscribeServer.cyclo ./library_IoT/Src/MQTTSubscribeServer.d ./library_IoT/Src/MQTTSubscribeServer.o ./library_IoT/Src/MQTTSubscribeServer.su ./library_IoT/Src/MQTTUnsubscribeClient.cyclo ./library_IoT/Src/MQTTUnsubscribeClient.d ./library_IoT/Src/MQTTUnsubscribeClient.o ./library_IoT/Src/MQTTUnsubscribeClient.su ./library_IoT/Src/MQTTUnsubscribeServer.cyclo ./library_IoT/Src/MQTTUnsubscribeServer.d ./library_IoT/Src/MQTTUnsubscribeServer.o ./library_IoT/Src/MQTTUnsubscribeServer.su ./library_IoT/Src/cJSON.cyclo ./library_IoT/Src/cJSON.d ./library_IoT/Src/cJSON.o ./library_IoT/Src/cJSON.su

.PHONY: clean-library_IoT-2f-Src

