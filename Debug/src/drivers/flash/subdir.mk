################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
CPP_SRCS += \
../src/drivers/flash/flash.cpp \
../src/drivers/flash/max32550_flash_driver.cpp

OBJS += \
./src/drivers/flash/flash.o \
./src/drivers/flash/max32550_flash_driver.o

CPP_DEPS += \
./src/drivers/flash/flash.d \
./src/drivers/flash/max32550_flash_driver.d

# Each subdirectory must supply rules for building sources it contributes
src/drivers/flash/%.o: ../src/drivers/flash/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++1y -fpic -I-lpthread -I-lusb-1.0 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '
