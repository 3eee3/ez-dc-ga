################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Object3dModel.cpp \
../src/ObjectFileReader.cpp \
../src/blender2oGL_MassSpring.cpp 

OBJS += \
./src/Object3dModel.o \
./src/ObjectFileReader.o \
./src/blender2oGL_MassSpring.o 

CPP_DEPS += \
./src/Object3dModel.d \
./src/ObjectFileReader.d \
./src/blender2oGL_MassSpring.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++0x -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


