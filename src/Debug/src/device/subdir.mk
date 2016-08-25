################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
CPP_SRCS += \
../src/device/Device.cpp

OBJS += \
./src/device/Device.o

CPP_DEPS += \
./src/device/Device.d


# Each subdirectory must supply rules for building sources it contributes
src/device/%.o: ../src/device/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	$(COMPILO) $(FLAGS)  -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '
