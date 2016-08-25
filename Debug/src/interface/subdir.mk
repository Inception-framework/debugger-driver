################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/interface/Consumer.cpp \
../src/interface/Consumer_test.cpp \
../src/interface/Interface.cpp \
../src/interface/Producer.cpp \
../src/interface/Producer_test.cpp 

OBJS += \
./src/interface/Consumer.o \
./src/interface/Consumer_test.o \
./src/interface/Interface.o \
./src/interface/Producer.o \
./src/interface/Producer_test.o 

CPP_DEPS += \
./src/interface/Consumer.d \
./src/interface/Consumer_test.d \
./src/interface/Interface.d \
./src/interface/Producer.d \
./src/interface/Producer_test.d 


# Each subdirectory must supply rules for building sources it contributes
src/interface/%.o: ../src/interface/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++1y -fpic -I-lpthread -I-lusb-1.0 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


