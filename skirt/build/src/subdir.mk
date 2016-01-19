################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Accelerometer.cpp \
../src/Mass.cpp \
../src/MipMap.cpp \
../src/Scene.cpp \
../src/Simulation.cpp \
../src/Spring.cpp \
../src/main.cpp 

C_SRCS += \
../src/SGIimage.c 

OBJS += \
./src/Accelerometer.o \
./src/Mass.o \
./src/MipMap.o \
./src/SGIimage.o \
./src/Scene.o \
./src/Simulation.o \
./src/Spring.o \
./src/main.o 

C_DEPS += \
./src/SGIimage.d 

CPP_DEPS += \
./src/Accelerometer.d \
./src/Mass.d \
./src/MipMap.d \
./src/Scene.d \
./src/Simulation.d \
./src/Spring.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++0x -O3 -pedantic -Wall -Wextra -Wconversion -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


