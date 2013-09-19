################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk
ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: meecastservice.exe secondary-outputs

# Tool invocations
meecastservice.exe: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: C++ Linker'
	clang++ -o"meecastservice.exe" $(OBJS) $(USER_OBJS) $(LIBS) -target i386-tizen-linux-gnueabi -gcc-toolchain /home/vlad/tizen-sdk/tools/smart-build-interface/../i386-linux-gnueabi-gcc-4.5/ -ccc-gcc-name i386-linux-gnueabi-g++ -march=i386 -Xlinker --as-needed -pie -lpthread -Xlinker -rpath="/opt/usr/apps/C8xIFTMoRh/lib" -Xlinker -rpath="/home/developer/sdk_tools/lib" --sysroot="/home/vlad/tizen-sdk/platforms/tizen2.1/rootstraps/tizen-emulator-2.1.native" -L"/home/vlad/tizen-sdk/platforms/tizen2.1/rootstraps/tizen-emulator-2.1.native/usr/lib" -L"/home/vlad/tizen-sdk/platforms/tizen2.1/rootstraps/tizen-emulator-2.1.native/usr/lib/osp" -losp-uifw -losp-appfw -losp-image -losp-json -losp-ime -losp-net -lpthread -losp-content -losp-locations -losp-telephony -losp-uix -losp-media -losp-messaging -losp-web -losp-social -losp-wifi -losp-bluetooth -losp-nfc -losp-face -losp-speech-tts -losp-speech-stt -losp-shell -losp-shell-core -losp-vision -lxml2 -Xlinker -rpath="/opt/usr/apps/ctLjIIgCCj/lib"
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(MO_FILES)$(OBJS)$(C++_DEPS)$(C_DEPS)$(CC_DEPS)$(EDJ_FILES)$(CPP_DEPS)$(EXECUTABLES)$(CXX_DEPS)$(C_UPPER_DEPS) meecastservice.exe
	-@echo ' '

secondary-outputs: $(MO_FILES) $(EDJ_FILES)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
