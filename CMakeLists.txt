cmake_minimum_required(VERSION 3.16)

project(Shrinkly VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Qt settings
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find Qt6 (or Qt5 if Qt6 not available)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Widgets)

# Source files
set(SOURCES
    app/main.cpp
    app/MainWindow.cpp
    app/MainWindow.h
    core/PngQuantRunner.cpp
    core/PngQuantRunner.h
    core/ImageTask.h
)

# Create executable
if(WIN32)
    add_executable(Shrinkly WIN32 ${SOURCES})
elseif(APPLE)
    add_executable(Shrinkly MACOSX_BUNDLE ${SOURCES})
else()
    add_executable(Shrinkly ${SOURCES})
endif()

# Link Qt libraries
target_link_libraries(Shrinkly
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Widgets
)

# Include directories
target_include_directories(Shrinkly PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/app
    ${CMAKE_CURRENT_SOURCE_DIR}/core
)

# Install rules
install(TARGETS Shrinkly
    BUNDLE DESTINATION .
    RUNTIME DESTINATION bin
)

# Copy resources to build directory
if(WIN32)
    # Windows: copy resources to exe directory
    add_custom_command(TARGET Shrinkly POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/resources
        $<TARGET_FILE_DIR:Shrinkly>/resources
    )
elseif(APPLE)
    # macOS: copy resources to app bundle
    set_target_properties(Shrinkly PROPERTIES
        MACOSX_BUNDLE_GUI_IDENTIFIER "com.shrinkly.app"
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        MACOSX_BUNDLE TRUE
    )

    add_custom_command(TARGET Shrinkly POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/resources/pngquant
        $<TARGET_BUNDLE_DIR:Shrinkly>/Contents/Resources/pngquant
    )
else()
    # Linux: copy resources to bin directory
    add_custom_command(TARGET Shrinkly POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/resources
        $<TARGET_FILE_DIR:Shrinkly>/resources
    )
endif()

# Set output directory
set_target_properties(Shrinkly PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
