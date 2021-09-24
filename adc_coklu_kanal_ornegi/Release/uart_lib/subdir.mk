################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../uart_lib/uart.c 

OBJS += \
./uart_lib/uart.o 

C_DEPS += \
./uart_lib/uart.d 


# Each subdirectory must supply rules for building sources it contributes
uart_lib/%.o: ../uart_lib/%.c uart_lib/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -I"/home/ahmet/Documents/avr_development/adc_coklu_kanal_denemesi/uart_lib" -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega328p -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


