project(LoopTrainer VERSION 0.0.1)

set (TargetName ${PROJECT_NAME})

juce_add_gui_app(${TargetName} PRODUCT_NAME "Audio App Template")
juce_generate_juce_header(${TargetName})

target_sources(${TargetName} PRIVATE
        Source/Main.cpp
        Source/MainComponent.cpp
        Source/MainWindow.cpp
        Source/Toolbar.cpp
        Source/FileNavigator.cpp)

target_compile_definitions(${TargetName} PRIVATE
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_APPLICATION_NAME_STRING="$<TARGET_PROPERTY:${TargetName},JUCE_PROJECT_NAME>"
        JUCE_APPLICATION_VERSION_STRING="$<TARGET_PROPERTY:${TargetName},JUCE_VERSION>")

target_link_libraries(${TargetName} PRIVATE
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
        juce::juce_gui_basics
        shared_processing_code)

#Fetching Rubberband from git
CPMAddPackage(
        NAME rubberband
        GITHUB_REPOSITORY breakfastquay/rubberband
        GIT_TAG v2.0.0
        DOWNLOAD_ONLY YES
)

find_library(RUBBERBAND rubberband)

if (NOT RUBBERBAND)
    if (rubberband_ADDED)
        # Rubberband has no cmake support
        # need to be build with meson and ninja
        execute_process(
                COMMAND meson build
                WORKING_DIRECTORY ${rubberband_SOURCE_DIR}
                RESULT_VARIABLE MESON_RESULT)

        execute_process(
                COMMAND ninja -C build
                WORKING_DIRECTORY ${rubberband_SOURCE_DIR}
                RESULT_VARIABLE NINJA_RESULT
        )

        if(NOT MESON_RESULT EQUAL "0" AND NOT NINJA_RESULT EQUAL "0")
            message(FATAL_ERROR "rubberband build has failed with  ${RUBBERBAND_RESULT}")
        endif()

        find_library(RUBBERBAND
                     NAMES rubberband
                     PATHS "${rubberband_SOURCE_DIR}/build")

        if (NOT RUBBERBAND)
            message(FATAL_ERROR "rubberband was not found")
        endif()
    endif()
endif()

target_link_libraries(${TargetName} PRIVATE ${RUBBERBAND})
target_include_directories(${TargetName} PRIVATE ${rubberband_SOURCE_DIR})