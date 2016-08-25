################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
CPP_SRCS += \
../src/jtag/ap/jtag_ap/JtagAP.cpp

OBJS += \
./src/jtag/ap/jtag_ap/JtagAP.o

CPP_DEPS += \
./src/jtag/ap/jtag_ap/JtagAP.d

# Each subdirectory must supply rules for building sources it contributes
src/jtag/ap/jtag_ap/%.o: ../src/jtag/ap/jtag_ap/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	$(COMPILO) $(FLAGS) -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '
