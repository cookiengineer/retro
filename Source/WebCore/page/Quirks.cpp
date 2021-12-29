/*
 * Copyright (C) 2018-2020 Apple Inc. All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"
#include "Quirks.h"

#include "Attr.h"
#include "DOMTokenList.h"
#include "DOMWindow.h"
#include "DeprecatedGlobalSettings.h"
#include "Document.h"
#include "DocumentLoader.h"
#include "DocumentStorageAccess.h"
#include "EventNames.h"
#include "FrameLoader.h"
#include "HTMLBodyElement.h"
#include "HTMLDivElement.h"
#include "HTMLMetaElement.h"
#include "HTMLObjectElement.h"
#include "HTMLVideoElement.h"
#include "JSEventListener.h"
#include "LayoutUnit.h"
#include "NamedNodeMap.h"
#include "NetworkStorageSession.h"
#include "PlatformMouseEvent.h"
#include "RegistrableDomain.h"
#include "ResourceLoadObserver.h"
#include "RuntimeApplicationChecks.h"
#include "RuntimeEnabledFeatures.h"
#include "SVGPathElement.h"
#include "SVGSVGElement.h"
#include "ScriptController.h"
#include "ScriptSourceCode.h"
#include "Settings.h"
#include "SpaceSplitString.h"
#include "UserAgent.h"
#include "UserContentTypes.h"
#include "UserScript.h"
#include "UserScriptTypes.h"

#if PLATFORM(COCOA)
#include "VersionChecks.h"
#endif

namespace WebCore {

static inline OptionSet<AutoplayQuirk> allowedAutoplayQuirks(Document& document)
{
    auto* loader = document.loader();
    if (!loader)
        return { };

    return loader->allowedAutoplayQuirks();
}

#if ENABLE(PUBLIC_SUFFIX_LIST)
static inline bool isYahooMail(Document& document)
{
    auto host = document.topDocument().url().host();
    return startsWithLettersIgnoringASCIICase(host, "mail.") && topPrivatelyControlledDomain(host.toString()).startsWith("yahoo.");
}
#endif

Quirks::Quirks(Document& document)
    : m_document(makeWeakPtr(document))
{
}

Quirks::~Quirks() = default;

inline bool Quirks::needsQuirks() const
{
    return m_document && m_document->settings().needsSiteSpecificQuirks();
}

bool Quirks::shouldIgnoreInvalidSignal() const
{
    return needsQuirks();
}

bool Quirks::needsFormControlToBeMouseFocusable() const
{
#if PLATFORM(MAC)
    if (!needsQuirks())
        return false;

    auto host = m_document->url().host();
    return equalLettersIgnoringASCIICase(host, "ceac.state.gov") || host.endsWithIgnoringASCIICase(".ceac.state.gov");
#else
    return false;
#endif
}

bool Quirks::needsAutoplayPlayPauseEvents() const
{
    if (!needsQuirks())
        return false;

    if (allowedAutoplayQuirks(*m_document).contains(AutoplayQuirk::SynthesizedPauseEvents))
        return true;

    return allowedAutoplayQuirks(m_document->topDocument()).contains(AutoplayQuirk::SynthesizedPauseEvents);
}

bool Quirks::needsSeekingSupportDisabled() const
{
    if (!needsQuirks())
        return false;

    auto host = m_document->topDocument().url().host();
    return equalLettersIgnoringASCIICase(host, "netflix.com") || host.endsWithIgnoringASCIICase(".netflix.com");
}

bool Quirks::needsPerDocumentAutoplayBehavior() const
{
#if PLATFORM(MAC)
    ASSERT(m_document == &m_document->topDocument());
    return needsQuirks() && allowedAutoplayQuirks(*m_document).contains(AutoplayQuirk::PerDocumentAutoplayBehavior);
#else
    if (!needsQuirks())
        return false;

    auto host = m_document->topDocument().url().host();
    return equalLettersIgnoringASCIICase(host, "netflix.com") || host.endsWithIgnoringASCIICase(".netflix.com");
#endif
}

bool Quirks::shouldAutoplayForArbitraryUserGesture() const
{
#if PLATFORM(MAC)
    return needsQuirks() && allowedAutoplayQuirks(*m_document).contains(AutoplayQuirk::ArbitraryUserGestures);
#else
    if (!needsQuirks())
        return false;

    auto domain = RegistrableDomain { m_document->topDocument().url() };
    return domain == "twitter.com"_s || domain == "facebook.com"_s;
#endif
}

bool Quirks::shouldAutoplayWebAudioForArbitraryUserGesture() const
{
    if (!needsQuirks())
        return false;

    auto host = m_document->topDocument().url().host();
    return equalLettersIgnoringASCIICase(host, "www.bing.com") || host.endsWithIgnoringASCIICase(".zoom.us");
}

bool Quirks::hasBrokenEncryptedMediaAPISupportQuirk() const
{
    if (!needsQuirks())
        return false;

    if (m_hasBrokenEncryptedMediaAPISupportQuirk)
        return m_hasBrokenEncryptedMediaAPISupportQuirk.value();

    auto domain = m_document->securityOrigin().domain().convertToASCIILowercase();

    m_hasBrokenEncryptedMediaAPISupportQuirk = domain == "starz.com"
        || domain.endsWith(".starz.com")
        || domain == "youtube.com"
        || domain.endsWith(".youtube.com")
        || domain == "hulu.com"
        || domain.endsWith("hulu.com");

    return m_hasBrokenEncryptedMediaAPISupportQuirk.value();
}

bool Quirks::shouldDisableContentChangeObserverTouchEventAdjustment() const
{
    if (!needsQuirks())
        return false;

    auto& topDocument = m_document->topDocument();
    auto* topDocumentLoader = topDocument.loader();
    if (!topDocumentLoader || !topDocumentLoader->allowContentChangeObserverQuirk())
        return false;

    auto host = m_document->topDocument().url().host();
    return host.endsWith(".youtube.com") || host == "youtube.com";
}

bool Quirks::shouldTooltipPreventFromProceedingWithClick(const Element& element) const
{
    if (!needsQuirks())
        return false;

    if (!equalLettersIgnoringASCIICase(m_document->topDocument().url().host(), "covid.cdc.gov"))
        return false;
    return element.hasClass() && element.classNames().contains("tooltip");
}

// FIXME: Remove after the site is fixed, <rdar://problem/75792913>
bool Quirks::shouldHideSearchFieldResultsButton() const
{
#if ENABLE(IOS_FORM_CONTROL_REFRESH)
    if (!needsQuirks())
        return false;

    if (topPrivatelyControlledDomain(m_document->topDocument().url().host().toString()).startsWith("google."))
        return true;
#endif
    return false;
}

bool Quirks::needsMillisecondResolutionForHighResTimeStamp() const
{
    if (!needsQuirks())
        return false;
    // webkit.org/b/210527
    auto host = m_document->url().host();
    return equalLettersIgnoringASCIICase(host, "www.icourse163.org");
}

bool Quirks::shouldStripQuotationMarkInFontFaceSetFamily() const
{
    if (!needsQuirks())
        return false;

    auto host = m_document->topDocument().url().host();
    return equalLettersIgnoringASCIICase(host, "docs.google.com");
}

bool Quirks::isTouchBarUpdateSupressedForHiddenContentEditable() const
{
#if PLATFORM(MAC)
    if (!needsQuirks())
        return false;

    auto host = m_document->topDocument().url().host();
    return equalLettersIgnoringASCIICase(host, "docs.google.com");
#else
    return false;
#endif
}

bool Quirks::isNeverRichlyEditableForTouchBar() const
{
#if PLATFORM(MAC)
    if (!needsQuirks())
        return false;

    auto& url = m_document->topDocument().url();
    auto host = url.host();

    if (equalLettersIgnoringASCIICase(host, "twitter.com"))
        return true;

    if (equalLettersIgnoringASCIICase(host, "onedrive.live.com"))
        return true;

    if (equalLettersIgnoringASCIICase(host, "trix-editor.org"))
        return true;

    if (equalLettersIgnoringASCIICase(host, "www.icloud.com")) {
        auto path = url.path();
        if (path.contains("notes") || url.fragmentIdentifier().contains("notes"))
            return true;
    }
#endif

    return false;
}

static bool shouldSuppressAutocorrectionAndAutocaptializationInHiddenEditableAreasForHost(const StringView& host)
{
#if PLATFORM(IOS_FAMILY)
    return equalLettersIgnoringASCIICase(host, "docs.google.com");
#else
    UNUSED_PARAM(host);
    return false;
#endif
}

bool Quirks::shouldDispatchSyntheticMouseEventsWhenModifyingSelection() const
{
    if (m_document->settings().shouldDispatchSyntheticMouseEventsWhenModifyingSelection())
        return true;

    if (!needsQuirks())
        return false;

    auto host = m_document->topDocument().url().host();
    if (equalLettersIgnoringASCIICase(host, "medium.com") || host.endsWithIgnoringASCIICase(".medium.com"))
        return true;

    if (equalLettersIgnoringASCIICase(host, "weebly.com") || host.endsWithIgnoringASCIICase(".weebly.com"))
        return true;

    return false;
}

bool Quirks::needsYouTubeMouseOutQuirk() const
{
#if PLATFORM(IOS_FAMILY)
    if (m_document->settings().shouldDispatchSyntheticMouseOutAfterSyntheticClick())
        return true;

    if (!needsQuirks())
        return false;

    return equalLettersIgnoringASCIICase(m_document->url().host(), "www.youtube.com");
#else
    return false;
#endif
}

bool Quirks::shouldAvoidUsingIOS13ForGmail() const
{
#if PLATFORM(IOS_FAMILY)
    if (!needsQuirks())
        return false;

    auto& url = m_document->topDocument().url();
    return equalLettersIgnoringASCIICase(url.host(), "mail.google.com");
#else
    return false;
#endif
}

bool Quirks::shouldSuppressAutocorrectionAndAutocaptializationInHiddenEditableAreas() const
{
    if (!needsQuirks())
        return false;

    return shouldSuppressAutocorrectionAndAutocaptializationInHiddenEditableAreasForHost(m_document->topDocument().url().host());
}

#if ENABLE(TOUCH_EVENTS)
bool Quirks::isAmazon() const
{
    return topPrivatelyControlledDomain(m_document->topDocument().url().host().toString()).startsWith("amazon.");
}

bool Quirks::isGoogleMaps() const
{
    auto& url = m_document->topDocument().url();
    return topPrivatelyControlledDomain(url.host().toString()).startsWith("google.") && url.path().startsWithIgnoringASCIICase("/maps/");
}

bool Quirks::shouldDispatchSimulatedMouseEvents(EventTarget* target) const
{
    if (RuntimeEnabledFeatures::sharedFeatures().mouseEventsSimulationEnabled())
        return true;

    if (!needsQuirks())
        return false;

    auto doShouldDispatchChecks = [this] () -> ShouldDispatchSimulatedMouseEvents {
        auto* loader = m_document->loader();
        if (!loader || loader->simulatedMouseEventsDispatchPolicy() != SimulatedMouseEventsDispatchPolicy::Allow)
            return ShouldDispatchSimulatedMouseEvents::No;

        if (isAmazon())
            return ShouldDispatchSimulatedMouseEvents::Yes;
        if (isGoogleMaps())
            return ShouldDispatchSimulatedMouseEvents::Yes;

        auto& url = m_document->topDocument().url();
        auto host = url.host().convertToASCIILowercase();

        if (host == "wix.com" || host.endsWith(".wix.com")) {
            // Disable simulated mouse dispatching for template selection.
            return url.path().startsWithIgnoringASCIICase("/website/templates/") ? ShouldDispatchSimulatedMouseEvents::No : ShouldDispatchSimulatedMouseEvents::Yes;
        }

        if ((host == "desmos.com" || host.endsWith(".desmos.com")) && url.path().startsWithIgnoringASCIICase("/calculator/"))
            return ShouldDispatchSimulatedMouseEvents::Yes;
        if (host == "figma.com" || host.endsWith(".figma.com"))
            return ShouldDispatchSimulatedMouseEvents::Yes;
        if (host == "trello.com" || host.endsWith(".trello.com"))
            return ShouldDispatchSimulatedMouseEvents::Yes;
        if (host == "airtable.com" || host.endsWith(".airtable.com"))
            return ShouldDispatchSimulatedMouseEvents::Yes;
        if (host == "msn.com" || host.endsWith(".msn.com"))
            return ShouldDispatchSimulatedMouseEvents::Yes;
        if (host == "flipkart.com" || host.endsWith(".flipkart.com"))
            return ShouldDispatchSimulatedMouseEvents::Yes;
        if (host == "iqiyi.com" || host.endsWith(".iqiyi.com"))
            return ShouldDispatchSimulatedMouseEvents::Yes;
        if (host == "trailers.apple.com")
            return ShouldDispatchSimulatedMouseEvents::Yes;
        if (host == "soundcloud.com")
            return ShouldDispatchSimulatedMouseEvents::Yes;
        if (host == "naver.com")
            return ShouldDispatchSimulatedMouseEvents::Yes;
        if (host == "nba.com" || host.endsWith(".nba.com"))
            return ShouldDispatchSimulatedMouseEvents::Yes;
        if (host.endsWith(".naver.com")) {
            // Disable the quirk for tv.naver.com subdomain to be able to simulate hover on videos.
            if (host == "tv.naver.com")
                return ShouldDispatchSimulatedMouseEvents::No;
            // Disable the quirk for mail.naver.com subdomain to be able to tap on mail subjects.
            if (host == "mail.naver.com")
                return ShouldDispatchSimulatedMouseEvents::No;
            // Disable the quirk on the mobile site.
            // FIXME: Maybe this quirk should be disabled for "m." subdomains on all sites? These are generally mobile sites that don't need mouse events.
            if (host == "m.naver.com")
                return ShouldDispatchSimulatedMouseEvents::No;
            return ShouldDispatchSimulatedMouseEvents::Yes;
        }
        if (host == "mybinder.org" || host.endsWith(".mybinder.org"))
            return ShouldDispatchSimulatedMouseEvents::DependingOnTargetFor_mybinder_org;
        return ShouldDispatchSimulatedMouseEvents::No;
    };

    if (m_shouldDispatchSimulatedMouseEventsQuirk == ShouldDispatchSimulatedMouseEvents::Unknown)
        m_shouldDispatchSimulatedMouseEventsQuirk = doShouldDispatchChecks();

    switch (m_shouldDispatchSimulatedMouseEventsQuirk) {
    case ShouldDispatchSimulatedMouseEvents::Unknown:
        ASSERT_NOT_REACHED();
        return false;

    case ShouldDispatchSimulatedMouseEvents::No:
        return false;

    case ShouldDispatchSimulatedMouseEvents::DependingOnTargetFor_mybinder_org:
        if (is<Node>(target)) {
            for (auto* node = downcast<Node>(target); node; node = node->parentNode()) {
                if (is<Element>(node) && downcast<Element>(*node).classList().contains("lm-DockPanel-tabBar"))
                    return true;
            }
        }
        return false;

    case ShouldDispatchSimulatedMouseEvents::Yes:
        return true;
    }

    ASSERT_NOT_REACHED();
    return false;
}

bool Quirks::shouldDispatchedSimulatedMouseEventsAssumeDefaultPrevented(EventTarget* target) const
{
    if (!needsQuirks() || !shouldDispatchSimulatedMouseEvents(target))
        return false;

    if (isAmazon() && is<Element>(target)) {
        // When panning on an Amazon product image, we're either touching on the #magnifierLens element
        // or its previous sibling.
        auto& element = downcast<Element>(*target);
        if (element.getIdAttribute() == "magnifierLens")
            return true;
        if (auto* sibling = element.nextElementSibling())
            return sibling->getIdAttribute() == "magnifierLens";
    }

    if (equalLettersIgnoringASCIICase(m_document->topDocument().url().host(), "soundcloud.com") && is<Element>(target))
        return downcast<Element>(*target).classList().contains("sceneLayer");

    return false;
}

std::optional<Event::IsCancelable> Quirks::simulatedMouseEventTypeForTarget(EventTarget* target) const
{
    if (!shouldDispatchSimulatedMouseEvents(target))
        return { };

    // On Google Maps, we want to limit simulated mouse events to dragging the little man that allows entering into Street View.
    if (isGoogleMaps()) {
        if (is<Element>(target) && downcast<Element>(target)->getAttribute("class") == "widget-expand-button-pegman-icon")
            return Event::IsCancelable::Yes;
        return { };
    }

    auto host = m_document->topDocument().url().host();
    if (equalLettersIgnoringASCIICase(host, "desmos.com") || host.endsWithIgnoringASCIICase(".desmos.com"))
        return Event::IsCancelable::No;

    if (equalLettersIgnoringASCIICase(host, "airtable.com") || host.endsWithIgnoringASCIICase(".airtable.com")) {
        // We want to limit simulated mouse events to elements under <div id="paneContainer"> to allow for column re-ordering and multiple cell selection.
        if (is<Node>(target)) {
            auto* node = downcast<Node>(target);
            if (auto* paneContainer = node->treeScope().getElementById(AtomString("paneContainer"))) {
                if (paneContainer->contains(node))
                    return Event::IsCancelable::Yes;
            }
        }
        return { };
    }

    return Event::IsCancelable::Yes;
}

bool Quirks::shouldMakeTouchEventNonCancelableForTarget(EventTarget* target) const
{
    if (!needsQuirks())
        return false;

    auto host = m_document->topDocument().url().host();

    if (equalLettersIgnoringASCIICase(host, "www.youtube.com")) {
        if (is<Element>(target)) {
            unsigned depth = 3;
            for (auto* element = downcast<Element>(target); element && depth; element = element->parentElement(), --depth) {
                if (element->localName() == "paper-item" && element->classList().contains("yt-dropdown-menu"))
                    return true;
            }
        }
    }

    return false;
}

bool Quirks::shouldPreventPointerMediaQueryFromEvaluatingToCoarse() const
{
    if (!needsQuirks())
        return false;

    auto host = m_document->topDocument().url().host();
    return equalLettersIgnoringASCIICase(host, "shutterstock.com") || host.endsWithIgnoringASCIICase(".shutterstock.com");
}

bool Quirks::shouldPreventDispatchOfTouchEvent(const AtomString& touchEventType, EventTarget* target) const
{
    if (!needsQuirks())
        return false;

    if (is<Element>(target) && touchEventType == eventNames().touchendEvent && equalLettersIgnoringASCIICase(m_document->topDocument().url().host(), "sites.google.com")) {
        auto& classList = downcast<Element>(*target).classList();
        return classList.contains("DPvwYc") && classList.contains("sm8sCf");
    }

    return false;
}

#endif

#if ENABLE(IOS_TOUCH_EVENTS)
bool Quirks::shouldSynthesizeTouchEvents() const
{
    if (!needsQuirks())
        return false;

    if (!m_shouldSynthesizeTouchEventsQuirk)
        m_shouldSynthesizeTouchEventsQuirk = isYahooMail(*m_document);
    return m_shouldSynthesizeTouchEventsQuirk.value();
}
#endif

bool Quirks::shouldAvoidResizingWhenInputViewBoundsChange() const
{
    if (!needsQuirks())
        return false;

    auto& url = m_document->topDocument().url();
    auto host = url.host();

    if (equalLettersIgnoringASCIICase(host, "live.com") || host.endsWithIgnoringASCIICase(".live.com"))
        return true;

    if (equalLettersIgnoringASCIICase(host, "twitter.com") || host.endsWithIgnoringASCIICase(".twitter.com"))
        return true;

    if ((equalLettersIgnoringASCIICase(host, "google.com") || host.endsWithIgnoringASCIICase(".google.com")) && url.path().startsWithIgnoringASCIICase("/maps/"))
        return true;

    if (host.endsWithIgnoringASCIICase(".sharepoint.com"))
        return true;

    return false;
}

bool Quirks::shouldDisablePointerEventsQuirk() const
{
#if PLATFORM(IOS_FAMILY)
    if (!needsQuirks())
        return false;

    auto& url = m_document->topDocument().url();
    auto host = url.host();
    if (equalLettersIgnoringASCIICase(host, "mailchimp.com") || host.endsWithIgnoringASCIICase(".mailchimp.com"))
        return true;
#endif
    return false;
}

bool Quirks::needsDeferKeyDownAndKeyPressTimersUntilNextEditingCommand() const
{
#if PLATFORM(IOS_FAMILY)
    if (m_document->settings().needsDeferKeyDownAndKeyPressTimersUntilNextEditingCommandQuirk())
        return true;

    if (!needsQuirks())
        return false;

    auto& url = m_document->topDocument().url();
    return equalLettersIgnoringASCIICase(url.host(), "docs.google.com") && url.path().startsWithIgnoringASCIICase("/spreadsheets/");
#else
    return false;
#endif
}

// FIXME(<rdar://problem/50394969>): Remove after desmos.com adopts inputmode="none".
bool Quirks::needsInputModeNoneImplicitly(const HTMLElement& element) const
{
#if PLATFORM(IOS_FAMILY)
    if (!needsQuirks())
        return false;

    if (element.hasTagName(HTMLNames::inputTag)) {
        if (!equalLettersIgnoringASCIICase(m_document->url().host(), "calendar.google.com"))
            return false;
        static NeverDestroyed<QualifiedName> dataInitialValueAttr(nullAtom(), "data-initial-value", nullAtom());
        static NeverDestroyed<QualifiedName> dataPreviousValueAttr(nullAtom(), "data-previous-value", nullAtom());

        return equalLettersIgnoringASCIICase(element.attributeWithoutSynchronization(HTMLNames::autocompleteAttr), "off")
            && element.hasAttributeWithoutSynchronization(dataInitialValueAttr)
            && element.hasAttributeWithoutSynchronization(dataPreviousValueAttr);
    }

    if (!element.hasTagName(HTMLNames::textareaTag))
        return false;

    auto& url = m_document->url();
    auto host = url.host();
    if (!host.endsWithIgnoringASCIICase(".desmos.com"))
        return false;

    return element.parentElement() && element.parentElement()->classNames().contains("dcg-mq-textarea");
#else
    UNUSED_PARAM(element);
    return false;
#endif
}

// FIXME: Remove after the site is fixed, <rdar://problem/50374200>
bool Quirks::needsGMailOverflowScrollQuirk() const
{
#if PLATFORM(IOS_FAMILY)
    if (!needsQuirks())
        return false;

    if (!m_needsGMailOverflowScrollQuirk)
        m_needsGMailOverflowScrollQuirk = equalLettersIgnoringASCIICase(m_document->url().host(), "mail.google.com");

    return *m_needsGMailOverflowScrollQuirk;
#else
    return false;
#endif
}

// FIXME: Remove after the site is fixed, <rdar://problem/50374311>
bool Quirks::needsYouTubeOverflowScrollQuirk() const
{
#if PLATFORM(IOS_FAMILY)
    if (!needsQuirks())
        return false;

    if (!m_needsYouTubeOverflowScrollQuirk)
        m_needsYouTubeOverflowScrollQuirk = equalLettersIgnoringASCIICase(m_document->url().host(), "www.youtube.com");

    return *m_needsYouTubeOverflowScrollQuirk;
#else
    return false;
#endif
}

// FIXME: Remove after the site is fixed, <rdar://problem/74377902>
bool Quirks::needsWeChatScrollingQuirk() const
{
#if PLATFORM(IOS)
    return needsQuirks() && !linkedOnOrAfter(SDKVersion::FirstWithoutWeChatScrollingQuirk) && IOSApplication::isWechat();
#else
    return false;
#endif
}

bool Quirks::shouldOmitHTMLDocumentSupportedPropertyNames()
{
#if PLATFORM(COCOA)
    static bool shouldOmitHTMLDocumentSupportedPropertyNames = !linkedOnOrAfter(SDKVersion::FirstWithHTMLDocumentSupportedPropertyNames);
    return shouldOmitHTMLDocumentSupportedPropertyNames;
#else
    return false;
#endif
}

bool Quirks::shouldSilenceWindowResizeEvents() const
{
#if PLATFORM(IOS)
    if (!needsQuirks())
        return false;

    // We silence window resize events during the 'homing out' snapshot sequence when on nytimes.com
    // to address <rdar://problem/59763843>, and on twitter.com to address <rdar://problem/58804852> &
    // <rdar://problem/61731801>.
    auto* page = m_document->page();
    if (!page || !page->isTakingSnapshotsForApplicationSuspension())
        return false;

    auto host = m_document->topDocument().url().host();
    return equalLettersIgnoringASCIICase(host, "nytimes.com") || host.endsWithIgnoringASCIICase(".nytimes.com")
        || equalLettersIgnoringASCIICase(host, "twitter.com") || host.endsWithIgnoringASCIICase(".twitter.com");
#else
    return false;
#endif
}

bool Quirks::shouldSilenceMediaQueryListChangeEvents() const
{
#if PLATFORM(IOS)
    if (!needsQuirks())
        return false;

    // We silence MediaQueryList's change events during the 'homing out' snapshot sequence when on twitter.com
    // to address <rdar://problem/58804852> & <rdar://problem/61731801>.
    auto* page = m_document->page();
    if (!page || !page->isTakingSnapshotsForApplicationSuspension())
        return false;

    auto host = m_document->topDocument().url().host();
    return equalLettersIgnoringASCIICase(host, "twitter.com") || host.endsWithIgnoringASCIICase(".twitter.com");
#else
    return false;
#endif
}

bool Quirks::shouldAvoidScrollingWhenFocusedContentIsVisible() const
{
    if (!needsQuirks())
        return false;

    return equalLettersIgnoringASCIICase(m_document->url().host(), "www.zillow.com");
}

bool Quirks::shouldUseLegacySelectPopoverDismissalBehaviorInDataActivation() const
{
    if (!needsQuirks())
        return false;

    auto host = m_document->url().host();
    return equalLettersIgnoringASCIICase(host, "att.com") || host.endsWithIgnoringASCIICase(".att.com");
}

bool Quirks::shouldIgnoreAriaForFastPathContentObservationCheck() const
{
#if PLATFORM(IOS_FAMILY)
    if (!needsQuirks())
        return false;

    auto host = m_document->url().host();
    return equalLettersIgnoringASCIICase(host, "www.ralphlauren.com");
#endif
    return false;
}

bool Quirks::shouldOpenAsAboutBlank(const String& stringToOpen) const
{
#if PLATFORM(IOS_FAMILY)
    if (!needsQuirks())
        return false;

    auto openerURL = m_document->url();
    if (!equalLettersIgnoringASCIICase(openerURL.host(), "docs.google.com"))
        return false;

    if (!m_document->frame() || !m_document->frame()->loader().userAgent(openerURL).contains("Macintosh"))
        return false;

    URL urlToOpen { URL { }, stringToOpen };
    if (!urlToOpen.protocolIsAbout())
        return false;

    return !equalLettersIgnoringASCIICase(urlToOpen.host(), "blank") && !equalLettersIgnoringASCIICase(urlToOpen.host(), "srcdoc");
#else
    UNUSED_PARAM(stringToOpen);
    return false;
#endif
}

bool Quirks::needsPreloadAutoQuirk() const
{
#if PLATFORM(IOS_FAMILY)
    if (!needsQuirks())
        return false;

    if (m_needsPreloadAutoQuirk)
        return m_needsPreloadAutoQuirk.value();

    auto domain = m_document->securityOrigin().domain().convertToASCIILowercase();

    m_needsPreloadAutoQuirk = domain == "vimeo.com" || domain.endsWith("vimeo.com");

    return m_needsPreloadAutoQuirk.value();
#else
    return false;
#endif
}

bool Quirks::shouldBypassBackForwardCache() const
{
    if (!needsQuirks())
        return false;

    auto topURL = m_document->topDocument().url();
    auto host = topURL.host();

    // Vimeo.com used to bypass the back/forward cache by serving "Cache-Control: no-store" over HTTPS.
    // We started caching such content in r250437 but the vimeo.com content unfortunately is not currently compatible
    // because it changes the opacity of its body to 0 when navigating away and fails to restore the original opacity
    // when coming back from the back/forward cache (e.g. in 'pageshow' event handler). See <rdar://problem/56996057>.
    if (topURL.protocolIs("https") && equalLettersIgnoringASCIICase(host, "vimeo.com")) {
        if (auto* documentLoader = m_document->frame() ? m_document->frame()->loader().documentLoader() : nullptr)
            return documentLoader->response().cacheControlContainsNoStore();
    }

    // Google Docs used to bypass the back/forward cache by serving "Cache-Control: no-store" over HTTPS.
    // We started caching such content in r250437 but the Google Docs index page unfortunately is not currently compatible
    // because it puts an overlay (with class "docs-homescreen-freeze-el-full") over the page when navigating away and fails
    // to remove it when coming back from the back/forward cache (e.g. in 'pageshow' event handler). See <rdar://problem/57670064>.
    // Note that this does not check for docs.google.com host because of hosted G Suite apps.
    static MainThreadNeverDestroyed<const AtomString> googleDocsOverlayDivClass("docs-homescreen-freeze-el-full", AtomString::ConstructFromLiteral);
    auto* firstChildInBody = m_document->body() ? m_document->body()->firstChild() : nullptr;
    if (is<HTMLDivElement>(firstChildInBody)) {
        auto& div = downcast<HTMLDivElement>(*firstChildInBody);
        if (div.hasClass() && div.classNames().contains(googleDocsOverlayDivClass))
            return true;
    }

    return false;
}

bool Quirks::shouldBypassAsyncScriptDeferring() const
{
    if (!needsQuirks())
        return false;

    if (!m_shouldBypassAsyncScriptDeferring) {
        auto domain = RegistrableDomain { m_document->topDocument().url() };
        // Deferring 'mapbox-gl.js' script on bungalow.com causes the script to get in a bad state (rdar://problem/61658940).
        m_shouldBypassAsyncScriptDeferring = (domain == "bungalow.com");
    }
    return *m_shouldBypassAsyncScriptDeferring;
}

bool Quirks::shouldMakeEventListenerPassive(const EventTarget& eventTarget, const AtomString& eventType, const EventListener& eventListener)
{
    auto eventTargetIsRoot = [](const EventTarget& eventTarget) {
        if (is<DOMWindow>(eventTarget))
            return true;

        if (is<Node>(eventTarget)) {
            auto& node = downcast<Node>(eventTarget);
            return is<Document>(node) || node.document().documentElement() == &node || node.document().body() == &node;
        }
        return false;
    };

    auto documentFromEventTarget = [](const EventTarget& eventTarget) -> Document* {
        return downcast<Document>(eventTarget.scriptExecutionContext());
    };

    if (eventNames().isTouchScrollBlockingEventType(eventType)) {
        if (eventTargetIsRoot(eventTarget)) {
            if (auto* document = documentFromEventTarget(eventTarget))
                return document->settings().passiveTouchListenersAsDefaultOnDocument();
        }
        return false;
    }

    if (eventNames().isWheelEventType(eventType)) {
        if (eventTargetIsRoot(eventTarget)) {
            if (auto* document = documentFromEventTarget(eventTarget))
                return document->settings().passiveWheelListenersAsDefaultOnDocument();
        }
        return false;
    }

    if (eventType == eventNames().mousewheelEvent) {
        if (!is<JSEventListener>(eventListener))
            return false;

        // For SmoothScroll.js
        // Matches Blink intervention in https://chromium.googlesource.com/chromium/src/+/b6b13c9cfe64d52a4168d9d8d1ad9bb8f0b46a2a%5E%21/
        if (is<DOMWindow>(eventTarget)) {
            auto* document = downcast<DOMWindow>(eventTarget).document();
            if (!document || !document->quirks().needsQuirks())
                return false;

            auto& jsEventListener = downcast<JSEventListener>(eventListener);
            if (jsEventListener.functionName() == "ssc_wheel")
                return true;
        }

        return false;
    }

    return false;
}

#if ENABLE(MEDIA_STREAM)
bool Quirks::shouldEnableLegacyGetUserMediaQuirk() const
{
    if (!needsQuirks())
        return false;

    if (!m_shouldEnableLegacyGetUserMediaQuirk) {
        auto host = m_document->securityOrigin().host();
        m_shouldEnableLegacyGetUserMediaQuirk = host == "www.baidu.com" || host == "www.warbyparker.com";
    }
    return m_shouldEnableLegacyGetUserMediaQuirk.value();
}
#endif

bool Quirks::needsCanPlayAfterSeekedQuirk() const
{
    if (!needsQuirks())
        return false;

    if (m_needsCanPlayAfterSeekedQuirk)
        return *m_needsCanPlayAfterSeekedQuirk;

    auto domain = m_document->securityOrigin().domain().convertToASCIILowercase();

    m_needsCanPlayAfterSeekedQuirk = domain == "hulu.com" || domain.endsWith(".hulu.com");

    return m_needsCanPlayAfterSeekedQuirk.value();
}

bool Quirks::shouldLayOutAtMinimumWindowWidthWhenIgnoringScalingConstraints() const
{
    if (!needsQuirks())
        return false;

    // FIXME: We should consider replacing this with a heuristic to determine whether
    // or not the edges of the page mostly lack content after shrinking to fit.
    return m_document->url().host().endsWithIgnoringASCIICase(".wikipedia.org");
}

bool Quirks::shouldIgnoreContentObservationForSyntheticClick(bool isFirstSyntheticClickOnPage) const
{
    if (!needsQuirks())
        return false;

    auto host = m_document->url().host();
    return isFirstSyntheticClickOnPage && (equalLettersIgnoringASCIICase(host, "shutterstock.com") || host.endsWithIgnoringASCIICase(".shutterstock.com"));
}

bool Quirks::shouldAvoidPastingImagesAsWebContent() const
{
    if (!needsQuirks())
        return false;

#if PLATFORM(IOS_FAMILY)
    if (!m_shouldAvoidPastingImagesAsWebContent)
        m_shouldAvoidPastingImagesAsWebContent = isYahooMail(*m_document);
    return *m_shouldAvoidPastingImagesAsWebContent;
#else
    return false;
#endif
}

bool Quirks::needsVP9FullRangeFlagQuirk() const
{
    if (!needsQuirks())
        return false;

    if (!m_needsVP9FullRangeFlagQuirk)
        m_needsVP9FullRangeFlagQuirk = equalLettersIgnoringASCIICase(m_document->url().host(), "www.youtube.com");

    return *m_needsVP9FullRangeFlagQuirk;
}

bool Quirks::needsHDRPixelDepthQuirk() const
{
    if (!needsQuirks())
        return false;

    if (!m_needsHDRPixelDepthQuirk)
        m_needsHDRPixelDepthQuirk = equalLettersIgnoringASCIICase(m_document->url().host(), "www.youtube.com");

    return *m_needsHDRPixelDepthQuirk;
}

#if ENABLE(WEB_AUTHN)
bool Quirks::shouldBypassUserGestureRequirementForWebAuthn() const
{
    if (!needsQuirks())
        return false;

    auto host = m_document->topDocument().url().host();
    if (equalLettersIgnoringASCIICase(host, "dropbox.com") || host.endsWithIgnoringASCIICase(".dropbox.com"))
        return true;

    if (equalLettersIgnoringASCIICase(host, "microsoft.com") || host.endsWithIgnoringASCIICase(".microsoft.com"))
        return true;

    if (equalLettersIgnoringASCIICase(host, "google.com") || host.endsWithIgnoringASCIICase(".google.com"))
        return true;

    if (equalLettersIgnoringASCIICase(host, "twitter.com") || host.endsWithIgnoringASCIICase(".twitter.com"))
        return true;

    if (equalLettersIgnoringASCIICase(host, "facebook.com") || host.endsWithIgnoringASCIICase(".facebook.com"))
        return true;

    return false;
}
#endif

#if ENABLE(IMAGE_ANALYSIS)

bool Quirks::needsToForceUserSelectWhenInstallingImageOverlay() const
{
    if (!needsQuirks())
        return false;

    auto& url = m_document->topDocument().url();
    if (topPrivatelyControlledDomain(url.host().toString()).startsWith("google.") && url.path() == "/search")
        return true;

    return false;
}

#endif // ENABLE(IMAGE_ANALYSIS)

}
