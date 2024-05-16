set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} "~/Dev/tools/QT/6.7.0/macos")
find_package(Qt6 6.4 REQUIRED COMPONENTS Quick Charts)

qt_standard_project_setup()

qt_add_executable(filter-designer
    main.cpp
)

qt_add_qml_module(filter-designer
    URI QT
    VERSION 1.0
    NO_RESOURCE_TARGET_PATH
    QML_FILES
        Main.qml
        SOURCES backend.hpp backend.cpp
)

set_target_properties(filter-designer PROPERTIES
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(filter-designer PRIVATE filter-designer-shared Qt6::Quick Qt6::Charts)