# The settings in this file are the WebKit project default values, and
# are recommended for most ports. Ports can override these settings in
# Options*.cmake, but should do so only if there is strong reason to
# deviate from the defaults of the WebKit project (e.g. if the feature
# requires platform-specific implementation that does not exist).

set(_WEBKIT_AVAILABLE_OPTIONS "")

set(PUBLIC YES)
set(PRIVATE NO)

macro(_ENSURE_OPTION_MODIFICATION_IS_ALLOWED)
    if (NOT _SETTING_WEBKIT_OPTIONS)
        message(FATAL_ERROR "Options must be set between WEBKIT_OPTION_BEGIN and WEBKIT_OPTION_END")
    endif ()
endmacro()

macro(_ENSURE_IS_WEBKIT_OPTION _name)
    list(FIND _WEBKIT_AVAILABLE_OPTIONS ${_name} ${_name}_OPTION_INDEX)
    if (${_name}_OPTION_INDEX EQUAL -1)
        message(FATAL_ERROR "${_name} is not a valid WebKit option")
    endif ()
endmacro()

macro(WEBKIT_OPTION_DEFINE _name _description _public _initial_value)
    _ENSURE_OPTION_MODIFICATION_IS_ALLOWED()

    set(_WEBKIT_AVAILABLE_OPTIONS_DESCRIPTION_${_name} ${_description})
    set(_WEBKIT_AVAILABLE_OPTIONS_IS_PUBLIC_${_name} ${_public})
    set(_WEBKIT_AVAILABLE_OPTIONS_INITIAL_VALUE_${_name} ${_initial_value})
    set(_WEBKIT_AVAILABLE_OPTIONS_${_name}_CONFLICTS "")
    set(_WEBKIT_AVAILABLE_OPTIONS_${_name}_DEPENDENCIES "")
    list(APPEND _WEBKIT_AVAILABLE_OPTIONS ${_name})

    EXPOSE_VARIABLE_TO_BUILD(${_name})
endmacro()

macro(WEBKIT_OPTION_DEFAULT_PORT_VALUE _name _public _value)
    _ENSURE_OPTION_MODIFICATION_IS_ALLOWED()
    _ENSURE_IS_WEBKIT_OPTION(${_name})

    set(_WEBKIT_AVAILABLE_OPTIONS_IS_PUBLIC_${_name} ${_public})
    set(_WEBKIT_AVAILABLE_OPTIONS_INITIAL_VALUE_${_name} ${_value})
endmacro()

macro(WEBKIT_OPTION_CONFLICT _name _conflict)
    _ENSURE_OPTION_MODIFICATION_IS_ALLOWED()
    _ENSURE_IS_WEBKIT_OPTION(${_name})
    _ENSURE_IS_WEBKIT_OPTION(${_conflict})

    list(APPEND _WEBKIT_AVAILABLE_OPTIONS_${_name}_CONFLICTS ${_conflict})
endmacro()

macro(WEBKIT_OPTION_DEPEND _name _depend)
    _ENSURE_OPTION_MODIFICATION_IS_ALLOWED()
    _ENSURE_IS_WEBKIT_OPTION(${_name})
    _ENSURE_IS_WEBKIT_OPTION(${_depend})

    list(APPEND _WEBKIT_AVAILABLE_OPTIONS_${_name}_DEPENDENCIES ${_depend})
endmacro()

macro(WEBKIT_OPTION_BEGIN)
    set(_SETTING_WEBKIT_OPTIONS TRUE)

    if (WTF_CPU_ARM64 OR WTF_CPU_X86_64)
        set(ENABLE_JIT_DEFAULT ON)
        set(ENABLE_FTL_DEFAULT ON)
        set(USE_SYSTEM_MALLOC_DEFAULT OFF)
        set(ENABLE_C_LOOP_DEFAULT OFF)
        set(ENABLE_SAMPLING_PROFILER_DEFAULT ON)
    elseif (WTF_CPU_ARM AND WTF_OS_LINUX AND ARM_THUMB2_DETECTED)
        set(ENABLE_JIT_DEFAULT ON)
        set(ENABLE_FTL_DEFAULT OFF)
        set(USE_SYSTEM_MALLOC_DEFAULT OFF)
        set(ENABLE_C_LOOP_DEFAULT OFF)
        set(ENABLE_SAMPLING_PROFILER_DEFAULT ON)
    elseif (WTF_CPU_MIPS AND WTF_OS_LINUX)
        set(ENABLE_JIT_DEFAULT ON)
        set(ENABLE_FTL_DEFAULT OFF)
        set(USE_SYSTEM_MALLOC_DEFAULT OFF)
        set(ENABLE_C_LOOP_DEFAULT OFF)
        set(ENABLE_SAMPLING_PROFILER_DEFAULT OFF)
    elseif (WTF_CPU_RISCV64)
        set(ENABLE_JIT_DEFAULT OFF)
        set(ENABLE_FTL_DEFAULT OFF)
        set(USE_SYSTEM_MALLOC_DEFAULT OFF)
        set(ENABLE_C_LOOP_DEFAULT OFF)
        set(ENABLE_SAMPLING_PROFILER_DEFAULT OFF)
    else ()
        set(ENABLE_JIT_DEFAULT OFF)
        set(ENABLE_FTL_DEFAULT OFF)
        set(USE_SYSTEM_MALLOC_DEFAULT ON)
        set(ENABLE_C_LOOP_DEFAULT ON)
        set(ENABLE_SAMPLING_PROFILER_DEFAULT OFF)
    endif ()

    if (DEFINED ClangTidy_EXE OR DEFINED IWYU_EXE)
        message(STATUS "Unified builds are disabled when analyzing sources")
        set(ENABLE_UNIFIED_BUILDS_DEFAULT OFF)
    else ()
        set(ENABLE_UNIFIED_BUILDS_DEFAULT ON)
    endif ()

    WEBKIT_OPTION_DEFINE(ENABLE_3D_TRANSFORMS "Toggle 3D transforms support" PRIVATE ON)
    WEBKIT_OPTION_DEFINE(ENABLE_CORE_IMAGE_ACCELERATED_FILTER_RENDER "Toggle Accelerated CSS/SVG Filter Rendering using CoreImage" PRIVATE OFF)
    WEBKIT_OPTION_DEFINE(ENABLE_ACCESSIBILITY "Whether to enable support for accessibility" PRIVATE ON)
    WEBKIT_OPTION_DEFINE(ENABLE_ACCESSIBILITY_ISOLATED_TREE "Toggle accessibility isolated tree support" PRIVATE OFF)
    WEBKIT_OPTION_DEFINE(ENABLE_API_TESTS "Enable public API unit tests" PRIVATE OFF)
    WEBKIT_OPTION_DEFINE(ENABLE_APPLICATION_MANIFEST "Toggle Application Manifest support" PRIVATE OFF)
    WEBKIT_OPTION_DEFINE(ENABLE_ASYNC_SCROLLING "Enable asynchronous scrolling" PRIVATE OFF)
    WEBKIT_OPTION_DEFINE(ENABLE_ATTACHMENT_ELEMENT "Toggle Attachment Element support" PRIVATE OFF)
    WEBKIT_OPTION_DEFINE(ENABLE_AUTOCAPITALIZE "Toggle autocapitalize support" PRIVATE OFF)
    WEBKIT_OPTION_DEFINE(ENABLE_AVF_CAPTIONS "Toggle AVFoundation caption support" PRIVATE OFF)
    WEBKIT_OPTION_DEFINE(ENABLE_BUBBLEWRAP_SANDBOX "Toggle Bubblewrap sandboxing support" PRIVATE OFF)
    WEBKIT_OPTION_DEFINE(ENABLE_CACHE_PARTITIONING "Toggle cache partitioning support" PRIVATE OFF)
    WEBKIT_OPTION_DEFINE(ENABLE_CHANNEL_MESSAGING "Toggle Channel Messaging support" PRIVATE ON)
    WEBKIT_OPTION_DEFINE(ENABLE_COG "Toggle Cog browser compilation." PRIVATE OFF)
    WEBKIT_OPTION_DEFINE(ENABLE_CONTENT_EXTENSIONS "Toggle Content Extensions support" PRIVATE OFF)
    WEBKIT_OPTION_DEFINE(ENABLE_CONTENT_FILTERING "Toggle content filtering support" PRIVATE OFF)
    WEBKIT_OPTION_DEFINE(ENABLE_CONTEXT_MENUS "Toggle Context Menu support" PRIVATE ON)
    WEBKIT_OPTION_DEFINE(ENABLE_CSS3_TEXT "Toggle CSS3 Text support" PRIVATE OFF)
    WEBKIT_OPTION_DEFINE(ENABLE_CSS_BOX_DECORATION_BREAK "Toggle CSS box-decoration-break support" PRIVATE ON)
    WEBKIT_OPTION_DEFINE(ENABLE_CSS_COMPOSITING "Toggle CSS Compositing support" PRIVATE ON)
    WEBKIT_OPTION_DEFINE(ENABLE_CSS_CONIC_GRADIENTS "Toggle CSS Conic Gradient support" PRIVATE OFF)
    WEBKIT_OPTION_DEFINE(ENABLE_CSS_DEVICE_ADAPTATION "Toggle CSS Device Adaptation support" PRIVATE OFF)
    WEBKIT_OPTION_DEFINE(ENABLE_CSS_IMAGE_RESOLUTION "Toggle CSS image-resolution support" PRIVATE OFF)
    WEBKIT_OPTION_DEFINE(ENABLE_CSS_PAINTING_API "Toggle CSS Painting API support" PRIVATE OFF)
    WEBKIT_OPTION_DEFINE(ENABLE_CSS_SELECTORS_LEVEL4 "Toggle CSS Selectors Level 4 support" PRIVATE ON)
    WEBKIT_OPTION_DEFINE(ENABLE_CSS_TRAILING_WORD "Toggle css trailing word" PRIVATE OFF)
    WEBKIT_OPTION_DEFINE(ENABLE_CSS_TYPED_OM "Toggle CSS Typed OM support" PRIVATE OFF)
    WEBKIT_OPTION_DEFINE(ENABLE_CURSOR_VISIBILITY "Toggle cursor visibility support" PRIVATE OFF)
    WEBKIT_OPTION_DEFINE(ENABLE_C_LOOP "Enable CLoop interpreter" PRIVATE ${ENABLE_C_LOOP_DEFAULT})
    WEBKIT_OPTION_DEFINE(ENABLE_DARK_MODE_CSS "Toggle Dark Mode CSS support" PRIVATE OFF)
    WEBKIT_OPTION_DEFINE(ENABLE_DATACUE_VALUE "Toggle datacue value support" PRIVATE OFF)
    WEBKIT_OPTION_DEFINE(ENABLE_DATALIST_ELEMENT "Toggle Datalist Element support" PRIVATE OFF)
    WEBKIT_OPTION_DEFINE(ENABLE_DEVICE_ORIENTATION "Toggle Device Orientation support" PRIVATE OFF)
    WEBKIT_OPTION_DEFINE(ENABLE_DFG_JIT "Toggle data flow graph JIT tier" PRIVATE ${ENABLE_JIT_DEFAULT})
    WEBKIT_OPTION_DEFINE(ENABLE_DOWNLOAD_ATTRIBUTE "Toggle Download Attribute support" PRIVATE OFF)
    WEBKIT_OPTION_DEFINE(ENABLE_DRAG_SUPPORT "Toggle support of drag actions (including selection of text with mouse)" PRIVATE OFF)
    WEBKIT_OPTION_DEFINE(ENABLE_ENCRYPTED_MEDIA "Toggle EME V3 support" PRIVATE OFF)
    WEBKIT_OPTION_DEFINE(ENABLE_EXPERIMENTAL_FEATURES "Enable experimental features" PRIVATE OFF)
    WEBKIT_OPTION_DEFINE(ENABLE_FILTERS_LEVEL_2 "Toggle Filters Module Level 2" PRIVATE OFF)
    WEBKIT_OPTION_DEFINE(ENABLE_FTL_JIT "Toggle FTL JIT support" PRIVATE ${ENABLE_FTL_DEFAULT})
    WEBKIT_OPTION_DEFINE(ENABLE_FTPDIR "Toggle FTP Directory support" PRIVATE ON)
    WEBKIT_OPTION_DEFINE(ENABLE_FULLSCREEN_API "Toggle Fullscreen API support" PRIVATE ON)
    WEBKIT_OPTION_DEFINE(ENABLE_GPU_PROCESS "Toggle GPU Process support" PRIVATE OFF)
    WEBKIT_OPTION_DEFINE(ENABLE_INPUT_TYPE_COLOR "Toggle Input Type Color support" PRIVATE OFF)
    WEBKIT_OPTION_DEFINE(ENABLE_INPUT_TYPE_DATE "Toggle Input Type Date support" PRIVATE OFF)
    WEBKIT_OPTION_DEFINE(ENABLE_INPUT_TYPE_DATETIMELOCAL "Toggle Input Type Datetimelocal support" PRIVATE OFF)
    WEBKIT_OPTION_DEFINE(ENABLE_INPUT_TYPE_MONTH "Toggle Input Type Month support" PRIVATE OFF)
    WEBKIT_OPTION_DEFINE(ENABLE_INPUT_TYPE_TIME "Toggle Input Type Time support" PRIVATE OFF)
    WEBKIT_OPTION_DEFINE(ENABLE_INPUT_TYPE_WEEK "Toggle Input Type Week support" PRIVATE OFF)
    WEBKIT_OPTION_DEFINE(ENABLE_INSPECTOR_ALTERNATE_DISPATCHERS "Toggle inspector alternate dispatchers support" PRIVATE OFF)
    WEBKIT_OPTION_DEFINE(ENABLE_INSPECTOR_EXTENSIONS "Toggle inspector web extensions support" PRIVATE OFF)
    WEBKIT_OPTION_DEFINE(ENABLE_INSPECTOR_TELEMETRY "Toggle inspector telemetry support" PRIVATE OFF)
    WEBKIT_OPTION_DEFINE(ENABLE_INTERSECTION_OBSERVER "Enable Intersection Observer support" PRIVATE ON)
    WEBKIT_OPTION_DEFINE(ENABLE_IOS_GESTURE_EVENTS "Toggle iOS gesture events support" PRIVATE OFF)
    WEBKIT_OPTION_DEFINE(ENABLE_IOS_TOUCH_EVENTS "Toggle iOS touch events support" PRIVATE OFF)
    WEBKIT_OPTION_DEFINE(ENABLE_JIT "Toggle JustInTime JavaScript support" PRIVATE ${ENABLE_JIT_DEFAULT})
    WEBKIT_OPTION_DEFINE(ENABLE_LAYOUT_FORMATTING_CONTEXT "Toggle Layout Formatting Context support" PRIVATE OFF)
    WEBKIT_OPTION_DEFINE(ENABLE_LEGACY_CUSTOM_PROTOCOL_MANAGER "Toggle legacy protocol manager support" PRIVATE OFF)
    WEBKIT_OPTION_DEFINE(ENABLE_LEGACY_ENCRYPTED_MEDIA "Toggle Legacy EME V2 support" PRIVATE OFF)
    WEBKIT_OPTION_DEFINE(ENABLE_LETTERPRESS "Toggle letterpress support" PRIVATE OFF)
    WEBKIT_OPTION_DEFINE(ENABLE_MAC_GESTURE_EVENTS "Toggle Mac gesture events support" PRIVATE OFF)
    WEBKIT_OPTION_DEFINE(ENABLE_MATHML "Toggle MathML support" PRIVATE ON)
    WEBKIT_OPTION_DEFINE(ENABLE_MEDIA_CAPTURE "Toggle Media Capture support" PRIVATE OFF)
    WEBKIT_OPTION_DEFINE(ENABLE_MEDIA_CONTROLS_SCRIPT "Toggle definition of media controls in Javascript" PRIVATE ON)
    WEBKIT_OPTION_DEFINE(ENABLE_MEDIA_SOURCE "Toggle Media Source support" PRIVATE OFF)
    WEBKIT_OPTION_DEFINE(ENABLE_MEDIA_STATISTICS "Toggle Media Statistics support" PRIVATE OFF)
    WEBKIT_OPTION_DEFINE(ENABLE_MEDIA_STREAM "Toggle Media Stream support" PRIVATE OFF)
    WEBKIT_OPTION_DEFINE(ENABLE_MEMORY_SAMPLER "Toggle Memory Sampler support" PRIVATE OFF)
    WEBKIT_OPTION_DEFINE(ENABLE_MHTML "Toggle MHTML support" PRIVATE OFF)
    WEBKIT_OPTION_DEFINE(ENABLE_MINIBROWSER "Toggle MiniBrowser compilation." PRIVATE OFF)
    WEBKIT_OPTION_DEFINE(ENABLE_MOUSE_CURSOR_SCALE "Toggle Scaled mouse cursor support" PRIVATE OFF)
    WEBKIT_OPTION_DEFINE(ENABLE_NAVIGATOR_STANDALONE "Toogle standalone navigator support" PRIVATE OFF)
    WEBKIT_OPTION_DEFINE(ENABLE_NETSCAPE_PLUGIN_API "Toggle Netscape Plugin API support" PRIVATE ON)
    WEBKIT_OPTION_DEFINE(ENABLE_NETWORK_CACHE_SPECULATIVE_REVALIDATION "Toogle network cache speculative revalidation support" PRIVATE OFF)
    WEBKIT_OPTION_DEFINE(ENABLE_NETWORK_CACHE_STALE_WHILE_REVALIDATE "Toogle network cache stale-while-revalidate caching strategy" PRIVATE OFF)
    WEBKIT_OPTION_DEFINE(ENABLE_NOTIFICATIONS "Toggle Notifications support" PRIVATE ON)
    WEBKIT_OPTION_DEFINE(ENABLE_OFFSCREEN_CANVAS "Toggle OffscreenCanvas support" PRIVATE OFF)
    WEBKIT_OPTION_DEFINE(ENABLE_OFFSCREEN_CANVAS_IN_WORKERS "Toggle OffscreenCanvas in Workers support" PRIVATE OFF)
    WEBKIT_OPTION_DEFINE(ENABLE_THUNDER "Toggle EME V3 Thunder support" PRIVATE OFF)
    WEBKIT_OPTION_DEFINE(ENABLE_ORIENTATION_EVENTS "Toggle Orientation Events support" PRIVATE OFF)
    WEBKIT_OPTION_DEFINE(ENABLE_OVERFLOW_SCROLLING_TOUCH "Toggle accelerated scrolling support" PRIVATE OFF)
    WEBKIT_OPTION_DEFINE(ENABLE_PDFKIT_PLUGIN "Toggle PDFKit plugin support" PRIVATE OFF)
    WEBKIT_OPTION_DEFINE(ENABLE_PERIODIC_MEMORY_MONITOR "Toggle periodical memory monitor support" PRIVATE OFF)
    WEBKIT_OPTION_DEFINE(ENABLE_PICTURE_IN_PICTURE_API "Toggle Picture-in-Picture API support" PRIVATE OFF)
    WEBKIT_OPTION_DEFINE(ENABLE_POINTER_LOCK "Toggle pointer lock support" PRIVATE OFF)
    WEBKIT_OPTION_DEFINE(ENABLE_PUBLIC_SUFFIX_LIST "Toggle public suffix list support" PRIVATE ON)
    WEBKIT_OPTION_DEFINE(ENABLE_REMOTE_INSPECTOR "Toggle remote inspector support" PRIVATE ON)
    WEBKIT_OPTION_DEFINE(ENABLE_RESIZE_OBSERVER "Toggle Resize Observer support" PRIVATE ON)
    WEBKIT_OPTION_DEFINE(ENABLE_RESOLUTION_MEDIA_QUERY "Toggle resolution media query support" PRIVATE OFF)
    WEBKIT_OPTION_DEFINE(ENABLE_RESOURCE_LOAD_STATISTICS "Toggle resource load statistics support" PRIVATE OFF)
    WEBKIT_OPTION_DEFINE(ENABLE_RESOURCE_USAGE "Toggle resource usage support" PRIVATE OFF)
    WEBKIT_OPTION_DEFINE(ENABLE_RUBBER_BANDING "Toggle rubber banding support" PRIVATE OFF)
    WEBKIT_OPTION_DEFINE(ENABLE_SAMPLING_PROFILER "Toggle sampling profiler support" PRIVATE ${ENABLE_SAMPLING_PROFILER_DEFAULT})
    WEBKIT_OPTION_DEFINE(ENABLE_SANDBOX_EXTENSIONS "Toggle sandbox extensions support" PRIVATE OFF)
    WEBKIT_OPTION_DEFINE(ENABLE_SERVER_PRECONNECT "Toggle server preconnect support" PRIVATE OFF)
    WEBKIT_OPTION_DEFINE(ENABLE_SERVICE_CONTROLS "Toggle service controls support" PRIVATE OFF)
    WEBKIT_OPTION_DEFINE(ENABLE_SERVICE_WORKER "Toggle Service Worker support" PRIVATE OFF)
    WEBKIT_OPTION_DEFINE(ENABLE_SHAREABLE_RESOURCE "Toggle network shareable resources support" PRIVATE OFF)
    WEBKIT_OPTION_DEFINE(ENABLE_SMOOTH_SCROLLING "Toggle smooth scrolling" PRIVATE ON)
    WEBKIT_OPTION_DEFINE(ENABLE_SPEECH_SYNTHESIS "Toggle Speech Synthesis API support" PRIVATE OFF)
    WEBKIT_OPTION_DEFINE(ENABLE_SPELLCHECK "Toggle Spellchecking support (requires Enchant)" PRIVATE OFF)
    WEBKIT_OPTION_DEFINE(ENABLE_TELEPHONE_NUMBER_DETECTION "Toggle telephone number detection support" PRIVATE OFF)
    WEBKIT_OPTION_DEFINE(ENABLE_TEXT_AUTOSIZING "Toggle automatic text size adjustment support" PRIVATE OFF)
    WEBKIT_OPTION_DEFINE(ENABLE_TOUCH_EVENTS "Toggle Touch Events support" PRIVATE OFF)
    WEBKIT_OPTION_DEFINE(ENABLE_TOUCH_SLIDER "Toggle Touch Slider support" PRIVATE OFF)
    WEBKIT_OPTION_DEFINE(ENABLE_UNIFIED_BUILDS "Toggle unified builds" PRIVATE ${ENABLE_UNIFIED_BUILDS_DEFAULT})
    WEBKIT_OPTION_DEFINE(ENABLE_USER_MESSAGE_HANDLERS "Toggle user script message handler support" PRIVATE ON)
    WEBKIT_OPTION_DEFINE(ENABLE_VARIATION_FONTS "Toggle variation fonts support" PRIVATE OFF)
    WEBKIT_OPTION_DEFINE(ENABLE_VIDEO "Toggle Video support" PRIVATE ON)
    WEBKIT_OPTION_DEFINE(ENABLE_VIDEO_PRESENTATION_MODE "Toggle Video presentation mode support" PRIVATE OFF)
    WEBKIT_OPTION_DEFINE(ENABLE_VIDEO_USES_ELEMENT_FULLSCREEN "Toggle video element fullscreen support" PRIVATE ON)
    WEBKIT_OPTION_DEFINE(ENABLE_WEBASSEMBLY "Toggle WebAssembly support" PRIVATE ${ENABLE_JIT_DEFAULT})
    WEBKIT_OPTION_DEFINE(ENABLE_WEBASSEMBLY_B3JIT "Toggle WebAssembly B3 JIT support" PRIVATE ${ENABLE_FTL_DEFAULT})
    WEBKIT_OPTION_DEFINE(ENABLE_WEBDRIVER "Toggle WebDriver service process" PRIVATE OFF)
    WEBKIT_OPTION_DEFINE(ENABLE_WEBDRIVER_KEYBOARD_INTERACTIONS "Toggle WebDriver keyboard interactions" PRIVATE OFF)
    WEBKIT_OPTION_DEFINE(ENABLE_WEBDRIVER_MOUSE_INTERACTIONS "Toggle WebDriver mouse interactions" PRIVATE OFF)
    WEBKIT_OPTION_DEFINE(ENABLE_WEBDRIVER_TOUCH_INTERACTIONS "Toggle WebDriver touch interactions" PRIVATE OFF)
    WEBKIT_OPTION_DEFINE(ENABLE_WEBDRIVER_WHEEL_INTERACTIONS "Toggle WebDriver wheel interactions" PRIVATE OFF)
    WEBKIT_OPTION_DEFINE(ENABLE_WEBGL "Toggle WebGL support" PRIVATE ON)
    WEBKIT_OPTION_DEFINE(ENABLE_WEBGL2 "Toggle WebGL2 support" PRIVATE OFF)
    WEBKIT_OPTION_DEFINE(ENABLE_WEB_API_STATISTICS "Toggle Web API statistics support" PRIVATE OFF)
    WEBKIT_OPTION_DEFINE(ENABLE_WEB_AUDIO "Toggle Web Audio support" PRIVATE ON)
    WEBKIT_OPTION_DEFINE(ENABLE_WEB_AUTHN "Toggle Web AuthN support" PRIVATE OFF)
    WEBKIT_OPTION_DEFINE(ENABLE_WEB_CRYPTO "Toggle WebCrypto Subtle-Crypto support" PRIVATE ON)
    WEBKIT_OPTION_DEFINE(ENABLE_WEB_RTC "Toggle WebRTC support" PRIVATE OFF)
    WEBKIT_OPTION_DEFINE(ENABLE_WEBXR "Toggle WebXR support" PRIVATE OFF)
    WEBKIT_OPTION_DEFINE(ENABLE_WIRELESS_PLAYBACK_TARGET "Toggle wireless playback target support" PRIVATE OFF)
    WEBKIT_OPTION_DEFINE(ENABLE_XSLT "Toggle XSLT support" PRIVATE ON)
    WEBKIT_OPTION_DEFINE(USE_SYSTEM_MALLOC "Toggle system allocator instead of WebKit's custom allocator" PRIVATE ${USE_SYSTEM_MALLOC_DEFAULT})

    WEBKIT_OPTION_CONFLICT(ENABLE_JIT ENABLE_C_LOOP)
    WEBKIT_OPTION_CONFLICT(ENABLE_SAMPLING_PROFILER ENABLE_C_LOOP)

    WEBKIT_OPTION_DEPEND(ENABLE_ACCESSIBILITY_ISOLATED_TREE ENABLE_ACCESSIBILITY)
    WEBKIT_OPTION_DEPEND(ENABLE_CSS_PAINTING_API ENABLE_CSS_TYPED_OM)
    WEBKIT_OPTION_DEPEND(ENABLE_WEB_RTC ENABLE_MEDIA_STREAM)
    WEBKIT_OPTION_DEPEND(ENABLE_ENCRYPTED_MEDIA ENABLE_VIDEO)
    WEBKIT_OPTION_DEPEND(ENABLE_LEGACY_ENCRYPTED_MEDIA ENABLE_VIDEO)
    WEBKIT_OPTION_DEPEND(ENABLE_DFG_JIT ENABLE_JIT)
    WEBKIT_OPTION_DEPEND(ENABLE_FTL_JIT ENABLE_DFG_JIT)
    WEBKIT_OPTION_DEPEND(ENABLE_WEBASSEMBLY ENABLE_JIT)
    WEBKIT_OPTION_DEPEND(ENABLE_WEBASSEMBLY_B3JIT ENABLE_FTL_JIT)
    WEBKIT_OPTION_DEPEND(ENABLE_INSPECTOR_ALTERNATE_DISPATCHERS ENABLE_REMOTE_INSPECTOR)
    WEBKIT_OPTION_DEPEND(ENABLE_MEDIA_CONTROLS_SCRIPT ENABLE_VIDEO)
    WEBKIT_OPTION_DEPEND(ENABLE_MEDIA_SOURCE ENABLE_VIDEO)
    WEBKIT_OPTION_DEPEND(ENABLE_MEDIA_STREAM ENABLE_VIDEO)
    WEBKIT_OPTION_DEPEND(ENABLE_THUNDER ENABLE_ENCRYPTED_MEDIA)
    WEBKIT_OPTION_DEPEND(ENABLE_VIDEO_PRESENTATION_MODE ENABLE_VIDEO)
    WEBKIT_OPTION_DEPEND(ENABLE_VIDEO_USES_ELEMENT_FULLSCREEN ENABLE_VIDEO)
    WEBKIT_OPTION_DEPEND(ENABLE_TOUCH_SLIDER ENABLE_TOUCH_EVENTS)
    WEBKIT_OPTION_DEPEND(ENABLE_WEBXR ENABLE_WEBGL)
endmacro()

macro(_WEBKIT_OPTION_ENFORCE_DEPENDS _name)
    foreach (_dependency ${_WEBKIT_AVAILABLE_OPTIONS_${_name}_DEPENDENCIES})
        if (NOT ${_dependency})
            message(STATUS "Disabling ${_name} since ${_dependency} is disabled.")
            set(${_name} OFF)
            set(_OPTION_CHANGED TRUE)
            break ()
        endif ()
    endforeach ()
endmacro()

macro(_WEBKIT_OPTION_ENFORCE_ALL_DEPENDS)
    set(_OPTION_CHANGED TRUE)
    while (${_OPTION_CHANGED})
        set(_OPTION_CHANGED FALSE)
        foreach (_name ${_WEBKIT_AVAILABLE_OPTIONS})
            if (${_name})
                _WEBKIT_OPTION_ENFORCE_DEPENDS(${_name})
            endif ()
        endforeach ()
    endwhile ()
endmacro()

macro(_WEBKIT_OPTION_ENFORCE_CONFLICTS _name)
    foreach (_conflict ${_WEBKIT_AVAILABLE_OPTIONS_${_name}_CONFLICTS})
        if (${_conflict})
            message(FATAL_ERROR "${_name} conflicts with ${_conflict}. You must disable one or the other.")
        endif ()
    endforeach ()
endmacro()

macro(_WEBKIT_OPTION_ENFORCE_ALL_CONFLICTS)
    foreach (_name ${_WEBKIT_AVAILABLE_OPTIONS})
        if (${_name})
            _WEBKIT_OPTION_ENFORCE_CONFLICTS(${_name})
        endif ()
    endforeach ()
endmacro()

macro(WEBKIT_OPTION_END)
    set(_SETTING_WEBKIT_OPTIONS FALSE)

    list(SORT _WEBKIT_AVAILABLE_OPTIONS)
    set(_MAX_FEATURE_LENGTH 0)
    foreach (_name ${_WEBKIT_AVAILABLE_OPTIONS})
        string(LENGTH ${_name} _name_length)
        if (_name_length GREATER _MAX_FEATURE_LENGTH)
            set(_MAX_FEATURE_LENGTH ${_name_length})
        endif ()

        option(${_name} "${_WEBKIT_AVAILABLE_OPTIONS_DESCRIPTION_${_name}}" ${_WEBKIT_AVAILABLE_OPTIONS_INITIAL_VALUE_${_name}})
        if (NOT ${_WEBKIT_AVAILABLE_OPTIONS_IS_PUBLIC_${_name}})
            mark_as_advanced(FORCE ${_name})
        endif ()
    endforeach ()

    # Run through every possible depends to make sure we have disabled anything
    # that could cause an unnecessary conflict before processing conflicts.
    _WEBKIT_OPTION_ENFORCE_ALL_DEPENDS()
    _WEBKIT_OPTION_ENFORCE_ALL_CONFLICTS()

    foreach (_name ${_WEBKIT_AVAILABLE_OPTIONS})
        if (${_name})
            list(APPEND FEATURE_DEFINES ${_name})
            set(FEATURE_DEFINES_WITH_SPACE_SEPARATOR "${FEATURE_DEFINES_WITH_SPACE_SEPARATOR} ${_name}")
        endif ()
    endforeach ()
endmacro()

macro(PRINT_WEBKIT_OPTIONS)
    message(STATUS "Enabled features:")

    set(_should_print_dots ON)
    foreach (_name ${_WEBKIT_AVAILABLE_OPTIONS})
        if (${_WEBKIT_AVAILABLE_OPTIONS_IS_PUBLIC_${_name}})
            string(LENGTH ${_name} _name_length)
            set(_message " ${_name} ")

            # Print dots on every other row, for readability.
            foreach (IGNORE RANGE ${_name_length} ${_MAX_FEATURE_LENGTH})
                if (${_should_print_dots})
                    set(_message "${_message}.")
                else ()
                    set(_message "${_message} ")
                endif ()
            endforeach ()

            set(_should_print_dots (NOT ${_should_print_dots}))

            set(_message "${_message} ${${_name}}")
            message(STATUS "${_message}")
        endif ()
    endforeach ()
endmacro()

set(_WEBKIT_CONFIG_FILE_VARIABLES "")

macro(EXPOSE_VARIABLE_TO_BUILD _variable_name)
    list(APPEND _WEBKIT_CONFIG_FILE_VARIABLES ${_variable_name})
endmacro()

macro(SET_AND_EXPOSE_TO_BUILD _variable_name)
    # It's important to handle the case where the value isn't passed, because often
    # during configuration an empty variable is the result of a failed package search.
    if (${ARGC} GREATER 1)
        set(_variable_value ${ARGV1})
    else ()
        set(_variable_value OFF)
    endif ()

    set(${_variable_name} ${_variable_value})
    EXPOSE_VARIABLE_TO_BUILD(${_variable_name})
endmacro()

macro(_ADD_CONFIGURATION_LINE_TO_HEADER_STRING _string _variable_name _output_variable_name)
    if (${${_variable_name}})
        set(${_string} "${_file_contents}#define ${_output_variable_name} 1\n")
    else ()
        set(${_string} "${_file_contents}#define ${_output_variable_name} 0\n")
    endif ()
endmacro()

macro(CREATE_CONFIGURATION_HEADER)
    list(SORT _WEBKIT_CONFIG_FILE_VARIABLES)
    set(_file_contents "#ifndef CMAKECONFIG_H\n")
    set(_file_contents "${_file_contents}#define CMAKECONFIG_H\n\n")

    foreach (_variable_name ${_WEBKIT_CONFIG_FILE_VARIABLES})
        _ADD_CONFIGURATION_LINE_TO_HEADER_STRING(_file_contents ${_variable_name} ${_variable_name})
    endforeach ()
    set(_file_contents "${_file_contents}\n#endif /* CMAKECONFIG_H */\n")

    file(WRITE "${CMAKE_BINARY_DIR}/cmakeconfig.h.tmp" "${_file_contents}")
    execute_process(COMMAND ${CMAKE_COMMAND}
        -E copy_if_different
        "${CMAKE_BINARY_DIR}/cmakeconfig.h.tmp"
        "${CMAKE_BINARY_DIR}/cmakeconfig.h"
    )
    file(REMOVE "${CMAKE_BINARY_DIR}/cmakeconfig.h.tmp")
endmacro()

macro(WEBKIT_CHECK_HAVE_INCLUDE _variable _header)
    check_include_file(${_header} ${_variable}_value)
    SET_AND_EXPOSE_TO_BUILD(${_variable} ${${_variable}_value})
endmacro()

macro(WEBKIT_CHECK_HAVE_FUNCTION _variable _function)
    check_function_exists(${_function} ${_variable}_value)
    SET_AND_EXPOSE_TO_BUILD(${_variable} ${${_variable}_value})
endmacro()

macro(WEBKIT_CHECK_HAVE_SYMBOL _variable _symbol _header)
    check_symbol_exists(${_symbol} "${_header}" ${_variable}_value)
    SET_AND_EXPOSE_TO_BUILD(${_variable} ${${_variable}_value})
endmacro()

macro(WEBKIT_CHECK_HAVE_STRUCT _variable _struct _member _header)
    check_struct_has_member(${_struct} ${_member} "${_header}" ${_variable}_value)
    SET_AND_EXPOSE_TO_BUILD(${_variable} ${${_variable}_value})
endmacro()

macro(WEBKIT_CHECK_SOURCE_COMPILES _variable _source)
    check_cxx_source_compiles("${_source}" ${_variable}_value)
    SET_AND_EXPOSE_TO_BUILD(${_variable} ${${_variable}_value})
endmacro()

option(ENABLE_EXPERIMENTAL_FEATURES "Enable experimental features" OFF)
SET_AND_EXPOSE_TO_BUILD(ENABLE_EXPERIMENTAL_FEATURES ${ENABLE_EXPERIMENTAL_FEATURES})
