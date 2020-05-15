cmake_minimum_required (VERSION 2.8.12)

project (MountIso)

set(PROJECT_VERSION "0.1.0")
set(PROJECT_VERSION_MAJOR 0)

set (QT_MIN_VERSION 5.12.0)

# Extra CMake stuff
include (FeatureSummary)

# KDE Frameworks
set (KF5_DEP_VERSION "5.17.0") # handled by release scripts

find_package (ECM ${KF5_DEP_VERSION}  NO_MODULE)

set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR})

include (KDEInstallDirs)
include (KDECMakeSettings)
include (KDECompilerSettings)
include (GenerateExportHeader)
include (ECMGenerateHeaders)
include(KDEClangFormat)
# Qt
set (CMAKE_AUTOMOC ON)

find_package (
   Qt5 ${QT_MIN_VERSION} CONFIG REQUIRED
   COMPONENTS
      Core
   )

find_package (
   KF5 ${KF5_DEP_VERSION} REQUIRED COMPONENTS
   CoreAddons
   I18n
   KIO
   )

include (ECMPackageConfigHelpers)
include (ECMSetupVersion)

add_subdirectory (mountisoplugin)

# add clang-format target for all our real source files
file(GLOB_RECURSE ALL_CLANG_FORMAT_SOURCE_FILES *.cpp *.h)
kde_clang_format(${ALL_CLANG_FORMAT_SOURCE_FILES})
