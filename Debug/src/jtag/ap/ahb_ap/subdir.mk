################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/jtag/ap/ahb_ap/AHB_AP.cpp 

OBJS += \
./src/jtag/ap/ahb_ap/AHB_AP.o 

CPP_DEPS += \
./src/jtag/ap/ahb_ap/AHB_AP.d 


# Each subdirectory must supply rules for building sources it contributes
src/jtag/ap/ahb_ap/%.o: ../src/jtag/ap/ahb_ap/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++1y -fpic -I-lpthread -I-lusb-1.0 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


