################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../L5_Application/source/cmd_handlers/handlers.cpp \
../L5_Application/source/cmd_handlers/prog_handlers.cpp \
../L5_Application/source/cmd_handlers/wireless_handlers.cpp 

OBJS += \
./L5_Application/source/cmd_handlers/handlers.o \
./L5_Application/source/cmd_handlers/prog_handlers.o \
./L5_Application/source/cmd_handlers/wireless_handlers.o 

CPP_DEPS += \
./L5_Application/source/cmd_handlers/handlers.d \
./L5_Application/source/cmd_handlers/prog_handlers.d \
./L5_Application/source/cmd_handlers/wireless_handlers.d 


# Each subdirectory must supply rules for building sources it contributes
L5_Application/source/cmd_handlers/%.o: ../L5_Application/source/cmd_handlers/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C++ Compiler'
	arm-none-eabi-g++ -mcpu=cortex-m3 -mthumb -Os -fmessage-length=0 -ffunction-sections -fdata-sections -Wall -Wshadow -Wlogical-op -Wfloat-equal -DBUILD_CFG_MPU=0 -I"C:\Users\Jaswa\Team_AutoNav\lpc1758_freertos" -I"C:\Users\Jaswa\Team_AutoNav\lpc1758_freertos\newlib" -I"C:\Users\Jaswa\Team_AutoNav\lpc1758_freertos\L0_LowLevel" -I"C:\Users\Jaswa\Team_AutoNav\lpc1758_freertos\L1_FreeRTOS" -I"C:\Users\Jaswa\Team_AutoNav\lpc1758_freertos\L1_FreeRTOS\include" -I"C:\Users\Jaswa\Team_AutoNav\lpc1758_freertos\L1_FreeRTOS\portable" -I"C:\Users\Jaswa\Team_AutoNav\lpc1758_freertos\L1_FreeRTOS\portable\no_mpu" -I"C:\Users\Jaswa\Team_AutoNav\lpc1758_freertos\L2_Drivers" -I"C:\Users\Jaswa\Team_AutoNav\lpc1758_freertos\L2_Drivers\base" -I"C:\Users\Jaswa\Team_AutoNav\lpc1758_freertos\L3_Utils" -I"C:\Users\Jaswa\Team_AutoNav\lpc1758_freertos\L3_Utils\tlm" -I"C:\Users\Jaswa\Team_AutoNav\lpc1758_freertos\L4_IO" -I"C:\Users\Jaswa\Team_AutoNav\lpc1758_freertos\L4_IO\fat" -I"C:\Users\Jaswa\Team_AutoNav\lpc1758_freertos\L4_IO\wireless" -I"C:\Users\Jaswa\Team_AutoNav\lpc1758_freertos\L5_Application" -std=gnu++11 -fabi-version=0 -fno-exceptions -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


