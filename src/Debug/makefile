################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

FLAGS := -std=c++1y -fpic -I-lpthread -I-lusb-1.0 -O0 -g3 -Wall -rdynamic
COMPILO := g++

# All of the sources participating in the build are defined here
-include sources.mk
-include src/benchmark/subdir.mk
-include src/jtag/subdir.mk
-include src/interface/subdir.mk
-include src/device/subdir.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables

# All Target
all: SuperspeedJTAG

# Tool invocations
SuperspeedJTAG: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
	g++ -rdynamic -shared -Wl,-soname=superspeed-jtag -o "libsuperspeed-jtag.so" $(OBJS) $(USER_OBJS) $(LIBS)
  #g++ -shared -Wl,-soname,libavatar-jtag.so.1 -o libavatar-jtag.so.1.0 $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	#ln -sf libavatar-jtag.so.1.0 libavatar-jtag.so
	#ln -sf libavatar-jtag.so.1.0 libavatar-jtag.so.1
	@echo ' '

# Other Targets
clean:
	-$(RM) $(CC_DEPS)$(C++_DEPS)$(EXECUTABLES)$(C_UPPER_DEPS)$(CXX_DEPS)$(OBJS)$(CPP_DEPS)$(C_DEPS) SuperspeedJTAG
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
