cmake_minimum_required(VERSION 3.6)
project(vulkan)

# GLFW - mudar diretorio para encontrar
#set(GLFW_INCLUDE_DIR "C:/dev/glfw-3.2.1.bin.WIN64/include")
#set(GLFW_LIB "C:/dev/glfw-3.2.1.bin.WIN64/lib-mingw-w64/glfw3.dll")

# Vulkan
set(VULKAN_INCLUDE_DIR "$ENV{VK_SDK_PATH}/Include")
# find_library(Vulkan_LIB NAMES vulkan-1 vulkan PATHS "$ENV{VK_SDK_PATH}/Bin32")
set(Vulkan_LIB "C:/VulkanSDK/1.0.39.1/Bin32/vulkan-1.lib")  # 32-bit library
if (Vulkan_LIB)
    message(STATUS "Vulkan lib found: ${Vulkan_LIB}")
endif()

set(SOURCE_FILES window.cpp)
#add_executable(window window.cpp instance.cpp)
add_executable(instance instance.cpp)
add_executable(devices devices.cpp)

include_directories(${VULKAN_INCLUDE_DIR})
target_link_libraries(instance ${Vulkan_LIB})
target_link_libraries(devices ${Vulkan_LIB})

