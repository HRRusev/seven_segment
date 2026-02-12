################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Core/myLibrary/SevenSegment.cpp 

OBJS += \
./Core/myLibrary/SevenSegment.o 

CPP_DEPS += \
./Core/myLibrary/SevenSegment.d 


# Each subdirectory must supply rules for building sources it contributes
Core/myLibrary/%.o Core/myLibrary/%.su Core/myLibrary/%.cyclo: ../Core/myLibrary/%.cpp Core/myLibrary/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F401xE -c -I../Core/Inc -I"/home/hristian/WorkDirectory/STM32/Libraries/seven_segment/example/SevenSegmentExample/Core/myLibrary/Inc" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-myLibrary

clean-Core-2f-myLibrary:
	-$(RM) ./Core/myLibrary/SevenSegment.cyclo ./Core/myLibrary/SevenSegment.d ./Core/myLibrary/SevenSegment.o ./Core/myLibrary/SevenSegment.su

.PHONY: clean-Core-2f-myLibrary

