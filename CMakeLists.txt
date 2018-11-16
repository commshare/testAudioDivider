cmake_minimum_required(VERSION 3.12)
project(AudioDividerOK)

set(CMAKE_CXX_STANDARD 11)

include_directories(AudioDivider/adcpp)
include_directories(AudioDivider/adcpp/Properties)
include_directories(AudioDivider/adcpp/WASAPI)
include_directories(AudioDivider/SoundHook/SoundHook)

add_executable(AudioDividerOK
        AudioDivider/adcpp/Properties/AssemblyInfo.cpp
        AudioDivider/adcpp/Properties/AssemblyInfo.h
        AudioDivider/adcpp/WASAPI/EnumsAndStructs.cpp
        AudioDivider/adcpp/WASAPI/EnumsAndStructs.h
        AudioDivider/adcpp/WASAPI/Interfaces.cpp
        AudioDivider/adcpp/WASAPI/Interfaces.h
        AudioDivider/adcpp/AudioDivider.FormAudioDivider.cpp
        AudioDivider/adcpp/AudioDivider.FormAudioDivider.h
        AudioDivider/adcpp/AudioDivider.FormAutoControl.cpp
        AudioDivider/adcpp/AudioDivider.FormAutoControl.h
        AudioDivider/adcpp/AutoInjectHandler.cpp
        AudioDivider/adcpp/AutoInjectHandler.h
        AudioDivider/adcpp/Communication.cpp
        AudioDivider/adcpp/Communication.h
        AudioDivider/adcpp/Configuration.cpp
        AudioDivider/adcpp/Configuration.h
        AudioDivider/adcpp/exceptionhelper.h
        AudioDivider/adcpp/IniConfiguration.cpp
        AudioDivider/adcpp/IniConfiguration.h
        AudioDivider/adcpp/Inject.cpp
        AudioDivider/adcpp/Inject.h
        AudioDivider/adcpp/Logging.cpp
        AudioDivider/adcpp/Logging.h
        AudioDivider/adcpp/Main.cpp
        AudioDivider/adcpp/Main.h
        AudioDivider/adcpp/Native.cpp
        AudioDivider/adcpp/Native.h
        AudioDivider/adcpp/ProgramAutoInfo.cpp
        AudioDivider/adcpp/ProgramAutoInfo.h
        AudioDivider/adcpp/ProgramInfo.cpp
        AudioDivider/adcpp/ProgramInfo.h
        AudioDivider/adcpp/Security.cpp
        AudioDivider/adcpp/Security.h
        AudioDivider/adcpp/SoundHandler.cpp
        AudioDivider/adcpp/SoundHandler.h
        AudioDivider/adcpp/stringbuilder.h
        AudioDivider/adcpp/StringExtensions.cpp
        AudioDivider/adcpp/StringExtensions.h
        AudioDivider/adcpp/tangible_filesystem.h
        AudioDivider/Helper/Helper/main.cpp
        AudioDivider/SoundHook/SoundHook/allIncludes.h
        AudioDivider/SoundHook/SoundHook/main.cpp
        AudioDivider/SoundHook/SoundHook/PipeClient.cpp)
