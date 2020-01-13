# I target a recent cmake, it shouldn't be a problem on a dev machine
cmake_minimum_required(VERSION 3.15)

# Check if module is specified
if(NOT DEFINED APP_NAME)
    message(FATAL_ERROR "You must set APP_NAME variable!")
endif()

# Variables regarding programmer, port and baud rate
set(PROG_TYPE arduino)
set(USBPORT /dev/ttyUSB0)
set(PROG_ARGS -b 57600 -P ${USBPORT})

# Variables regarding the AVR chip
set(MCU   atmega328p)
set(F_CPU 16000000)
set(BAUD  9600)
add_definitions(-DF_CPU=${F_CPU})

# Programs names
set(AVRCPP   avr-g++)
set(AVRC     avr-gcc)
set(AVRSTRIP avr-strip)
set(OBJCOPY  avr-objcopy)
set(OBJDUMP  avr-objdump)
set(AVRSIZE  avr-size)
set(AVRDUDE  avrdude)

# Sets the compiler needs to come before the project function
set(CMAKE_SYSTEM_NAME  Generic)
set(CMAKE_CXX_COMPILER ${AVRCPP})
set(CMAKE_C_COMPILER   ${AVRC})
set(CMAKE_ASM_COMPILER   ${AVRC})

# Project name 
project(MyFirmware  VERSION 1.0
                    LANGUAGES C)

# Compiler flags
set(CSTANDARD "-std=gnu99")
set(CDEBUG    "-gstabs -g -ggdb")
set(CWARN     "-Wall -Wstrict-prototypes -Wl,--gc-sections -Wl,--relax")
set(CTUNING   "-funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums -ffunction-sections -fdata-sections")
set(COPT      "-Os -lm -lprintf_flt")
set(CMCU      "-mmcu=${MCU}")
set(CDEFS     "-DF_CPU=${F_CPU} -DBAUD=${BAUD}")

set(CFLAGS   "${CMCU} ${CDEBUG} ${CDEFS} ${COPT} ${CWARN} ${CSTANDARD} ${CTUNING}")
set(CXXFLAGS "${CMCU} ${CDEBUG} ${CDEFS} ${COPT} ${CTUNING}")

set(CMAKE_C_FLAGS   "${CFLAGS}")
set(CMAKE_CXX_FLAGS "${CXXFLAGS}")
set(CMAKE_ASM_FLAGS   "${CFLAGS}")

# Important project paths
set(APP_DIR_NAME    "app")
set(SRC             "src")
set(INC             "")
set(COMMON          "common")

set(APP_SRC_PATH    "${CMAKE_SOURCE_DIR}/${APP_DIR_NAME}/${APP_NAME}/${SRC}")
set(APP_INC_PATH    "${CMAKE_SOURCE_DIR}/${APP_DIR_NAME}/${APP_NAME}/${INC}")
set(COMMON_SRC_PATH "${CMAKE_SOURCE_DIR}/${COMMON}/${SRC}")
set(COMMON_INC_PATH "${CMAKE_SOURCE_DIR}/${COMMON}/${INC}")

# ----------------------- STUFF TO TAKE CARE OF -----------------------
# Set names for libraries
set(USART_IRQ_LIB "usart_irq")
#set(ADC_TIMER_LIB "adc_timer")
set(BLINK "blink")
set(WAVE_GENERATOR "wave_generator")
set(GPIO "gpio")

# Set names for modules
set(OSC "OSC")
set(SEQ "SEQ")

# Create list of libraries required for particular module
set(LIBS_LIST "")
if("${APP_NAME}" STREQUAL "${OSC}")
    list(APPEND LIBS_LIST   ${USART_IRQ_LIB} 
        #${ADC_TIMER_LIB}
                            ${WAVE_GENERATOR}
                            ${GPIO})

elseif("${APP_NAME}" STREQUAL "${SEQ}")
    list(APPEND LIBS_LIST   ${USART_IRQ_LIB} 
        #${ADC_TIMER_LIB}
                            ${WAVE_GENERATOR}
                            ${GPIO})

else()
    message(FATAL_ERROR "APP_NAME value is not valid!")

endif()

# Libraries (everything from /common)
add_library(${USART_IRQ_LIB} STATIC ${COMMON_SRC_PATH}/USART_irq.c)
#add_library(${ADC_TIMER_LIB} STATIC ${COMMON_SRC_PATH}/ADC_timer.c)
add_library(${BLINK} STATIC ${COMMON_SRC_PATH}/BLINK.c)
add_library(${WAVE_GENERATOR} STATIC ${COMMON_SRC_PATH}/waveforms_generator.c)
add_library(${GPIO} STATIC ${COMMON_SRC_PATH}/GPIO.c)

target_include_directories(${USART_IRQ_LIB} PUBLIC ${COMMON_INC_PATH})
#target_include_directories(${ADC_TIMER_LIB} PUBLIC ${COMMON_INC_PATH})
target_include_directories(${BLINK} PUBLIC ${COMMON_INC_PATH})
target_include_directories(${WAVE_GENERATOR} PUBLIC ${COMMON_INC_PATH})
target_include_directories(${GPIO} PUBLIC ${COMMON_INC_PATH})
# ----------------------- END OF STUFF TO TAKE CARE OF -----------------------

# Pick files to be compiled as main executable
file(GLOB SRC_FILES "${APP_SRC_PATH}/*.c")

# Build module
add_executable(${APP_NAME} ${SRC_FILES})
target_include_directories(${APP_NAME} PUBLIC ${APP_INC_PATH})
set_target_properties(${APP_NAME} PROPERTIES OUTPUT_NAME "${APP_NAME}.elf")

# Add libraries for particular modules (apps)
target_link_libraries(${APP_NAME} PUBLIC ${LIBS_LIST})

# Compiling targets
add_custom_target(strip ALL     ${AVRSTRIP} "${APP_NAME}.elf" DEPENDS ${APP_NAME})
add_custom_target(hex   ALL     ${OBJCOPY} -R .eeprom -O ihex "${APP_NAME}.elf" "${APP_NAME}.hex" DEPENDS strip)
add_custom_target(eeprom        ${OBJCOPY} -j .eeprom --change-section-lma .eeprom=0 -O ihex "${APP_NAME}.elf" "${APP_NAME}.eeprom" DEPENDS strip)
add_custom_target(disassemble   ${OBJDUMP} -S "${APP_NAME}.elf" > "${APP_NAME}.lst" DEPENDS strip)

# Flashing targets
add_custom_target(flash         ${AVRDUDE} -c ${PROG_TYPE} -p ${MCU} ${PROG_ARGS} -U flash:w:${APP_NAME}.hex DEPENDS hex)

# Fuses (For ATMega328P-PU, Calculated using http://eleccelerator.com/fusecalc/fusecalc.php?chip=atmega328p)
add_custom_target(reset         ${AVRDUDE} -c ${PROG_TYPE} -p ${MCU} ${PROG_ARGS} -e)
add_custom_target(fuses_16mhz   ${AVRDUDE} -c ${PROG_TYPE} -p ${MCU} ${PROG_ARGS} -U lfuse:w:0xFF:m)

# Utilities targets
add_custom_target(avr_terminal  ${AVRDUDE} -c ${PROG_TYPE} -p ${MCU} ${PROG_ARGS} -nt)
add_custom_target(size          ${AVRSIZE} -C "--mcu=${MCU}" "${APP_NAME}.elf" DEPENDS ${APP_NAME})

# Config logging
message("* -----")
message("* Project Name:\t${APP_NAME}")
message("* Sources:\t${SRC_FILES}")
message("* app inc pat:\t${APP_INC_PATH}")
message("* Libraries:\t${LIBS_LIST}")
message("* C Flags:\t${CMAKE_C_FLAGS}")
message("* -----\n")
