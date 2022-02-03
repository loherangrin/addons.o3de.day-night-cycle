# Copyright (c) Contributors to the Open 3D Engine Project.
# For complete copyright and license terms please see the LICENSE at the root of this distribution.
#
# SPDX-License-Identifier: Apache-2.0
#
#

# Currently we are in the Code folder: ${CMAKE_CURRENT_LIST_DIR}
# Get the platform specific folder ${pal_dir} for the current folder: ${CMAKE_CURRENT_LIST_DIR}/Platform/${PAL_PLATFORM_NAME}
# Note: ly_get_list_relative_pal_filename will take care of the details for us, as this may be a restricted platform
#       in which case it will see if that platform is present here or in the restricted folder.
#       i.e. It could here in our gem : Gems/Loherangrin_DayNightCycle/Code/Platform/<platorm_name>  or
#            <restricted_folder>/<platform_name>/Gems/Loherangrin_DayNightCycle/Code
ly_get_list_relative_pal_filename(pal_dir ${CMAKE_CURRENT_LIST_DIR}/Platform/${PAL_PLATFORM_NAME} ${o3de_gem_restricted_path} ${o3de_gem_path} ${o3de_gem_name})

# Now that we have the platform abstraction layer (PAL) folder for this folder, thats where we will find the
# traits for this platform. Traits for a platform are defines for things like whether or not something in this gem
# is supported by this platform.
include(${pal_dir}/PAL_${PAL_PLATFORM_NAME_LOWERCASE}.cmake)

# Add the Loherangrin_DayNightCycle.Static target
# Note: We include the common files and the platform specific files which are set in daynightcycle_common_files.cmake
# and in ${pal_dir}/daynightcycle_${PAL_PLATFORM_NAME_LOWERCASE}_files.cmake
ly_add_target(
    NAME Loherangrin_DayNightCycle.Static STATIC
    NAMESPACE Gem
    FILES_CMAKE
        daynightcycle_files.cmake
        ${pal_dir}/daynightcycle_${PAL_PLATFORM_NAME_LOWERCASE}_files.cmake
    INCLUDE_DIRECTORIES
        PUBLIC
            Include
        PRIVATE
            Source
    BUILD_DEPENDENCIES
        PUBLIC
            AZ::AzCore
            AZ::AzFramework
            Gem::Loherangrin_DateTime.Static
)

# Here add Loherangrin_DayNightCycle target, it depends on the Loherangrin_DayNightCycle.Static
ly_add_target(
    NAME Loherangrin_DayNightCycle ${PAL_TRAIT_MONOLITHIC_DRIVEN_MODULE_TYPE}
    NAMESPACE Gem
    FILES_CMAKE
        daynightcycle_shared_files.cmake
        ${pal_dir}/daynightcycle_shared_${PAL_PLATFORM_NAME_LOWERCASE}_files.cmake
    INCLUDE_DIRECTORIES
        PUBLIC
            Include
        PRIVATE
            Source
    BUILD_DEPENDENCIES
        PRIVATE
            Gem::Loherangrin_DayNightCycle.Static
            Gem::Loherangrin_DateTime.Static
)

# By default, we will specify that the above target Loherangrin_DayNightCycle would be used by
# Client and Server type targets when this gem is enabled.  If you don't want it
# active in Clients or Servers by default, delete one of both of the following lines:
ly_create_alias(NAME Loherangrin_DayNightCycle.Clients NAMESPACE Gem TARGETS Gem::Loherangrin_DayNightCycle)
ly_create_alias(NAME Loherangrin_DayNightCycle.Servers NAMESPACE Gem TARGETS Gem::Loherangrin_DayNightCycle)

# If we are on a host platform, we want to add the host tools targets like the Loherangrin_DayNightCycle.Editor target which
# will also depend on Loherangrin_DayNightCycle.Static
if(PAL_TRAIT_BUILD_HOST_TOOLS)
    ly_add_target(
        NAME Loherangrin_DayNightCycle.Editor.Static STATIC
        NAMESPACE Gem
        FILES_CMAKE
            daynightcycle_editor_files.cmake
        INCLUDE_DIRECTORIES
            PRIVATE
                Source
            PUBLIC
                Include
        BUILD_DEPENDENCIES
            PUBLIC
                AZ::AzToolsFramework
                Gem::Loherangrin_DayNightCycle.Static
    )

    ly_add_target(
        NAME Loherangrin_DayNightCycle.Editor GEM_MODULE
        NAMESPACE Gem
        AUTOMOC
        FILES_CMAKE
            daynightcycle_editor_shared_files.cmake
        INCLUDE_DIRECTORIES
            PRIVATE
                Source
            PUBLIC
                Include
        BUILD_DEPENDENCIES
            PUBLIC
                Gem::Loherangrin_DayNightCycle.Editor.Static
    )

    # By default, we will specify that the above target Loherangrin_DayNightCycle would be used by
    # Tool and Builder type targets when this gem is enabled.  If you don't want it
    # active in Tools or Builders by default, delete one of both of the following lines:
    ly_create_alias(NAME Loherangrin_DayNightCycle.Tools    NAMESPACE Gem TARGETS Gem::Loherangrin_DayNightCycle.Editor)
    ly_create_alias(NAME Loherangrin_DayNightCycle.Builders NAMESPACE Gem TARGETS Gem::Loherangrin_DayNightCycle.Editor)


endif()

################################################################################
# Tests
################################################################################
# See if globally, tests are supported
if(PAL_TRAIT_BUILD_TESTS_SUPPORTED)
    # We globally support tests, see if we support tests on this platform for Loherangrin_DayNightCycle.Static
    if(PAL_TRAIT_LOHERANGRIN_DAYNIGHTCYCLE_TEST_SUPPORTED)
        # We support Loherangrin_DayNightCycle.Tests on this platform, add Loherangrin_DayNightCycle.Tests target which depends on Loherangrin_DayNightCycle.Static
        ly_add_target(
            NAME Loherangrin_DayNightCycle.Tests ${PAL_TRAIT_TEST_TARGET_TYPE}
            NAMESPACE Gem
            FILES_CMAKE
                daynightcycle_files.cmake
                daynightcycle_tests_files.cmake
            INCLUDE_DIRECTORIES
                PRIVATE
                    Tests
                    Source
            BUILD_DEPENDENCIES
                PRIVATE
                    AZ::AzTest
                    AZ::AzFramework
                    Gem::Loherangrin_DayNightCycle.Static
        )

        # Add Loherangrin_DayNightCycle.Tests to googletest
        ly_add_googletest(
            NAME Gem::Loherangrin_DayNightCycle.Tests
        )
    endif()

    # If we are a host platform we want to add tools test like editor tests here
    if(PAL_TRAIT_BUILD_HOST_TOOLS)
        # We are a host platform, see if Editor tests are supported on this platform
        if(PAL_TRAIT_LOHERANGRIN_DAYNIGHTCYCLE_EDITOR_TEST_SUPPORTED)
            # We support Loherangrin_DayNightCycle.Editor.Tests on this platform, add Loherangrin_DayNightCycle.Editor.Tests target which depends on Loherangrin_DayNightCycle.Editor
            ly_add_target(
                NAME Loherangrin_DayNightCycle.Editor.Tests ${PAL_TRAIT_TEST_TARGET_TYPE}
                NAMESPACE Gem
                FILES_CMAKE
                    daynightcycle_editor_tests_files.cmake
                INCLUDE_DIRECTORIES
                    PRIVATE
                        Tests
                        Source
                BUILD_DEPENDENCIES
                    PRIVATE
                        AZ::AzTest
                        Gem::Loherangrin_DayNightCycle.Editor
            )

            # Add Loherangrin_DayNightCycle.Editor.Tests to googletest
            ly_add_googletest(
                NAME Gem::Loherangrin_DayNightCycle.Editor.Tests
            )
        endif()
    endif()
endif()
