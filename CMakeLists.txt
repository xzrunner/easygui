cmake_minimum_required(VERSION 3.8)

project(easygui)

add_definitions(
    -DNO_BOOST
)

option(EASYGUI_BUILD_TESTS "Set to ON to build the test suite." OFF)

################################################################################
# Source groups
################################################################################
set(dataset
    "include/easygui/Context.h"
    "source/Context.cpp"
)
source_group("dataset" FILES ${dataset})

set(dataset__config
    "include/easygui/RenderStyle.h"
    "include/easygui/UpdateConfig.h"
    "source/RenderStyle.cpp"
)
source_group("dataset\\config" FILES ${dataset__config})

set(dataset__render
    "include/easygui/RenderBuffer.h"
    "source/RenderBuffer.cpp"
)
source_group("dataset\\render" FILES ${dataset__render})

set(dataset__state
    "include/easygui/GuiState.h"
    "include/easygui/InputEvent.h"
    "include/easygui/IOState.h"
    "source/IOState.cpp"
)
source_group("dataset\\state" FILES ${dataset__state})

set(ecs
    "include/easygui/CompStorage.h"
    "include/easygui/CompType.h"
    "include/easygui/Entity.h"
    "include/easygui/EntityFactory.h"
    "include/easygui/System.h"
    "source/EntityFactory.cpp"
    "source/System.cpp"
)
source_group("ecs" FILES ${ecs})

set(utility
    "include/easygui/Callback.h"
    "include/easygui/ImGui.h"
    "include/easygui/typedef.h"
    "include/easygui/Utility.h"
    "source/Callback.cpp"
    "source/ImGui.cpp"
    "source/Utility.cpp"
)
source_group("utility" FILES ${utility})

set(widgets
    "include/easygui/Window.h"
    "source/Window.cpp"
)
source_group("widgets" FILES ${widgets})

set(widgets__button
    "include/easygui/ArrowButton.h"
    "include/easygui/Button.h"
    "include/easygui/Checkbox.h"
    "include/easygui/RadioButton.h"
    "source/ArrowButton.cpp"
    "source/Button.cpp"
    "source/Checkbox.cpp"
    "source/RadioButton.cpp"
)
source_group("widgets\\button" FILES ${widgets__button})

set(widgets__frame
    "include/easygui/Frame.h"
    "source/Frame.cpp"
)
source_group("widgets\\frame" FILES ${widgets__frame})

set(widgets__list
    "include/easygui/Combo.h"
    "include/easygui/Slider.h"
    "source/Combo.cpp"
    "source/Slider.cpp"
)
source_group("widgets\\list" FILES ${widgets__list})

set(widgets__text
    "include/easygui/Label.h"
    "include/easygui/Selectable.h"
    "source/Label.cpp"
    "source/Selectable.cpp"
)
source_group("widgets\\text" FILES ${widgets__text})

set(ALL_FILES
    ${dataset}
    ${dataset__config}
    ${dataset__render}
    ${dataset__state}
    ${ecs}
    ${utility}
    ${widgets}
    ${widgets__button}
    ${widgets__frame}
    ${widgets__list}
    ${widgets__text}
)

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/third_party/sm)
include_directories(${CMAKE_SOURCE_DIR}/third_party/unirender/include)
include_directories(${CMAKE_SOURCE_DIR}/third_party/tessellation/include)
include_directories(${CMAKE_SOURCE_DIR}/third_party/primitive/include)

add_library(easygui STATIC ${ALL_FILES})

if(EASYGUI_BUILD_TESTS)
    set(TEST_FILES
        "test/main.cpp"
    )

    include_directories(${CMAKE_SOURCE_DIR}/third_party/unirender/include)

    add_executable(easygui-test ${TEST_FILES})
    target_include_directories(easygui-test PRIVATE "third_party/unirender/include")
endif()