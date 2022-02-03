# Copyright (c) Contributors to the Open 3D Engine Project.
# For complete copyright and license terms please see the LICENSE at the root of this distribution.
#
# SPDX-License-Identifier: Apache-2.0
#
#

# Currently we are in the Loherangrin_DayNightCycle_CppExample/Code folder: ${CMAKE_CURRENT_LIST_DIR}
# Get the platform specific folder ${pal_dir} for the current folder: ${CMAKE_CURRENT_LIST_DIR}/Platform/${PAL_PLATFORM_NAME}
# Note: ly_get_list_relative_pal_filename will take care of the details for us, as this may be a restricted platform
#       in which case it will see if that platform is present here or in the restricted folder.
#       i.e. It could here : Loherangrin_DayNightCycle_CppExample/Code/Platform/<platorm_name>  or
#            <restricted_folder>/<platform_name>/Loherangrin_DayNightCycle_CppExample/Code
ly_get_list_relative_pal_filename(pal_dir ${CMAKE_CURRENT_LIST_DIR}/Platform/${PAL_PLATFORM_NAME} ${o3de_project_restricted_path} ${o3de_project_path} ${o3de_project_name})

# Now that we have the platform abstraction layer (PAL) folder for this folder, thats where we will find the
# traits for this platform. Traits for a platform are defines for things like whether or not something in this project
# is supported by this platform.
include(${pal_dir}/PAL_${PAL_PLATFORM_NAME_LOWERCASE}.cmake)

# Now that we have loaded our project traits for this platform, see if this project is even supported on this platform.
# If its not supported we just return after including the unsupported.
if(NOT PAL_TRAIT_LOHERANGRIN_DAYNIGHTCYCLE_CPPEXAMPLE_SUPPORTED)
    return()
endif()

# We are on a supported platform, so add the Loherangrin_DayNightCycle_CppExample target
# Note: We include the common files and the platform specific files which are set in daynightcycle_cppexample_files.cmake and
# in ${pal_dir}/daynightcycle_cppexample_${PAL_PLATFORM_NAME_LOWERCASE}_files.cmake
ly_add_target(
    NAME Loherangrin_DayNightCycle_CppExample.Static STATIC
    NAMESPACE Gem
    FILES_CMAKE
        daynightcycle_cppexample_files.cmake
        ${pal_dir}/daynightcycle_cppexample_${PAL_PLATFORM_NAME_LOWERCASE}_files.cmake
    INCLUDE_DIRECTORIES
        PUBLIC
            Include
    BUILD_DEPENDENCIES
        PRIVATE
            AZ::AzGameFramework
            Gem::Atom_AtomBridge.Static
            Gem::AtomLyIntegration_CommonFeatures.Static
            Gem::Loherangrin_DayNightCycle.Static
            Gem::Loherangrin_DateTime.Static
            Gem::LyShine.Static
)

ly_add_target(
    NAME Loherangrin_DayNightCycle_CppExample ${PAL_TRAIT_MONOLITHIC_DRIVEN_MODULE_TYPE}
    NAMESPACE Gem
    FILES_CMAKE
        daynightcycle_cppexample_shared_files.cmake
        ${pal_dir}/daynightcycle_cppexample_shared_${PAL_PLATFORM_NAME_LOWERCASE}_files.cmake
    INCLUDE_DIRECTORIES
        PUBLIC
            Include
    BUILD_DEPENDENCIES
        PRIVATE
            Gem::Loherangrin_DayNightCycle_CppExample.Static
            AZ::AzCore
            Gem::Loherangrin_DayNightCycle.Static
            Gem::Loherangrin_DateTime.Static
            Gem::LyShine.Static
)

# if enabled, Loherangrin_DayNightCycle_CppExample is used by all kinds of applications
ly_create_alias(NAME Loherangrin_DayNightCycle_CppExample.Builders NAMESPACE Gem TARGETS Gem::Loherangrin_DayNightCycle_CppExample)
ly_create_alias(NAME Loherangrin_DayNightCycle_CppExample.Tools    NAMESPACE Gem TARGETS Gem::Loherangrin_DayNightCycle_CppExample)
ly_create_alias(NAME Loherangrin_DayNightCycle_CppExample.Clients  NAMESPACE Gem TARGETS Gem::Loherangrin_DayNightCycle_CppExample)
ly_create_alias(NAME Loherangrin_DayNightCycle_CppExample.Servers  NAMESPACE Gem TARGETS Gem::Loherangrin_DayNightCycle_CppExample)

################################################################################
# Gem dependencies
################################################################################

# Enable the specified list of gems from GEM_FILE or GEMS list for this specific project:
ly_enable_gems(PROJECT_NAME Loherangrin_DayNightCycle_CppExample GEM_FILE enabled_gems.cmake)


if(PAL_TRAIT_BUILD_SERVER_SUPPORTED)
    # this property causes it to actually make a ServerLauncher.
    # if you don't want a Server application, you can remove this and the
    # following ly_enable_gems lines.
    set_property(GLOBAL APPEND PROPERTY LY_LAUNCHER_SERVER_PROJECTS Loherangrin_DayNightCycle_CppExample)
endif()
