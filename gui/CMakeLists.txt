set(APP_NAME "FilterDesigner")

if(NOT QT_PATH)
  message(SEND_ERROR "Please specify Qt library path via -DQT_PATH")
endif()
message("-- Qt Path: " ${QT_PATH})

set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${QT_PATH})
find_package(Qt6 6.4 REQUIRED COMPONENTS Quick Charts)

qt_standard_project_setup()

set(APP_ICON_MACOS "${CMAKE_CURRENT_SOURCE_DIR}/icons/app_icon.icns")
set_source_files_properties(${APP_ICON_MACOS} PROPERTIES
       MACOSX_PACKAGE_LOCATION "Resources")

qt_add_executable(${APP_NAME} MACOSX_BUNDLE
  Main.cpp
  ${APP_ICON_MACOS}
)

qt_add_resources(${APP_NAME} "assets"
    PREFIX "/resources"
    FILES "icons/clipboard.png"
)

qt_add_qml_module(${APP_NAME}
    URI qt
    VERSION ${PROJECT_VERSION}
    NO_RESOURCE_TARGET_PATH
    QML_FILES qt/qml/Main.qml
    QML_FILES qt/qml/FilterCoefficients.qml
    QML_FILES qt/qml/FrequencyResponse.qml
    QML_FILES qt/qml/Controls.qml
    SOURCES Backend.hpp Backend.cpp
    SOURCES ListSelectorValues.hpp
    SOURCES ListSelectorValues.cpp
    SOURCES ValueRange.hpp
    SOURCES DefaultControlValues.hpp
)

set_target_properties(${APP_NAME} PROPERTIES
    MACOSX_BUNDLE_INFO_PLIST ${PROJECT_SOURCE_DIR}/macos/Info.plist.in
    MACOSX_BUNDLE_ICON_FILE app_icon.icns
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
)

target_link_libraries(${APP_NAME} PRIVATE FilterDesignerShared Qt6::Quick Qt6::Charts)

# Package all dependencies into a MacOS .app bundle
if(APPLE_DISTRIBUTION_IDENTITY)
message("-- Apple Distribution Identity: " ${APPLE_DISTRIBUTION_IDENTITY})

add_custom_target(remove-extended-attributes
    VERBATIM
    COMMAND xattr -cr $<TARGET_BUNDLE_DIR:${APP_NAME}>)

add_custom_target(copy-provision-profile
    VERBATIM
    DEPENDS remove-extended-attributes
    COMMAND cp 
    ${PROJECT_SOURCE_DIR}/macos/embedded.provisionprofile 
    $<TARGET_BUNDLE_DIR:${APP_NAME}>/Contents)

add_custom_target(qt-bundle
    VERBATIM
    DEPENDS copy-provision-profile
    COMMAND ${QT_PATH}/bin/macdeployqt6 
    $<TARGET_BUNDLE_DIR:${APP_NAME}> 
    -qmldir=${PROJECT_SOURCE_DIR}/gui/qt/qml
    -always-overwrite 
    -verbose=2
    -appstore-compliant
    -sign-for-notarization=${APPLE_DISTRIBUTION_IDENTITY})

add_custom_target(remove-dsym
    VERBATIM
    DEPENDS qt-bundle
    COMMAND find $<TARGET_BUNDLE_DIR:${APP_NAME}> -name "*.dSYM" | xargs -I $ rm -R $)

add_custom_target(codesign-with-entitlements
    VERBATIM
    DEPENDS remove-dsym
    COMMAND codesign 
    --options runtime 
    --sign ${APPLE_DISTRIBUTION_IDENTITY}
    --force 
    --timestamp 
    --verbose
    --entitlements ${PROJECT_SOURCE_DIR}/macos/entitlements.plist
    $<TARGET_BUNDLE_DIR:${APP_NAME}>)

add_custom_target(mac-bundle
    DEPENDS codesign-with-entitlements)

endif()

# Prepare .pkg installer
if(APPLE_INSTALLER_IDENTITY)
message("-- Apple Installer Identity: " ${APPLE_INSTALLER_IDENTITY})

add_custom_target(mac-pkg
    VERBATIM
    COMMAND productbuild --sign ${APPLE_INSTALLER_IDENTITY} 
    --component $<TARGET_BUNDLE_DIR:${APP_NAME}> /Applications 
    ${PROJECT_BINARY_DIR}/gui/${APP_NAME}.pkg)
endif()