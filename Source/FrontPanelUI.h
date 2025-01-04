/*
  ==============================================================================

    YamahaRefaceDXUI.h
    Created: 3 Jan 2025 2:04:57pm
    Author:  Eugene Clewlow

  ==============================================================================
*/

#pragma once

//==============================================================================
class FrontPanelUI   : public juce::Component
{
    public:
    //==============================================================================
    FrontPanelUI()
    {
        //        addAndMakeVisible (table);
        //        addAndMakeVisible(tabbed);
        
        pitchLabel.setText("pitch", juce::NotificationType::dontSendNotification);
//        formantLabel.setInterceptsMouseClicks(false, false);
//        formantLabel.setJustificationType(juce::Justification::centred);
        formantLabel.setText("Formant", juce::NotificationType::dontSendNotification);
        xfadeLabel.setText("xfade", juce::NotificationType::dontSendNotification);
        
        pitchKnob.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);

        
        addAndMakeVisible (pitchKnob);
        addAndMakeVisible(pitchLabel);
        addAndMakeVisible (formantKnob);
        addAndMakeVisible(formantLabel);
        //        addAndMakeVisible (linkButton);
        addAndMakeVisible(xfadeSlider);
        addAndMakeVisible(xfadeLabel);
        addAndMakeVisible(mixSlider);
        addAndMakeVisible(attackSlider);
        addAndMakeVisible(releaseSlider);
        
        //        setSize (1200, 600);
        
    }
    
    void paint (juce::Graphics& g) override
    {
        g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    }
    
    void resized() override
    {
        juce::FlexBox fb;                                               // [1]
//        fb.flexWrap = juce::FlexBox::Wrap::wrap;                        // [2]
        fb.flexWrap = juce::FlexBox::Wrap::noWrap;
        fb.justifyContent = juce::FlexBox::JustifyContent::center;      // [3]
        fb.alignContent = juce::FlexBox::AlignContent::center;          // [4]
        
//        for (auto* b : buttons)                                         // [5]
//            fb.items.add (juce::FlexItem (*b).withMinWidth (50.0f).withMinHeight (50.0f));
        fb.items.add (juce::FlexItem (pitchKnob).withMinWidth (50.0f).withMinHeight (50.0f));
        fb.items.add (juce::FlexItem (formantKnob).withMinWidth (50.0f).withMinHeight (50.0f));
        fb.items.add (juce::FlexItem (pitchLabel).withMinWidth (50.0f).withMinHeight (50.0f));
        fb.items.add (juce::FlexItem (formantLabel).withMinWidth (50.0f).withMinHeight (50.0f));
        fb.items.add (juce::FlexItem (xfadeSlider).withMinWidth (50.0f).withMinHeight (50.0f));
        fb.items.add (juce::FlexItem (xfadeLabel).withMinWidth (50.0f).withMinHeight (50.0f));
        fb.items.add (juce::FlexItem (mixSlider).withMinWidth (50.0f).withMinHeight (50.0f));
        fb.items.add (juce::FlexItem (attackSlider).withMinWidth (50.0f).withMinHeight (50.0f));
        fb.items.add (juce::FlexItem (releaseSlider).withMinWidth (50.0f).withMinHeight (50.0f));

        fb.performLayout (getLocalBounds());                            // [6]
        
        
//        formantLabel.setBounds (getLocalBounds());
//        pitchKnob.setBounds (getLocalBounds());
//        formantKnob.setBounds (getLocalBounds());
//        pitchLabel.setBounds (getLocalBounds());
//        formantLabel.setBounds (getLocalBounds());
//        xfadeSlider.setBounds (getLocalBounds());
//        xfadeLabel.setBounds (getLocalBounds());
//        mixSlider.setBounds (getLocalBounds());
//        attackSlider.setBounds (getLocalBounds());
//        releaseSlider.setBounds (getLocalBounds());
        //        tabbed.setBounds (getLocalBounds());
    }
    
    private:
    juce::Slider pitchKnob;
    juce::Slider formantKnob;
    juce::Label pitchLabel;
    juce::Label formantLabel;
    juce::Slider xfadeSlider;
    juce::Label xfadeLabel;
    juce::Slider mixSlider;
    juce::Slider attackSlider;
    juce::Slider releaseSlider;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FrontPanelUI)
};

