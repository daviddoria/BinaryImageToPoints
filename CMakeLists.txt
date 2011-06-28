cmake_minimum_required(VERSION 2.6)

PROJECT(BinaryImageToPoints)

#### Libraries ####
#VTK
FIND_PACKAGE(VTK REQUIRED)
INCLUDE(${VTK_USE_FILE})

#ITK
FIND_PACKAGE(ITK REQUIRED)
INCLUDE(${ITK_USE_FILE})

#### Executables ####
ADD_EXECUTABLE(BinaryImageToPoints BinaryImageToPoints.cxx)
TARGET_LINK_LIBRARIES(BinaryImageToPoints vtkHybrid ${ITK_LIBRARIES})
