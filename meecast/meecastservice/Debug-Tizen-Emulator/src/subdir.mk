################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/App.cpp \
../src/Entry.cpp 

OBJS += \
./src/App.o \
./src/Entry.o 

CPP_DEPS += \
./src/App.d \
./src/Entry.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: C++ Compiler'
	clang++ -I"pch" -D_DEBUG -I"/home/vlad/workspace/meecast/meecast/meecastservice/inc" -O0 -g3 -Wall -c -fmessage-length=0 -target i386-tizen-linux-gnueabi -gcc-toolchain /home/vlad/tizen-sdk/tools/smart-build-interface/../i386-linux-gnueabi-gcc-4.5/ -ccc-gcc-name i386-linux-gnueabi-g++ -march=i386 -Wno-gnu -fPIE --sysroot="/home/vlad/tizen-sdk/platforms/tizen2.1/rootstraps/tizen-emulator-2.1.native" -I"/home/vlad/tizen-sdk/platforms/tizen2.1/rootstraps/tizen-emulator-2.1.native/usr/include/libxml2" -I"/home/vlad/tizen-sdk/library" -I"/home/vlad/tizen-sdk/platforms/tizen2.1/rootstraps/tizen-emulator-2.1.native/usr/include" -I"/home/vlad/tizen-sdk/platforms/tizen2.1/rootstraps/tizen-emulator-2.1.native/usr/include/osp" -D_APP_LOG -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


