################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/GitHub/Veridian-Heliograph/Firmware/Projects/Template1/Src/main.c \
C:/GitHub/Veridian-Heliograph/Firmware/Projects/Template1/Src/stm32wbxx_hal_msp.c \
C:/GitHub/Veridian-Heliograph/Firmware/Projects/Template1/Src/stm32wbxx_it.c \
../Application/User/syscalls.c \
../Application/User/sysmem.c 

OBJS += \
./Application/User/main.o \
./Application/User/stm32wbxx_hal_msp.o \
./Application/User/stm32wbxx_it.o \
./Application/User/syscalls.o \
./Application/User/sysmem.o 

C_DEPS += \
./Application/User/main.d \
./Application/User/stm32wbxx_hal_msp.d \
./Application/User/stm32wbxx_it.d \
./Application/User/syscalls.d \
./Application/User/sysmem.d 


# Each subdirectory must supply rules for building sources it contributes
Application/User/main.o: C:/GitHub/Veridian-Heliograph/Firmware/Projects/Template1/Src/main.c Application/User/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DDEBUG -DSTM32WB55xx -c -I../../Inc -I../../../../Drivers/STM32WBxx_HAL_Driver/Inc -I../../../../Drivers/STM32WBxx_HAL_Driver/Inc/Legacy -I../../../../Drivers/CMSIS/Device/ST/STM32WBxx/Include -I../../../../Drivers/CMSIS/Include -I../../../../Drivers/BSP/P-NUCLEO-WB55.Nucleo -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Application/User/stm32wbxx_hal_msp.o: C:/GitHub/Veridian-Heliograph/Firmware/Projects/Template1/Src/stm32wbxx_hal_msp.c Application/User/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DDEBUG -DSTM32WB55xx -c -I../../Inc -I../../../../Drivers/STM32WBxx_HAL_Driver/Inc -I../../../../Drivers/STM32WBxx_HAL_Driver/Inc/Legacy -I../../../../Drivers/CMSIS/Device/ST/STM32WBxx/Include -I../../../../Drivers/CMSIS/Include -I../../../../Drivers/BSP/P-NUCLEO-WB55.Nucleo -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Application/User/stm32wbxx_it.o: C:/GitHub/Veridian-Heliograph/Firmware/Projects/Template1/Src/stm32wbxx_it.c Application/User/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DDEBUG -DSTM32WB55xx -c -I../../Inc -I../../../../Drivers/STM32WBxx_HAL_Driver/Inc -I../../../../Drivers/STM32WBxx_HAL_Driver/Inc/Legacy -I../../../../Drivers/CMSIS/Device/ST/STM32WBxx/Include -I../../../../Drivers/CMSIS/Include -I../../../../Drivers/BSP/P-NUCLEO-WB55.Nucleo -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Application/User/%.o: ../Application/User/%.c Application/User/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DDEBUG -DSTM32WB55xx -c -I../../Inc -I../../../../Drivers/STM32WBxx_HAL_Driver/Inc -I../../../../Drivers/STM32WBxx_HAL_Driver/Inc/Legacy -I../../../../Drivers/CMSIS/Device/ST/STM32WBxx/Include -I../../../../Drivers/CMSIS/Include -I../../../../Drivers/BSP/P-NUCLEO-WB55.Nucleo -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Application-2f-User

clean-Application-2f-User:
	-$(RM) ./Application/User/main.d ./Application/User/main.o ./Application/User/stm32wbxx_hal_msp.d ./Application/User/stm32wbxx_hal_msp.o ./Application/User/stm32wbxx_it.d ./Application/User/stm32wbxx_it.o ./Application/User/syscalls.d ./Application/User/syscalls.o ./Application/User/sysmem.d ./Application/User/sysmem.o

.PHONY: clean-Application-2f-User

