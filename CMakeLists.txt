#工程：边缘特征点提取和匹配
#单位：哈尔滨工程大学
#时间：2017年6月25日
#作者：蒋强卫
#文件功能：编译工程，生成动态链接库

# cmake -D CUDA_USE_STATIC_CUDA_RUNTIME=OFF .. //camke command
project(edgedetection)
cmake_minimum_required(VERSION 2.8)

if(CMAKE_COMPILER_IS_GNUCXX)
	set(CMAKE_CXX_FLAGS "-Wno-variadic-macros -Wuninitialized -Wcast-align -Wredundant-decls -Wno-unknown-pragmas -fdata-sections -ffunction-sections -pedantic -Wextra -Wold-style-cast -Wstrict-overflow -Woverloaded-virtual -Wsign-promo -Wabi -Wmissing-declarations -Wall -std=c++1y -O3")
endif()




find_package( OpenCV REQUIRED )
file(GLOB SOURCES "src/*.cpp")
include_directories(include)

#生成矫正图像的动态链接库
#SET(SRC_SDK correct_lib.cpp utils.cpp)#生成动态库需要至少包含一个源文件
#add_library(correct_lib SHARED ${SRC_SDK})
#target_link_libraries(correct_lib ${OpenCV_LIBS} "-lpopt" -fPIC -shared )


#生成特征点匹配和定位动态链接库
#add_executable(edgedetection ${SOURCES})
#target_link_libraries(edgedetection ${OpenCV_LIBS})

add_library(edgedetection_position_lib SHARED ${SOURCES})
target_link_libraries(edgedetection_position_lib ${OpenCV_LIBS} "-lpopt" -fPIC -shared )
