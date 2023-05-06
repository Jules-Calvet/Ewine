################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../library_Iot/Src/MQTTConnectClient.c \
../library_Iot/Src/MQTTConnectServer.c \
../library_Iot/Src/MQTTDeserializePublish.c \
../library_Iot/Src/MQTTFormat.c \
../library_Iot/Src/MQTTPacket.c \
../library_Iot/Src/MQTTSerializePublish.c \
../library_Iot/Src/MQTTSubscribeClient.c \
../library_Iot/Src/MQTTSubscribeServer.c \
../library_Iot/Src/MQTTUnsubscribeClient.c \
../library_Iot/Src/MQTTUnsubscribeServer.c \
../library_Iot/Src/cJSON.c 

OBJS += \
./library_Iot/Src/MQTTConnectClient.o \
./library_Iot/Src/MQTTConnectServer.o \
./library_Iot/Src/MQTTDeserializePublish.o \
./library_Iot/Src/MQTTFormat.o \
./library_Iot/Src/MQTTPacket.o \
./library_Iot/Src/MQTTSerializePublish.o \
./library_Iot/Src/MQTTSubscribeClient.o \
./library_Iot/Src/MQTTSubscribeServer.o \
./library_Iot/Src/MQTTUnsubscribeClient.o \
./library_Iot/Src/MQTTUnsubscribeServer.o \
./library_Iot/Src/cJSON.o 

C_DEPS += \
./library_Iot/Src/MQTTConnectClient.d \
./library_Iot/Src/MQTTConnectServer.d \
./library_Iot/Src/MQTTDeserializePublish.d \
./library_Iot/Src/MQTTFormat.d \
./library_Iot/Src/MQTTPacket.d \
./library_Iot/Src/MQTTSerializePublish.d \
./library_Iot/Src/MQTTSubscribeClient.d \
./library_Iot/Src/MQTTSubscribeServer.d \
./library_Iot/Src/MQTTUnsubscribeClient.d \
./library_Iot/Src/MQTTUnsubscribeServer.d \
./library_Iot/Src/cJSON.d 


# Each subdirectory must supply rules for building sources it contributes
library_Iot/Src/%.o library_Iot/Src/%.su library_Iot/Src/%.cyclo: ../library_Iot/Src/%.c library_Iot/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32WB55xx -c -I../Core/Inc -I../Drivers/STM32WBxx_HAL_Driver/Inc -I../Drivers/STM32WBxx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32WBxx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Justin/Desktop/Ewine/Ewine/STM32Projects/Wifi/Test_Wifi_07/ESP32/Inc" -I"C:/Users/Justin/Desktop/Ewine/Ewine/STM32Projects/Wifi/Test_Wifi_07/library_Iot/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-library_Iot-2f-Src

clean-library_Iot-2f-Src:
	-$(RM) ./library_Iot/Src/MQTTConnectClient.cyclo ./library_Iot/Src/MQTTConnectClient.d ./library_Iot/Src/MQTTConnectClient.o ./library_Iot/Src/MQTTConnectClient.su ./library_Iot/Src/MQTTConnectServer.cyclo ./library_Iot/Src/MQTTConnectServer.d ./library_Iot/Src/MQTTConnectServer.o ./library_Iot/Src/MQTTConnectServer.su ./library_Iot/Src/MQTTDeserializePublish.cyclo ./library_Iot/Src/MQTTDeserializePublish.d ./library_Iot/Src/MQTTDeserializePublish.o ./library_Iot/Src/MQTTDeserializePublish.su ./library_Iot/Src/MQTTFormat.cyclo ./library_Iot/Src/MQTTFormat.d ./library_Iot/Src/MQTTFormat.o ./library_Iot/Src/MQTTFormat.su ./library_Iot/Src/MQTTPacket.cyclo ./library_Iot/Src/MQTTPacket.d ./library_Iot/Src/MQTTPacket.o ./library_Iot/Src/MQTTPacket.su ./library_Iot/Src/MQTTSerializePublish.cyclo ./library_Iot/Src/MQTTSerializePublish.d ./library_Iot/Src/MQTTSerializePublish.o ./library_Iot/Src/MQTTSerializePublish.su ./library_Iot/Src/MQTTSubscribeClient.cyclo ./library_Iot/Src/MQTTSubscribeClient.d ./library_Iot/Src/MQTTSubscribeClient.o ./library_Iot/Src/MQTTSubscribeClient.su ./library_Iot/Src/MQTTSubscribeServer.cyclo ./library_Iot/Src/MQTTSubscribeServer.d ./library_Iot/Src/MQTTSubscribeServer.o ./library_Iot/Src/MQTTSubscribeServer.su ./library_Iot/Src/MQTTUnsubscribeClient.cyclo ./library_Iot/Src/MQTTUnsubscribeClient.d ./library_Iot/Src/MQTTUnsubscribeClient.o ./library_Iot/Src/MQTTUnsubscribeClient.su ./library_Iot/Src/MQTTUnsubscribeServer.cyclo ./library_Iot/Src/MQTTUnsubscribeServer.d ./library_Iot/Src/MQTTUnsubscribeServer.o ./library_Iot/Src/MQTTUnsubscribeServer.su ./library_Iot/Src/cJSON.cyclo ./library_Iot/Src/cJSON.d ./library_Iot/Src/cJSON.o ./library_Iot/Src/cJSON.su

.PHONY: clean-library_Iot-2f-Src

