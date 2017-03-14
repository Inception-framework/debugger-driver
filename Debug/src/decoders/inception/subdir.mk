################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
CPP_SRCS += \
../src/decoders/inception/InceptionDecoder.cpp

OBJS += \
./src/decoders/inception/InceptionDecoder.o

CPP_DEPS += \
./src/decoders/inception/InceptionDecoder.d

# Each subdirectory must supply rules for building sources it contributes
src/decoders/inception/%.o: ../src/decoders/inception/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++1y -fpic -I-lpthread -I-lusb-1.0 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '
