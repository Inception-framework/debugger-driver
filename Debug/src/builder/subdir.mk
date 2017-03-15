################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/builder/Command.cpp \
../src/builder/CommandsFactory.cpp \
../src/builder/Jtag.cpp \
../src/builder/TDO.cpp

OBJS += \
./src/builder/Command.o \
./src/builder/CommandsFactory.o \
./src/builder/Jtag.o \
./src/builder/TDO.o

CPP_DEPS += \
./src/builder/Command.d \
./src/builder/CommandsFactory.d \
./src/builder/Jtag.d \
./src/builder/TDO.d


# Each subdirectory must supply rules for building sources it contributes
src/jtag/%.o: ../src/jtag/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++1y -fpic -I-lpthread -I-lusb-1.0 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


