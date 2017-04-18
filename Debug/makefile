################################################################################
# Automatically-generated file. Do not edit!
################################################################################

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk

-include src/test/subdir.mk

-include src/builder/subdir.mk
-include src/builder/jtag/subdir.mk
-include src/builder/inception/subdir.mk
-include src/builder/file_builder/subdir.mk

-include src/decoders/subdir.mk
-include src/decoders/jtag/subdir.mk
-include src/decoders/inception/subdir.mk
-include src/decoders/file_builder/subdir.mk

-include src/binutils/subdir.mk
-include src/interface/subdir.mk
-include src/device/subdir.mk
-include src/benchmark/subdir.mk
-include src/drivers/subdir.mk
-include src/drivers/flash/subdir.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk
-include src/trace/subdir.mk

# Add inputs and outputs from these tool invocations to the build variables

# All Target
all: inception

# Tool invocations
inception: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
	g++ -std=c++1y -o inception $(OBJS) $(USER_OBJS) $(LIBS)
	g++ -shared -Wl,-soname=inception -o "libinception.so" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(LIBRARIES)$(CC_DEPS)$(C++_DEPS)$(C_UPPER_DEPS)$(CXX_DEPS)$(OBJS)$(CPP_DEPS)$(C_DEPS) libinception.so
	-@echo ' '
