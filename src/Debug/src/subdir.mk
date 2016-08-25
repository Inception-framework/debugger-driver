################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
CPP_SRCS += \
../src/Decoder.cpp \
../src/superspeedjtag.cpp

# ../src/main.cpp \

OBJS += \
./src/Decoder.o \
./src/superspeedjtag.o

# ./src/main.o \

CPP_DEPS += \
./src/Decoder.d \
./src/superspeedjtag.d

# ./src/main.d \

# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	$(COMPILO) $(FLAGS) -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '
