################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../L4_IO/fat/option/ccsbcs.c \
../L4_IO/fat/option/reentrant.c 

OBJS += \
./L4_IO/fat/option/ccsbcs.o \
./L4_IO/fat/option/reentrant.o 

C_DEPS += \
./L4_IO/fat/option/ccsbcs.d \
./L4_IO/fat/option/reentrant.d 


# Each subdirectory must supply rules for building sources it contributes
L4_IO/fat/option/%.o: ../L4_IO/fat/option/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -Os -fmessage-length=0 -ffunction-sections -fdata-sections -Wall -Wshadow -Wlogical-op -Wfloat-equal -DBUILD_CFG_MPU=0 -I"C:\Users\Jaswa\Team_AutoNav\lpc1758_freertos" -I"C:\Users\Jaswa\Team_AutoNav\lpc1758_freertos\newlib" -I"C:\Users\Jaswa\Team_AutoNav\lpc1758_freertos\L0_LowLevel" -I"C:\Users\Jaswa\Team_AutoNav\lpc1758_freertos\L1_FreeRTOS" -I"C:\Users\Jaswa\Team_AutoNav\lpc1758_freertos\L1_FreeRTOS\include" -I"C:\Users\Jaswa\Team_AutoNav\lpc1758_freertos\L1_FreeRTOS\portable\no_mpu" -I"C:\Users\Jaswa\Team_AutoNav\lpc1758_freertos\L2_Drivers" -I"C:\Users\Jaswa\Team_AutoNav\lpc1758_freertos\L2_Drivers\base" -I"C:\Users\Jaswa\Team_AutoNav\lpc1758_freertos\L3_Utils" -I"C:\Users\Jaswa\Team_AutoNav\lpc1758_freertos\L3_Utils\tlm" -I"C:\Users\Jaswa\Team_AutoNav\lpc1758_freertos\L4_IO" -I"C:\Users\Jaswa\Team_AutoNav\lpc1758_freertos\L4_IO\fat" -I"C:\Users\Jaswa\Team_AutoNav\lpc1758_freertos\L4_IO\wireless" -I"C:\Users\Jaswa\Team_AutoNav\lpc1758_freertos\L5_Application" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

