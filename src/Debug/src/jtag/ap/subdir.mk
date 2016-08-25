################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
CPP_SRCS += \
../src/jtag/ap/AccessPort.cpp

OBJS += \
./src/jtag/ap/AccessPort.o

CPP_DEPS += \
./src/jtag/ap/AccessPort.d

-include src/jtag/ap/ahb_ap/subdir.mk
-include src/jtag/ap/jtag_ap/subdir.mk

# Each subdirectory must supply rules for building sources it contributes
src/jtag/ap/%.o: ../src/jtag/ap/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	$(COMPILO) $(FLAGS) -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '
