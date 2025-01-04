/*
  ==============================================================================

    TabbedUI.h
    Created: 3 Jan 2025 2:14:23pm
    Author:  Eugene Clewlow

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "TableListBoxTutorial.h"
#include "FrontPanelUI.h"
#include "SysexValidator.h"
#include "Util.h"


class DemoTabbedComponent : public juce::TabbedComponent
{
    public:
    DemoTabbedComponent (): TabbedComponent (juce::TabbedButtonBar::TabsAtTop)
    {
        auto colour = findColour (juce::ResizableWindow::backgroundColourId);
        
        addTab ("Panel",     colour, new FrontPanelUI(), true);
        addTab ("PresetManager",     colour, new TableTutorialComponent(), true);
        addTab ("SysexValidator",    colour, new SysexValidator(), true);
        addTab ("Misc",        colour, new TableTutorialComponent(), true);
        addTab ("Menus",       colour, new TableTutorialComponent(), true);
        addTab ("Tables",      colour, new TableTutorialComponent(), true);
        addTab ("Drag & Drop", colour, new TableTutorialComponent(), true);
        
        getTabbedButtonBar().getTabButton(5)->setExtraComponent(new CustomTabButton(),juce::TabBarButton::afterText);
        
        setCurrentTabIndex(2);
    }
    
    // This is a small star button that is put inside one of the tabs. You can
    // use this technique to create things like "close tab" buttons, etc.
    class CustomTabButton final : public Component
    {
        public:
        CustomTabButton()
        {
            setSize (20, 20);
        }
        
        void paint (juce::Graphics& g) override
        {
            juce::Path star;
            star.addStar ({}, 7, 1.0f, 2.0f);
            
            g.setColour (juce::Colours::green);
            g.fillPath (star, star.getTransformToScaleToFit (getLocalBounds().reduced (2).toFloat(), true));
        }
        
        void mouseDown (const juce::MouseEvent&) override
        {
            showBubbleMessage (*this,
                               "This is a custom tab component\n"
                               "\n"
                               "You can use these to implement things like close-buttons "
                               "or status displays for your tabs.",
                               bubbleMessage,
                               false);
        }
        private:
        bool runningComponentTransformsDemo;
        std::unique_ptr<juce::BubbleMessageComponent> bubbleMessage;
    };
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DemoTabbedComponent)
};


