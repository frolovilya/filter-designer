if(NOT QT_PATH)
    set(QT_PATH "~/Dev/tools/QT/6.7.0/macos")
endif()
message("Qt Path: " ${QT_PATH})

set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${QT_PATH})
find_package(Qt6 6.4 REQUIRED COMPONENTS Quick Charts)

qt_standard_project_setup()

qt_add_executable(filter-designer
  Main.cpp
)

qt_add_resources(filter-designer "assets"
    PREFIX "/icons"
    FILES "qt/clipboard.png"
)

qt_add_qml_module(filter-designer
    URI QT
    VERSION 1.0
    NO_RESOURCE_TARGET_PATH
    QML_FILES
        qt/Main.qml
        SOURCES Backend.hpp Backend.cpp
        QML_FILES qt/FilterCoefficients.qml
        QML_FILES qt/FrequencyResponse.qml
        QML_FILES qt/Controls.qml
        SOURCES ListSelectorValues.hpp
        SOURCES ListSelectorValues.cpp
        SOURCES ValueRange.hpp
        SOURCES DefaultControlValues.hpp
)

set_target_properties(filter-designer PROPERTIES
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(filter-designer PRIVATE filter-designer-shared Qt6::Quick Qt6::Charts)
