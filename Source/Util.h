/*
  ==============================================================================

    Util.h
    Created: 3 Jan 2025 2:15:06pm
    Author:  Eugene Clewlow

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>


//==============================================================================
void showBubbleMessage (juce::Component& targetComponent, const juce::String& textToShow,
                        std::unique_ptr<juce::BubbleMessageComponent>& bmc,
                        bool isRunningComponentTransformDemo)
{
    bmc.reset (new juce::BubbleMessageComponent());
    
    if (isRunningComponentTransformDemo)
    {
        //        targetComponent.findParentComponentOfClass<WidgetsDemo>()->addChildComponent (bmc.get());
    }
    else if (juce::Desktop::canUseSemiTransparentWindows())
    {
        bmc->setAlwaysOnTop (true);
        bmc->addToDesktop (0);
    }
    else
    {
        targetComponent.getTopLevelComponent()->addChildComponent (bmc.get());
    }
    
    juce::AttributedString text (textToShow);
    text.setJustification (juce::Justification::centred);
    text.setColour (targetComponent.findColour (juce::TextButton::textColourOffId));
    
    bmc->showAt (&targetComponent, text, 2000, true, false);
}


