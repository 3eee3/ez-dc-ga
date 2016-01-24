################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Accelerometer.cpp \
../src/Collision.cpp \
../src/Mass.cpp \
../src/MipMap.cpp \
../src/Scene.cpp \
../src/Simulation.cpp \
../src/Spring.cpp \
../src/main.cpp \
../src/model_mapping.cpp 

C_SRCS += \
../src/SGIimage.c \
../src/dice.c \
../src/skirt_sphere.c 

OBJS += \
./src/Accelerometer.o \
./src/Collision.o \
./src/Mass.o \
./src/MipMap.o \
./src/SGIimage.o \
./src/Scene.o \
./src/Simulation.o \
./src/Spring.o \
./src/dice.o \
./src/main.o \
./src/model_mapping.o \
./src/skirt_sphere.o 

C_DEPS += \
./src/SGIimage.d \
./src/dice.d \
./src/skirt_sphere.d 

CPP_DEPS += \
./src/Accelerometer.d \
./src/Collision.d \
./src/Mass.d \
./src/MipMap.d \
./src/Scene.d \
./src/Simulation.d \
./src/Spring.d \
./src/main.d \
./src/model_mapping.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++0x -O0 -g3 -pedantic -Wall -Wextra -Wconversion -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


