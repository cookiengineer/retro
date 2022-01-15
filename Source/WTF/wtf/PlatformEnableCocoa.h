/*
 * Copyright (C) 2006-2020 Apple Inc. All rights reserved.
 * Copyright (C) 2007-2009 Torch Mobile, Inc.
 * Copyright (C) 2010, 2011 Research In Motion Limited. All rights reserved.
 * Copyright (C) 2013 Samsung Electronics. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#ifndef WTF_PLATFORM_GUARD_AGAINST_INDIRECT_INCLUSION
#error "Please #include <wtf/Platform.h> instead of this file directly."
#endif

#if !PLATFORM(COCOA)
#error "This file should only be included when building for one of Apple's Cocoa platforms."
#endif

/* Please keep the following in alphabetical order so we can notice duplicates. */
/* Items should only be here if they are different from the defaults in PlatformEnable.h. */

#if !defined(ENABLE_3D_TRANSFORMS)
#define ENABLE_3D_TRANSFORMS 1
#endif

#if !defined(ENABLE_ACCESSIBILITY_ISOLATED_TREE) && PLATFORM(MAC) && __MAC_OS_X_VERSION_MIN_REQUIRED >= 110000
#define ENABLE_ACCESSIBILITY_ISOLATED_TREE 1
#endif

#if !defined(ENABLE_AIRPLAY_PICKER) && !PLATFORM(MAC) && !PLATFORM(MACCATALYST)
#define ENABLE_AIRPLAY_PICKER 1
#endif

#if !defined(ENABLE_ANIMATED_KEYBOARD_SCROLLING) && PLATFORM(IOS_FAMILY)
#define ENABLE_ANIMATED_KEYBOARD_SCROLLING 1
#endif

#if !defined(ENABLE_APPLICATION_MANIFEST)
#define ENABLE_APPLICATION_MANIFEST 1
#endif

#if !defined(ENABLE_APP_BOUND_DOMAINS) && PLATFORM(IOS_FAMILY)
#define ENABLE_APP_BOUND_DOMAINS 1
#endif

#if !defined(ENABLE_ASYNC_SCROLLING)
#define ENABLE_ASYNC_SCROLLING 1
#endif

#if !defined(ENABLE_ATTACHMENT_ELEMENT)
#define ENABLE_ATTACHMENT_ELEMENT 1
#endif

#if !defined(ENABLE_AUTOCAPITALIZE) && !PLATFORM(MAC)
#define ENABLE_AUTOCAPITALIZE 1
#endif

#if !defined(ENABLE_AUTOCORRECT)
#define ENABLE_AUTOCORRECT 1
#endif

#if !defined(ENABLE_AVF_CAPTIONS)
#define ENABLE_AVF_CAPTIONS 1
#endif

#if !defined(ENABLE_CACHE_PARTITIONING)
#define ENABLE_CACHE_PARTITIONING 1
#endif

#if !defined(ENABLE_CFPREFS_DIRECT_MODE) && (PLATFORM(IOS_FAMILY) || PLATFORM(MAC) && __MAC_OS_X_VERSION_MIN_REQUIRED >= 110000)
#define ENABLE_CFPREFS_DIRECT_MODE 1
#endif

#if !defined(ENABLE_CONTENT_CHANGE_OBSERVER) && PLATFORM(IOS_FAMILY)
#define ENABLE_CONTENT_CHANGE_OBSERVER 1
#endif

#if !defined(ENABLE_CONTENT_EXTENSIONS)
#define ENABLE_CONTENT_EXTENSIONS 1
#endif

#if !defined(ENABLE_CONTENT_FILTERING) && !PLATFORM(APPLETV)
#define ENABLE_CONTENT_FILTERING 1
#endif

#if !defined(ENABLE_CONTEXT_MENUS) && PLATFORM(IOS_FAMILY)
#define ENABLE_CONTEXT_MENUS 0
#endif

#if !defined(ENABLE_CONTEXT_MENU_EVENT) && PLATFORM(APPLETV)
#define ENABLE_CONTEXT_MENU_EVENT 0
#endif

#if !defined(ENABLE_CORE_IMAGE_ACCELERATED_FILTER_RENDER)
#define ENABLE_CORE_IMAGE_ACCELERATED_FILTER_RENDER 1
#endif

#if !defined(ENABLE_CSS_COMPOSITING)
#define ENABLE_CSS_COMPOSITING 1
#endif

// FIXME: Seems likely this can be enabled for tvOS.
#if !defined(ENABLE_CSS_CONIC_GRADIENTS) && !(PLATFORM(MAC) && __MAC_OS_X_VERSION_MIN_REQUIRED < 101400) && !PLATFORM(APPLETV)
#define ENABLE_CSS_CONIC_GRADIENTS 1
#endif

#if !defined(ENABLE_CSS_PAINTING_API)
#define ENABLE_CSS_PAINTING_API 1
#endif

#if !defined(ENABLE_CSS_SELECTORS_LEVEL4)
#define ENABLE_CSS_SELECTORS_LEVEL4 1
#endif

#if !defined(ENABLE_CSS_TRAILING_WORD)
#define ENABLE_CSS_TRAILING_WORD 1
#endif

#if !defined(ENABLE_CSS_TYPED_OM)
#define ENABLE_CSS_TYPED_OM 1
#endif

#if !defined(ENABLE_CURSOR_VISIBILITY)
#define ENABLE_CURSOR_VISIBILITY 1
#endif

#if !defined(ENABLE_CUSTOM_CURSOR_SUPPORT) && PLATFORM(IOS_FAMILY)
#define ENABLE_CUSTOM_CURSOR_SUPPORT 0
#endif

#if !defined(ENABLE_DARK_MODE_CSS) && !PLATFORM(APPLETV)
#define ENABLE_DARK_MODE_CSS 1
#endif

#if !defined(ENABLE_DATACUE_VALUE)
#define ENABLE_DATACUE_VALUE 1
#endif

#if !defined(ENABLE_DATALIST_ELEMENT)
#define ENABLE_DATALIST_ELEMENT 1
#endif

#if !defined(ENABLE_DATA_DETECTION) && (PLATFORM(MAC) || PLATFORM(IOS) || (PLATFORM(MACCATALYST) && __IPHONE_OS_VERSION_MIN_REQUIRED >= 150000))
#define ENABLE_DATA_DETECTION 1
#endif

#if !defined(ENABLE_DRAG_SUPPORT) && PLATFORM(APPLETV)
#define ENABLE_DRAG_SUPPORT 0
#endif

#if !defined(ENABLE_EDITABLE_REGION) && PLATFORM(IOS)
#define ENABLE_EDITABLE_REGION 1
#endif

#if !defined(ENABLE_FILE_REPLACEMENT)
#define ENABLE_FILE_REPLACEMENT 1
#endif

#if !defined(ENABLE_FILESYSTEM_POSIX_FAST_PATH)
#define ENABLE_FILESYSTEM_POSIX_FAST_PATH 1
#endif

#if !defined(ENABLE_FILTERS_LEVEL_2)
#define ENABLE_FILTERS_LEVEL_2 1
#endif

#if !defined(ENABLE_FTL_JIT) && !USE(JSVALUE32_64)
#define ENABLE_FTL_JIT 1
#endif

#if !defined(ENABLE_FULL_KEYBOARD_ACCESS) && !PLATFORM(MACCATALYST) && !PLATFORM(APPLETV)
#define ENABLE_FULL_KEYBOARD_ACCESS 1
#endif

#if !defined(ENABLE_GPU_PROCESS)
#define ENABLE_GPU_PROCESS 1
#endif

#if !defined(ENABLE_GPU_PROCESS_BY_DEFAULT)
#if PLATFORM(MAC) \
    || ((PLATFORM(IOS) || PLATFORM(MACCATALYST)) && __IPHONE_OS_VERSION_MIN_REQUIRED >= 150000) \
    || (PLATFORM(APPLETV) && __TV_OS_VERSION_MIN_REQUIRED >= 150000)
#define ENABLE_GPU_PROCESS_BY_DEFAULT 1
#endif
#endif

#if !defined(ENABLE_GPU_DRIVER_PREWARMING) && PLATFORM(MAC) && __MAC_OS_X_VERSION_MIN_REQUIRED >= 101600
#define ENABLE_GPU_DRIVER_PREWARMING 1
#endif

#if !defined(ENABLE_HARDWARE_JPEG) && !(PLATFORM(MAC) && CPU(X86_64))
#define ENABLE_HARDWARE_JPEG 1
#endif

#if !defined(ENABLE_INPUT_TYPE_DATE)
#define ENABLE_INPUT_TYPE_DATE 1
#endif

#if !defined(ENABLE_INPUT_TYPE_DATETIMELOCAL)
#define ENABLE_INPUT_TYPE_DATETIMELOCAL 1
#endif

#if !defined(ENABLE_INPUT_TYPE_MONTH)
#define ENABLE_INPUT_TYPE_MONTH 1
#endif

#if !defined(ENABLE_INPUT_TYPE_TIME)
#define ENABLE_INPUT_TYPE_TIME 1
#endif

#if !defined(ENABLE_INPUT_TYPE_WEEK)
#define ENABLE_INPUT_TYPE_WEEK 1
#endif

#if !defined(ENABLE_INSPECTOR_ALTERNATE_DISPATCHERS)
#define ENABLE_INSPECTOR_ALTERNATE_DISPATCHERS 1
#endif

#if !defined(ENABLE_INSPECTOR_EXTENSIONS) && PLATFORM(MAC)
#define ENABLE_INSPECTOR_EXTENSIONS 1
#endif

#if !defined(ENABLE_INSPECTOR_TELEMETRY)
#define ENABLE_INSPECTOR_TELEMETRY 1
#endif

#if !defined(ENABLE_INTERSECTION_OBSERVER)
#define ENABLE_INTERSECTION_OBSERVER 1
#endif

#if !defined(ENABLE_IOS_FORM_CONTROL_REFRESH) && PLATFORM(IOS_FAMILY)
#define ENABLE_IOS_FORM_CONTROL_REFRESH 1
#endif

#if !defined(ENABLE_IOS_GESTURE_EVENTS) && PLATFORM(IOS_FAMILY) && USE(APPLE_INTERNAL_SDK)
#define ENABLE_IOS_GESTURE_EVENTS 1
#endif

#if !defined(ENABLE_IOS_TOUCH_EVENTS) && PLATFORM(IOS_FAMILY) && USE(APPLE_INTERNAL_SDK)
#define ENABLE_IOS_TOUCH_EVENTS 1
#endif

#if !defined(ENABLE_LAYOUT_FORMATTING_CONTEXT)
#define ENABLE_LAYOUT_FORMATTING_CONTEXT 1
#endif

#if !defined(ENABLE_LETTERPRESS) && PLATFORM(IOS_FAMILY)
#define ENABLE_LETTERPRESS 1
#endif

#if !defined(ENABLE_MAC_GESTURE_EVENTS) && PLATFORM(MAC) && USE(APPLE_INTERNAL_SDK)
#define ENABLE_MAC_GESTURE_EVENTS 1
#endif

#if !defined(ENABLE_MEDIA_CAPTURE) && !PLATFORM(MAC)
#define ENABLE_MEDIA_CAPTURE 1
#endif

#if !defined(ENABLE_MEDIA_CONTROLS_SCRIPT)
#define ENABLE_MEDIA_CONTROLS_SCRIPT 1
#endif

#if !defined(ENABLE_MEDIA_CONTROLS_CONTEXT_MENUS) && (PLATFORM(MAC) || ((PLATFORM(IOS) || PLATFORM(MACCATALYST)) && __IPHONE_OS_VERSION_MIN_REQUIRED >= 150000))
#define ENABLE_MEDIA_CONTROLS_CONTEXT_MENUS 1
#endif

#if !defined(ENABLE_MEDIA_SESSION_COORDINATOR) && PLATFORM(MAC) && __MAC_OS_X_VERSION_MAX_ALLOWED >= 120000
#define ENABLE_MEDIA_SESSION_COORDINATOR 1
#endif

#if !defined(ENABLE_MEDIA_SESSION_PLAYLIST) && PLATFORM(MAC) && __MAC_OS_X_VERSION_MAX_ALLOWED >= 120000
#define ENABLE_MEDIA_SESSION_PLAYLIST 1
#endif

#if !defined(ENABLE_MEDIA_SESSION) && !PLATFORM(APPLETV)
#define ENABLE_MEDIA_SESSION 1
#endif

#if !defined(ENABLE_MEDIA_SOURCE) && !PLATFORM(MACCATALYST) && !PLATFORM(APPLETV)
#define ENABLE_MEDIA_SOURCE 1
#endif

#if !defined(ENABLE_MEDIA_STREAM) && !PLATFORM(MACCATALYST) && !PLATFORM(APPLETV)
#define ENABLE_MEDIA_STREAM 1
#endif

#if !defined(ENABLE_MEDIA_USAGE) && ((PLATFORM(MAC) && __MAC_OS_X_VERSION_MIN_REQUIRED >= 110000) || (PLATFORM(MACCATALYST) && __IPHONE_OS_VERSION_MIN_REQUIRED >= 140000))
#define ENABLE_MEDIA_USAGE 1
#endif

#if !defined(ENABLE_MEMORY_SAMPLER)
#define ENABLE_MEMORY_SAMPLER 1
#endif

#if !defined(ENABLE_META_VIEWPORT) && PLATFORM(IOS_FAMILY)
#define ENABLE_META_VIEWPORT 1
#endif

// FIXME: We almost certainly do not need this monospace font exception for watchOS and tvOS.
#if !defined(ENABLE_MONOSPACE_FONT_EXCEPTION) && ((PLATFORM(MAC) && __MAC_OS_X_VERSION_MIN_REQUIRED < 101500) || PLATFORM(APPLETV))
#define ENABLE_MONOSPACE_FONT_EXCEPTION 1
#endif

#if !defined(ENABLE_MOUSE_CURSOR_SCALE) && PLATFORM(MAC)
#define ENABLE_MOUSE_CURSOR_SCALE 1
#endif

#if !defined(ENABLE_NAVIGATOR_STANDALONE) && PLATFORM(IOS_FAMILY)
#define ENABLE_NAVIGATOR_STANDALONE 1
#endif

#if !defined(ENABLE_NETWORK_CACHE_SPECULATIVE_REVALIDATION)
#define ENABLE_NETWORK_CACHE_SPECULATIVE_REVALIDATION 1
#endif

#if !defined(ENABLE_NETWORK_CACHE_STALE_WHILE_REVALIDATE)
#define ENABLE_NETWORK_CACHE_STALE_WHILE_REVALIDATE 1
#endif

#if !defined(ENABLE_NON_VISIBLE_WEBPROCESS_MEMORY_CLEANUP_TIMER) && (PLATFORM(MAC) || PLATFORM(MACCATALYST))
#define ENABLE_NON_VISIBLE_WEBPROCESS_MEMORY_CLEANUP_TIMER 1
#endif

#if !defined(ENABLE_NOTIFICATIONS) && PLATFORM(MAC)
#define ENABLE_NOTIFICATIONS 1
#endif

#if !defined(ENABLE_OVERFLOW_SCROLLING_TOUCH) && PLATFORM(IOS_FAMILY)
#define ENABLE_OVERFLOW_SCROLLING_TOUCH 1
#endif

#if !defined(ENABLE_PERIODIC_MEMORY_MONITOR) && PLATFORM(MAC)
#define ENABLE_PERIODIC_MEMORY_MONITOR 1
#endif

#if !defined(ENABLE_PLATFORM_DRIVEN_TEXT_CHECKING) && PLATFORM(MACCATALYST)
#define ENABLE_PLATFORM_DRIVEN_TEXT_CHECKING 1
#endif

#if !defined(ENABLE_POINTER_LOCK) && PLATFORM(IOS_FAMILY)
#define ENABLE_POINTER_LOCK 0
#endif

#if !defined(ENABLE_PRIMARY_SNAPSHOTTED_PLUGIN_HEURISTIC)
#define ENABLE_PRIMARY_SNAPSHOTTED_PLUGIN_HEURISTIC 1
#endif

#if !defined(ENABLE_PUBLIC_SUFFIX_LIST)
#define ENABLE_PUBLIC_SUFFIX_LIST 1
#endif

#if !defined(ENABLE_REMOTE_INSPECTOR)
#define ENABLE_REMOTE_INSPECTOR 1
#endif

#if !defined(ENABLE_RESIZE_OBSERVER)
#define ENABLE_RESIZE_OBSERVER 1
#endif

#if !defined(ENABLE_RESOURCE_LOAD_STATISTICS)
#define ENABLE_RESOURCE_LOAD_STATISTICS 1
#endif

#if !defined(ENABLE_SPEECH_SYNTHESIS) && !PLATFORM(MACCATALYST)
#define ENABLE_SPEECH_SYNTHESIS 1
#endif

#if !defined(ENABLE_RESOURCE_USAGE)
#define ENABLE_RESOURCE_USAGE 1
#endif

#if !defined(ENABLE_REVEAL) && !(PLATFORM(MAC) && __MAC_OS_X_VERSION_MIN_REQUIRED < 101500) && !PLATFORM(APPLETV)
#define ENABLE_REVEAL 1
#endif

#if !defined(ENABLE_ROUTING_ARBITRATION) && (PLATFORM(MAC) && __MAC_OS_X_VERSION_MIN_REQUIRED >= 110000)
#define ENABLE_ROUTING_ARBITRATION 1
#endif

#if !defined(ENABLE_RUBBER_BANDING) && PLATFORM(MAC)
#define ENABLE_RUBBER_BANDING 1
#endif

#if !defined(ENABLE_SANDBOX_EXTENSIONS)
#define ENABLE_SANDBOX_EXTENSIONS 1
#endif

#if !defined(ENABLE_SCROLLING_THREAD) && PLATFORM(MAC)
#define ENABLE_SCROLLING_THREAD 1
#endif

#if !defined(ENABLE_SEC_ITEM_SHIM)
#define ENABLE_SEC_ITEM_SHIM 1
#endif

#if !defined(ENABLE_SEPARATED_WX_HEAP) && CPU(ARM64)
#define ENABLE_SEPARATED_WX_HEAP 1
#endif

#if !defined(ENABLE_SERVER_PRECONNECT)
#define ENABLE_SERVER_PRECONNECT 1
#endif

#if !defined(ENABLE_SERVICE_CONTROLS) && PLATFORM(MAC)
#define ENABLE_SERVICE_CONTROLS 1
#endif

#if !defined(ENABLE_SHAREABLE_RESOURCE)
#define ENABLE_SHAREABLE_RESOURCE 1
#endif

#if !defined(ENABLE_SIGILL_CRASH_ANALYZER) && !PLATFORM(MAC)
#define ENABLE_SIGILL_CRASH_ANALYZER 1
#endif

#if !defined(ENABLE_SMOOTH_SCROLLING) && PLATFORM(MAC)
#define ENABLE_SMOOTH_SCROLLING 1
#endif

// FIXME: Should this be enabled for watchOS and tvOS?
#if !defined(ENABLE_TAKE_UNBOUNDED_NETWORKING_ASSERTION) && !PLATFORM(MAC) && !PLATFORM(APPLETV)
#define ENABLE_TAKE_UNBOUNDED_NETWORKING_ASSERTION 1
#endif

#if !defined(ENABLE_TELEPHONE_NUMBER_DETECTION)
#define ENABLE_TELEPHONE_NUMBER_DETECTION 1
#endif

#if !defined(ENABLE_TEXT_AUTOSIZING)
#define ENABLE_TEXT_AUTOSIZING 1
#endif

#if !defined(ENABLE_TEXT_CARET) && PLATFORM(IOS_FAMILY)
#define ENABLE_TEXT_CARET 0
#endif

#if !defined(ENABLE_TEXT_SELECTION) && PLATFORM(IOS_FAMILY)
#define ENABLE_TEXT_SELECTION 0
#endif

#if !defined(ENABLE_TOUCH_EVENTS) && PLATFORM(IOS_FAMILY) && USE(APPLE_INTERNAL_SDK)
#define ENABLE_TOUCH_EVENTS 1
#endif

#if !defined(ENABLE_TOUCH_ACTION_REGIONS) && PLATFORM(IOS_FAMILY)
#define ENABLE_TOUCH_ACTION_REGIONS 1
#endif

#if !defined(ENABLE_WHEEL_EVENT_REGIONS) && (PLATFORM(MAC) || PLATFORM(IOS) || PLATFORM(MACCATALYST))
#define ENABLE_WHEEL_EVENT_REGIONS 1
#endif

#if !defined(ENABLE_UI_SIDE_COMPOSITING)
#define ENABLE_UI_SIDE_COMPOSITING 1
#endif

#if !defined(ENABLE_USER_MESSAGE_HANDLERS)
#define ENABLE_USER_MESSAGE_HANDLERS 1
#endif

#if !defined(ENABLE_VARIATION_FONTS)
#define ENABLE_VARIATION_FONTS 1
#endif

#if !defined(ENABLE_VIDEO)
#define ENABLE_VIDEO 1
#endif

#if !defined(ENABLE_HOVER_GESTURE_RECOGNIZER) && HAVE(UIKIT_WEBKIT_INTERNALS)
#define ENABLE_HOVER_GESTURE_RECOGNIZER 1
#endif

#if !defined(ENABLE_VP9) && ((PLATFORM(MAC) && __MAC_OS_X_VERSION_MIN_REQUIRED >= 110000) || PLATFORM(IOS) || PLATFORM(IOS_SIMULATOR))
#define ENABLE_VP9 1
#endif

#if !defined(ENABLE_WEBDRIVER_KEYBOARD_INTERACTIONS)
#define ENABLE_WEBDRIVER_KEYBOARD_INTERACTIONS 1
#endif

#if !defined(ENABLE_WEBDRIVER_MOUSE_INTERACTIONS) && PLATFORM(MAC)
#define ENABLE_WEBDRIVER_MOUSE_INTERACTIONS 1
#endif

#if !defined(ENABLE_WEBGL)
#define ENABLE_WEBGL 1
#endif

#if !defined(ENABLE_WEBGL2)
#define ENABLE_WEBGL2 1
#endif

#if !defined(ENABLE_WEBPROCESS_NSRUNLOOP) && PLATFORM(MAC)
#define ENABLE_WEBPROCESS_NSRUNLOOP 1
#endif

#if !defined(ENABLE_WEB_ARCHIVE)
#define ENABLE_WEB_ARCHIVE 1
#endif

#if !defined(ENABLE_WEB_AUDIO)
#define ENABLE_WEB_AUDIO 1
#endif

#if !defined(ENABLE_WEB_AUTHN) && !PLATFORM(MACCATALYST) && !PLATFORM(APPLETV)
#define ENABLE_WEB_AUTHN 1
#endif

#if !defined(ENABLE_WEB_CRYPTO)
#define ENABLE_WEB_CRYPTO 1
#endif

#if !defined(ENABLE_WEB_PLAYBACK_CONTROLS_MANAGER) && PLATFORM(MAC)
#define ENABLE_WEB_PLAYBACK_CONTROLS_MANAGER 1
#endif

#if !defined(ENABLE_WIRELESS_PLAYBACK_TARGET)
#define ENABLE_WIRELESS_PLAYBACK_TARGET 1
#endif

#if !defined(ENABLE_WKPDFVIEW) && (PLATFORM(IOS) || (PLATFORM(MACCATALYST) && __IPHONE_OS_VERSION_MIN_REQUIRED >= 140000))
#define ENABLE_WKPDFVIEW 1
#endif

#if !defined(ENABLE_VORBIS) && PLATFORM(MAC) && __MAC_OS_X_VERSION_MAX_ALLOWED >= 110300
#define ENABLE_VORBIS 1
#endif

#if !defined(ENABLE_SET_WEBCONTENT_PROCESS_INFORMATION_IN_NETWORK_PROCESS) && PLATFORM(MAC) && __MAC_OS_X_VERSION_MIN_REQUIRED >= 120000
#define ENABLE_SET_WEBCONTENT_PROCESS_INFORMATION_IN_NETWORK_PROCESS 1
#endif

#if !defined(ENABLE_WEBM_FORMAT_READER) && PLATFORM(MAC) && __MAC_OS_X_VERSION_MAX_ALLOWED >= 110300
#define ENABLE_WEBM_FORMAT_READER 1
#endif

#if !defined(ENABLE_OPUS) && PLATFORM(MAC) && __MAC_OS_X_VERSION_MIN_REQUIRED >= 120000
#define ENABLE_OPUS 1
#endif

#if !defined(ENABLE_CSS_TRANSFORM_STYLE_OPTIMIZED_3D) && HAVE(UIKIT_WEBKIT_INTERNALS)
#define ENABLE_CSS_TRANSFORM_STYLE_OPTIMIZED_3D 1
#endif

#if !defined(ENABLE_NOTIFY_FILTERING) && PLATFORM(MAC) && __MAC_OS_X_VERSION_MIN_REQUIRED >= 120000
#define ENABLE_NOTIFY_FILTERING 1
#endif

#if !defined(ENABLE_APP_PRIVACY_REPORT) && PLATFORM(IOS_FAMILY) && __IPHONE_OS_VERSION_MIN_REQUIRED >= 150000
#define ENABLE_APP_PRIVACY_REPORT 1
#endif

#if !defined(ENABLE_DESTINATION_COLOR_SPACE_DISPLAY_P3)
#define ENABLE_DESTINATION_COLOR_SPACE_DISPLAY_P3 1
#endif

#if !defined(ENABLE_DESTINATION_COLOR_SPACE_LINEAR_SRGB)
#define ENABLE_DESTINATION_COLOR_SPACE_LINEAR_SRGB 1
#endif

#if !defined(ENABLE_IMAGE_ANALYSIS) && HAVE(VK_IMAGE_ANALYSIS)
#define ENABLE_IMAGE_ANALYSIS 1
#endif

#if !defined(ENABLE_IMAGE_ANALYSIS_FOR_MACHINE_READABLE_CODES) && HAVE(VK_IMAGE_ANALYSIS_FOR_MACHINE_READABLE_CODES)
#define ENABLE_IMAGE_ANALYSIS_FOR_MACHINE_READABLE_CODES 1
#endif
