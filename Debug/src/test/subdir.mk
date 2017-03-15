################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
CPP_SRCS += \
../src/test/TestReport.cpp \
../src/test/TestsFactory.cpp

OBJS += \
./src/test/TestReport.o \
./src/test/TestsFactory.o

CPP_DEPS += \
./src/test/TestReport.d \
./src/test/TestsFactory.d

# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++1y -fpic -I-lpthread -I-lusb-1.0 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '
