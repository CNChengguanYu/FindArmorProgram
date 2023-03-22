cmake_minimum_required(VERSION 3.10)
project(FindArmorProgram)

set(CMAKE_CXX_STANDARD 14)
find_package(OpenCV 4 REQUIRED)
add_executable(FindArmorProgram main.cpp LightBar/LightBar.cpp LightBar/LightBar.h settings.h FindModule/FindModule.cpp FindModule/FindModule.h Armor/Armor.cpp Armor/Armor.h)
TARGET_LINK_LIBRARIES(FindArmorProgram ${OpenCV_LIBS})