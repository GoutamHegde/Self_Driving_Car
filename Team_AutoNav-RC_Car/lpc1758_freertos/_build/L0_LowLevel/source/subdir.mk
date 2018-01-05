################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../L0_LowLevel/source/low_level_init.cpp \
../L0_LowLevel/source/lpc_sys.cpp \
../L0_LowLevel/source/startup.cpp \
../L0_LowLevel/source/sys_clock.cpp 

C_SRCS += \
../L0_LowLevel/source/core_cm3.c \
../L0_LowLevel/source/lpc_peripherals.c \
../L0_LowLevel/source/uart0_min.c 

OBJS += \
./L0_LowLevel/source/core_cm3.o \
./L0_LowLevel/source/low_level_init.o \
./L0_LowLevel/source/lpc_peripherals.o \
./L0_LowLevel/source/lpc_sys.o \
./L0_LowLevel/source/startup.o \
./L0_LowLevel/source/sys_clock.o \
./L0_LowLevel/source/uart0_min.o 

C_DEPS += \
./L0_LowLevel/source/core_cm3.d \
./L0_LowLevel/source/lpc_peripherals.d \
./L0_LowLevel/source/uart0_min.d 

CPP_DEPS += \
./L0_LowLevel/source/low_level_init.d \
./L0_LowLevel/source/lpc_sys.d \
./L0_LowLevel/source/startup.d \
./L0_LowLevel/source/sys_clock.d 


# Each subdirectory must supply rules for building sources it contributes
L0_LowLevel/source/%.o: ../L0_LowLevel/source/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -Os -fmessage-length=0 -ffunction-sections -fdata-sections -Wall -Wshadow -Wlogical-op -Wfloat-equal -DBUILD_CFG_MPU=0 -I"C:\Users\Jaswa\Team_AutoNav\lpc1758_freertos" -I"C:\Users\Jaswa\Team_AutoNav\lpc1758_freertos\newlib" -I"C:\Users\Jaswa\Team_AutoNav\lpc1758_freertos\L0_LowLevel" -I"C:\Users\Jaswa\Team_AutoNav\lpc1758_freertos\L1_FreeRTOS" -I"C:\Users\Jaswa\Team_AutoNav\lpc1758_freertos\L1_FreeRTOS\include" -I"C:\Users\Jaswa\Team_AutoNav\lpc1758_freertos\L1_FreeRTOS\portable\no_mpu" -I"C:\Users\Jaswa\Team_AutoNav\lpc1758_freertos\L2_Drivers" -I"C:\Users\Jaswa\Team_AutoNav\lpc1758_freertos\L2_Drivers\base" -I"C:\Users\Jaswa\Team_AutoNav\lpc1758_freertos\L3_Utils" -I"C:\Users\Jaswa\Team_AutoNav\lpc1758_freertos\L3_Utils\tlm" -I"C:\Users\Jaswa\Team_AutoNav\lpc1758_freertos\L4_IO" -I"C:\Users\Jaswa\Team_AutoNav\lpc1758_freertos\L4_IO\fat" -I"C:\Users\Jaswa\Team_AutoNav\lpc1758_freertos\L4_IO\wireless" -I"C:\Users\Jaswa\Team_AutoNav\lpc1758_freertos\L5_Application" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

L0_LowLevel/source/%.o: ../L0_LowLevel/source/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C++ Compiler'
	arm-none-eabi-g++ -mcpu=cortex-m3 -mthumb -Os -fmessage-length=0 -ffunction-sections -fdata-sections -Wall -Wshadow -Wlogical-op -Wfloat-equal -DBUILD_CFG_MPU=0 -I"C:\Users\Jaswa\Team_AutoNav\lpc1758_freertos" -I"C:\Users\Jaswa\Team_AutoNav\lpc1758_freertos\newlib" -I"C:\Users\Jaswa\Team_AutoNav\lpc1758_freertos\L0_LowLevel" -I"C:\Users\Jaswa\Team_AutoNav\lpc1758_freertos\L1_FreeRTOS" -I"C:\Users\Jaswa\Team_AutoNav\lpc1758_freertos\L1_FreeRTOS\include" -I"C:\Users\Jaswa\Team_AutoNav\lpc1758_freertos\L1_FreeRTOS\portable" -I"C:\Users\Jaswa\Team_AutoNav\lpc1758_freertos\L1_FreeRTOS\portable\no_mpu" -I"C:\Users\Jaswa\Team_AutoNav\lpc1758_freertos\L2_Drivers" -I"C:\Users\Jaswa\Team_AutoNav\lpc1758_freertos\L2_Drivers\base" -I"C:\Users\Jaswa\Team_AutoNav\lpc1758_freertos\L3_Utils" -I"C:\Users\Jaswa\Team_AutoNav\lpc1758_freertos\L3_Utils\tlm" -I"C:\Users\Jaswa\Team_AutoNav\lpc1758_freertos\L4_IO" -I"C:\Users\Jaswa\Team_AutoNav\lpc1758_freertos\L4_IO\fat" -I"C:\Users\Jaswa\Team_AutoNav\lpc1758_freertos\L4_IO\wireless" -I"C:\Users\Jaswa\Team_AutoNav\lpc1758_freertos\L5_Application" -std=gnu++11 -fabi-version=0 -fno-exceptions -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


