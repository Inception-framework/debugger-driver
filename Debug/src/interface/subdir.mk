################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/interface/Consumer.cpp \
../src/interface/Interface.cpp \
../src/interface/Producer.cpp

OBJS += \
./src/interface/Consumer.o \
./src/interface/Interface.o \
./src/interface/Producer.o

CPP_DEPS += \
./src/interface/Consumer.d \
./src/interface/Interface.d \
./src/interface/Producer.d


# Each subdirectory must supply rules for building sources it contributes
src/interface/%.o: ../src/interface/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++1y -fpic -I-lpthread -I-lusb-1.0 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


