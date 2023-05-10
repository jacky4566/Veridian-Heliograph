################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/SharpMemLCD/LS013B7DH03.c 

OBJS += \
./Drivers/SharpMemLCD/LS013B7DH03.o 

C_DEPS += \
./Drivers/SharpMemLCD/LS013B7DH03.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/SharpMemLCD/%.o Drivers/SharpMemLCD/%.su Drivers/SharpMemLCD/%.cyclo: ../Drivers/SharpMemLCD/%.c Drivers/SharpMemLCD/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L432xx -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/SharpMemLCD -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I../Drivers/GNSSPVT -I../Drivers/nanoprintf -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-SharpMemLCD

clean-Drivers-2f-SharpMemLCD:
	-$(RM) ./Drivers/SharpMemLCD/LS013B7DH03.cyclo ./Drivers/SharpMemLCD/LS013B7DH03.d ./Drivers/SharpMemLCD/LS013B7DH03.o ./Drivers/SharpMemLCD/LS013B7DH03.su

.PHONY: clean-Drivers-2f-SharpMemLCD

