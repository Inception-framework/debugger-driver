################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/jtag/Command.cpp \
../src/jtag/Command_test.cpp \
../src/jtag/CommandsFactory.cpp \
../src/jtag/Jtag.cpp \
../src/jtag/TDO.cpp \
../src/jtag/JTAGBuilder.cpp

OBJS += \
./src/jtag/Command.o \
./src/jtag/Command_test.o \
./src/jtag/CommandsFactory.o \
./src/jtag/Jtag.o \
./src/jtag/TDO.o \
./src/jtag/JTAGBuilder.o

CPP_DEPS += \
./src/jtag/Command.d \
./src/jtag/Command_test.d \
./src/jtag/CommandsFactory.d \
./src/jtag/Jtag.d \
./src/jtag/TDO.d \
./src/jtag/JTAGBuilder.d


# Each subdirectory must supply rules for building sources it contributes
src/jtag/%.o: ../src/jtag/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++1y -fpic -I-lpthread -I-lusb-1.0 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


