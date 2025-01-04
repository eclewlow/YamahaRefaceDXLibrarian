#pragma once

#include <JuceHeader.h>

#include "TabbedUI.h"

//==============================================================================
class MainComponent   : public juce::Component
{
    public:
    //==============================================================================
    MainComponent()
    {
        //        addAndMakeVisible (table);
        addAndMakeVisible(tabbed);
        
        setSize (1200, 600);
    }
    
    void paint (juce::Graphics& g) override
    {
        g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    }
    
    void resized() override
    {
        //        table.setBounds (getLocalBounds());
        tabbed.setBounds (getLocalBounds());
    }
    
    private:
    //==============================================================================
    DemoTabbedComponent tabbed;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
