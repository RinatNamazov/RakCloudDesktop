# *****************************************************************************
# *
# *  PROJECT:           RakCloudDesktop
# *  LICENSE:           See LICENSE in the top level directory
# *  FILE:              CMakeLists.txt
# *  DESCRIPTION:       CMake
# *  COPYRIGHT:         (c) 2021 RINWARES <rinwares.com>
# *  AUTHOR:            Rinat Namazov <rinat.namazov@rinwares.com>
# *
# *****************************************************************************

cmake_minimum_required(VERSION 3.15)

project(RakCloudDesktop)

option(UPDATE_TRANSLATION_SOURCES "Updates *.ts files" ON)

set(CMAKE_AUTOUIC OFF)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Widgets WebSockets LinguistTools REQUIRED)
find_package(protobuf CONFIG REQUIRED)
find_package(OpenSSL REQUIRED)
find_path(JWT_CPP_INCLUDE_DIRS "jwt-cpp/base.h")

file(GLOB RAKCLOUDDESKTOP_SRCS
    ${CMAKE_SOURCE_DIR}/src/RinwaresRootCa.cpp
    ${CMAKE_SOURCE_DIR}/src/RakCloudManagerApi.cpp
    ${CMAKE_SOURCE_DIR}/src/CommandLine.cpp
    ${CMAKE_SOURCE_DIR}/src/GeneralWindow.cpp
    ${CMAKE_SOURCE_DIR}/src/AuthWindow.cpp
    ${CMAKE_SOURCE_DIR}/src/MainWindow.cpp
    ${CMAKE_SOURCE_DIR}/src/main.cpp
    ${CMAKE_SOURCE_DIR}/src/utils.cpp
)

file(GLOB RAKCLOUDDESKTOP_HDRS
    ${CMAKE_SOURCE_DIR}/src/RinwaresRootCa.hpp
    ${CMAKE_SOURCE_DIR}/src/RakCloudManagerApi.hpp
    ${CMAKE_SOURCE_DIR}/src/CommandLine.hpp
    ${CMAKE_SOURCE_DIR}/src/GeneralWindow.hpp
    ${CMAKE_SOURCE_DIR}/src/AuthWindow.hpp
    ${CMAKE_SOURCE_DIR}/src/MainWindow.hpp
    ${CMAKE_SOURCE_DIR}/src/utils.hpp
)

file(GLOB RAKCLOUDDESKTOP_UI
    ${CMAKE_SOURCE_DIR}/src/GeneralWindow.ui
    ${CMAKE_SOURCE_DIR}/src/AuthWindow.ui
    ${CMAKE_SOURCE_DIR}/src/MainWindow.ui
)

file(GLOB RAKCLOUDDESKTOP_TS
    ${CMAKE_SOURCE_DIR}/translations/en_US.ts
    ${CMAKE_SOURCE_DIR}/translations/ru_RU.ts
)

set(PROTO_SRCS
    ${CMAKE_SOURCE_DIR}/proto/rcmisc.proto
    ${CMAKE_SOURCE_DIR}/proto/rcrequest.proto
    ${CMAKE_SOURCE_DIR}/proto/rcresponse.proto
)

if (UPDATE_TRANSLATION_SOURCES)
    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR}/src ${CMAKE_BINARY_DIR} ${RAKCLOUDDESKTOP_TS})
else()
    qt5_add_translation(QM_FILES ${TRANSLATION_FILES})
endif()

qt5_wrap_ui(UI_HEADERS ${RAKCLOUDDESKTOP_UI})

if (WIN32)
    set(APP_ICON_RESOURCE_WINDOWS ${CMAKE_SOURCE_DIR}/resources/resources.rc)
    set(WIN32DEF "WIN32") # To prevent the console from appearing.
endif()

add_executable(RakCloudDesktop ${WIN32DEF}
    ${RAKCLOUDDESKTOP_SRCS}
    ${RAKCLOUDDESKTOP_HDRS}
    ${RAKCLOUDDESKTOP_UI}
    ${RAKCLOUDDESKTOP_TS}
    ${UI_HEADERS}
    ${QM_FILES}
    ${CMAKE_SOURCE_DIR}/resources/resources.qrc
    ${APP_ICON_RESOURCE_WINDOWS}
)

protobuf_generate(LANGUAGE cpp TARGET RakCloudDesktop PROTOS ${PROTO_SRCS})

target_compile_features(RakCloudDesktop PRIVATE cxx_std_17)

target_include_directories(RakCloudDesktop PRIVATE
    ${CMAKE_BINARY_DIR}
    ${CMAKE_SOURCE_DIR}/src
    ${JWT_CPP_INCLUDE_DIRS}
)

target_link_libraries(RakCloudDesktop PRIVATE
    Qt5::Widgets
    Qt5::WebSockets
    protobuf::libprotobuf
    OpenSSL::SSL OpenSSL::Crypto
)

install(TARGETS RakCloudDesktop DESTINATION .)
install(FILES ${QM_FILES} DESTINATION languages)

# Use windeployqt/linuxdeployqt/macdeployqt.
