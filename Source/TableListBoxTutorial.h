/*
 ==============================================================================
 
 This file is part of the JUCE tutorials.
 Copyright (c) 2020 - Raw Material Software Limited
 
 The code included in this file is provided under the terms of the ISC license
 http://www.isc.org/downloads/software-support-policy/isc-license. Permission
 To use, copy, modify, and/or distribute this software for any purpose with or
 without fee is hereby granted provided that the above copyright notice and
 this permission notice appear in all copies.
 
 THE SOFTWARE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL WARRANTIES,
 WHETHER EXPRESSED OR IMPLIED, INCLUDING MERCHANTABILITY AND FITNESS FOR
 PURPOSE, ARE DISCLAIMED.
 
 ==============================================================================
 */

/*******************************************************************************
 The block below describes the properties of this PIP. A PIP is a short snippet
 of code that can be read by the Projucer and used to generate a JUCE project.
 
 BEGIN_JUCE_PIP_METADATA
 
 name:             TableListBoxTutorial
 version:          1.0.0
 vendor:           JUCE
 website:          http://juce.com
 description:      Explores table list boxes.
 
 dependencies:     juce_core, juce_data_structures, juce_events, juce_graphics,
 juce_gui_basics
 exporters:        xcode_mac, vs2019, linux_make
 
 type:             Component
 mainClass:        MainComponent
 
 useLocalCopy:     1
 
 END_JUCE_PIP_METADATA
 
 *******************************************************************************/


#pragma once

//==============================================================================
class TableTutorialComponent    : public juce::Component,
public juce::TableListBoxModel, public juce::DragAndDropContainer, public juce::DragAndDropTarget, public juce::FileDragAndDropTarget
{
    public:
    
    
    //    Public Member Functions
    //    virtual     ~DragAndDropTarget ()=default
    //         Destructor.
    //
    bool isInterestedInDragSource (const SourceDetails &dragSourceDetails) {
        //         Callback to check whether this target is interested in the type of object being dragged.
        //
        return true;
    }
    void itemDragEnter (const SourceDetails &dragSourceDetails) {
        //         Callback to indicate that something is being dragged over this component.
        //
    }
    void itemDragMove (const SourceDetails &dragSourceDetails) {
        //         Callback to indicate that the user is dragging something over this component.
        //
    }
    void itemDragExit (const SourceDetails &dragSourceDetails) {
        //         Callback to indicate that something has been dragged off the edge of this component.
        //
    }
    void itemDropped (const SourceDetails &dragSourceDetails) {
        //         Callback to indicate that the user has dropped something onto this component.
        //
        printf("item dropped\n");
        //        dragSourceDetails.sourceComponent
        table.updateContent();
        //        table.
        //        DBG(dragSourceDetails.sourceComponent.toString());
        
        DBG(dragSourceDetails.description.toString());
    }
    bool shouldDrawDragImageWhenOver () {
        //         Overriding this allows the target to tell the drag container whether to draw the drag image while the cursor is over it.
        return true;
    }
    
    
    
    
    
    //    Public Member Functions
    //    virtual     ~FileDragAndDropTarget ()=default
    //         Destructor.
    //
    bool isInterestedInFileDrag (const juce::StringArray &files) override {
        //         Callback to check whether this target is interested in the set of files being offered.
        //
        return true;
    }
    void fileDragEnter (const juce::StringArray &files, int x, int y) override {
        //         Callback to indicate that some files are being dragged over this component.
        //
    }
    void fileDragMove (const juce::StringArray &files, int x, int y) override {
        //         Callback to indicate that the user is dragging some files over this component.
        //
    }
    void fileDragExit (const juce::StringArray &files) override {
        //         Callback to indicate that the mouse has moved away from this component.
        //
    }
    void filesDropped (const juce::StringArray &files, int x, int y) override {
        //         Callback to indicate that the user has dropped the files onto this component.
        printf("file dropped\n");
    }
    
    juce::var getDragSourceDescription (const juce::SparseSet<int>& currentlySelectedRows) override
    {
        printf("test\n");
        for(int i = 0; i < currentlySelectedRows.getNumRanges(); i++) {
            juce::Range<int> r = currentlySelectedRows.getRange(i);
            DBG(r.getStart() << " + " << r.getEnd());
        }
        //        DBG(currentlySelectedRows);
        return {"whatever"};
    }
    TableTutorialComponent()
    {
        bool loadWithFileChooser = false;
        if(loadWithFileChooser) {
            const auto callback = [this] (const juce::FileChooser& chooser)
            {
                loadData (chooser.getResult());                                             // [1]
                
                addAndMakeVisible (table);                                                  // [1]
                
                table.setColour (juce::ListBox::outlineColourId, juce::Colours::grey);      // [2]
                table.setOutlineThickness (1);
                
                if (columnList != nullptr)
                {
                    for (auto* columnXml : columnList->getChildIterator())
                    {
                        table.getHeader().addColumn (columnXml->getStringAttribute ("name"), // [2]
                                                     columnXml->getIntAttribute ("columnId"),
                                                     columnXml->getIntAttribute ("width"),
                                                     50,
                                                     400,
                                                     juce::TableHeaderComponent::defaultFlags);
                    }
                }
                
                table.getHeader().setSortColumnId (1, true);                                // [3]
                table.getHeader().setStretchToFitActive(true);
                
                table.setMultipleSelectionEnabled (true);                                   // [4]
                
                resized();
            };
            
            fileChooser.launchAsync (  juce::FileBrowserComponent::openMode
                                     | juce::FileBrowserComponent::canSelectFiles,
                                     callback);
        } else {
            //            auto xmlFile = juce::File(BinaryData::TableData_xml);
            //            BinaryData::
            //            loadData (xmlFile);                                             // [1]
            for(int i = 0; i < BinaryData::RefaceDX_syxSize; i++)
            {
                DBG(juce::String::toHexString((void*)&BinaryData::RefaceDX_syx[i], 1));
            }
            
            // [1]
            int sum = 0;
            //https://yamahamusicians.com/forum/viewtopic.php?t=6864
            for(int i = 7; i < 13 - 2; i++)
            {
                sum += BinaryData::RefaceDX_syx[i];
            }
            
            int checksum = (~sum + 1) & 0x7F;
            
            if(checksum == BinaryData::RefaceDX_syx[11]) {
                DBG("checksum calculated successfully!");
            } else {
                DBG("checksum calculation failed!");
            }
            
            juce::String wholeSyx = juce::String::toHexString((void*)BinaryData::RefaceDX_syx, 241);
            
            // 1 patch = 13 + 51 + 41 * 4 + 13 = 241
            DBG(wholeSyx);
            
            tutorialData = juce::XmlDocument::parse (BinaryData::TableData_xml);            // [3]
            
            /*
             
             there are two different types of sysex to verify
             1 is the edit buffer sysex, which only contains info for 1 complete sound
             2 is the bank/patch sysex, which contains anywhere from 1 to 32 sounds
             we need to detect and check the validity of both of these data types
             we need to 1:
             - check for header to determine sysex type, and then check footer to confirm sysex type
                - if type 1 is found, then the program should only read up to one patch
                - if type 2 is found, the program should check the entire file, it should check iteratively:
                    - for a header and matching footer
                    - for voice and 4 operator, that each parameter data type lies within data range.
                    - lastly, after all patches are validated, that no two patches have the same bank/patch number associated
             RULES:
             - file must be a multiple of 241. Not including 0
             - every 241 bytes, starting at by 241*n+0, a header must be found that matches the pattern:
                            F0 43 00 7F 1C 00 04 05 0E 0F 00 5E F7
                            F0 43 00 7F 1C 00 04 05 0E 00 00 6D F7
                    verify the first 7 bytes equals above.  yes byte count for the header should always be 00 04
                    verify that byte index 7 is 05 - always
                    verify that byte index 8 is 0E - always
                    read bytes 9 and 10 (9<<8 | 10).
                        if n == 3840 or n[9]==0Fh && n[10] == 00h,
                            then the patch is an edit buffer (question: should we allow multiple edit buffer patches in single sysex?
                        elif n < 32 && n >= 0
                            then the patch is a bank/prog
                    verify the checksum at byte index 11
                    verify byte index 12 is F7 - always
                    n++
                    
                    
             
             0 - F0H - exclusive status
             1 - 43H - YAHAMA ID
             2 - 00H - Device Number
             3 - 7FH - Group number HIGH
             
             4 - 1CH - Group number LOW
             5 - xxH - BYTE COUNT HIGH
             6 - xxH - BYTE COUNT LOW
             7 - 05H - Model Number
             
             8 - xxH - Address HIGH
             9 - xxH - Address MID
             10 -xxH - address LOW
             
             11 -xxH - checksum
             12 -F7H - end of exclusive
             
             */
            // bulk header for edit buffer is  is F0 43 00 7F 1C 00 04 05 0E 0F 00 5E F7
            // byte count is 04
            // BinaryData::RefaceDX_syx[5] == 0 && BinaryData::RefaceDX_syx[6] == 4
            // total first 13 bytes is "F043007F1C0004050E0F005EF7"
            // String.indexOfChar("F043007F1C0004050E0F005EF7") == 0
            // BinaryData::RefaceDX_syxSize == 241
            // String.indexOfChar("F043007F1C0004050E0F005EF7") == 0
            // String.indexOfChar("F043007F1C0004050F0F005DF7") == 0
            
            
            // bulk header for bank 0 patch 0 is  F0 43 00 7F 1C 00 04 05 0E 00 00 6D F7
            // byte count is 04
            
            //            Byte Count shows the size of data in blocks from Model ID onward (up to but not including the checksum).
            //            The Check sum is the value that results in a value of 0 for the lower 7 bits when the Model ID, Start Address,
            //            Data and Check sum itself are added.
            
            
            
            
            dataList   = tutorialData->getChildByName ("DATA");
            columnList = tutorialData->getChildByName ("HEADERS");          // [4]
            
            numRows = dataList->getNumChildElements();                      // [5]
            
            addAndMakeVisible (table);                                                  // [1]
            
            table.setColour (juce::ListBox::outlineColourId, juce::Colours::grey);      // [2]
            table.setOutlineThickness (1);
            
            if (columnList != nullptr)
            {
                for (auto* columnXml : columnList->getChildIterator())
                {
                    table.getHeader().addColumn (columnXml->getStringAttribute ("name"), // [2]
                                                 columnXml->getIntAttribute ("columnId"),
                                                 columnXml->getIntAttribute ("width"),
                                                 50,
                                                 400,
                                                 juce::TableHeaderComponent::defaultFlags);
                }
            }
            
            table.getHeader().setSortColumnId (1, true);                                // [3]
            table.getHeader().setStretchToFitActive(true);
            
            table.setMultipleSelectionEnabled (true);                                   // [4]
            
            resized();
        }
    }
    
    int getNumRows() override
    {
        return numRows;
    }
    
    void paintRowBackground (juce::Graphics& g, int rowNumber, int /*width*/, int /*height*/, bool rowIsSelected) override
    {
        auto alternateColour = getLookAndFeel().findColour (juce::ListBox::backgroundColourId)
            .interpolatedWith (getLookAndFeel().findColour (juce::ListBox::textColourId), 0.03f);
        if (rowIsSelected)
            g.fillAll (juce::Colours::lightblue);
        else if (rowNumber % 2)
            g.fillAll (alternateColour);
    }
    
    void paintCell (juce::Graphics& g, int rowNumber, int columnId,
                    int width, int height, bool rowIsSelected) override
    {
        g.setColour (rowIsSelected ? juce::Colours::darkblue : getLookAndFeel().findColour (juce::ListBox::textColourId));  // [5]
        g.setFont (font);
        
        if (auto* rowElement = dataList->getChildElement (rowNumber))
        {
            auto text = rowElement->getStringAttribute (getAttributeNameForColumnId (columnId));
            
            g.drawText (text, 2, 0, width - 4, height, juce::Justification::centredLeft, true);                             // [6]
        }
        
        g.setColour (getLookAndFeel().findColour (juce::ListBox::backgroundColourId));
        g.fillRect (width - 1, 0, 1, height);                                                                               // [7]
    }
    
    void sortOrderChanged (int newSortColumnId, bool isForwards) override
    {
        if (newSortColumnId != 0)
        {
            TutorialDataSorter sorter (getAttributeNameForColumnId (newSortColumnId), isForwards);
            dataList->sortChildElements (sorter);
            
            table.updateContent();
        }
    }
    
    Component* refreshComponentForCell (int rowNumber, int columnId, bool /*isRowSelected*/,
                                        Component* existingComponentToUpdate) override
    {
        if (columnId == 9)  // [8]
        {
            auto* selectionBox = static_cast<SelectionColumnCustomComponent*> (existingComponentToUpdate);
            
            if (selectionBox == nullptr)
                selectionBox = new SelectionColumnCustomComponent (*this);
            
            selectionBox->setRowAndColumn (rowNumber, columnId);
            return selectionBox;
        }
        
        if (columnId == 8)  // [9]
        {
            auto* textLabel = static_cast<EditableTextCustomComponent*> (existingComponentToUpdate);
            
            if (textLabel == nullptr)
                textLabel = new EditableTextCustomComponent (*this);
            
            textLabel->setRowAndColumn (rowNumber, columnId);
            return textLabel;
        }
        
        jassert (existingComponentToUpdate == nullptr);
        return nullptr;     // [10]
    }
    
    int getColumnAutoSizeWidth (int columnId) override
    {
        if (columnId == 9)
            return 50;
        
        int widest = 32;
        
        for (auto i = getNumRows(); --i >= 0;)
        {
            if (auto* rowElement = dataList->getChildElement (i))
            {
                auto text = rowElement->getStringAttribute (getAttributeNameForColumnId (columnId));
                
                widest = juce::jmax (widest, font.getStringWidth (text));
            }
        }
        
        return widest + 8;
    }
    
    int getSelection (const int rowNumber) const
    {
        return dataList->getChildElement (rowNumber)->getIntAttribute ("Select");
    }
    
    void setSelection (const int rowNumber, const int newSelection)
    {
        dataList->getChildElement (rowNumber)->setAttribute ("Select", newSelection);
    }
    
    juce::String getText (const int columnNumber, const int rowNumber) const
    {
        return dataList->getChildElement (rowNumber)->getStringAttribute (getAttributeNameForColumnId (columnNumber));
    }
    
    void setText (const int columnNumber, const int rowNumber, const juce::String& newText)
    {
        const auto& columnName = table.getHeader().getColumnName (columnNumber);
        dataList->getChildElement (rowNumber)->setAttribute (columnName, newText);
    }
    
    //==============================================================================
    void resized() override
    {
        table.setBoundsInset (juce::BorderSize<int> (8));
    }
    
    private:
    juce::TableListBox table  { {}, this };
    juce::Font font           { 14.0f };
    
    std::unique_ptr<juce::XmlElement> tutorialData;
    juce::XmlElement* columnList = nullptr;
    juce::XmlElement* dataList = nullptr;
    int numRows = 0;
    
    //==============================================================================
    class EditableTextCustomComponent  : public juce::Label
    {
        public:
        EditableTextCustomComponent (TableTutorialComponent& td)
        : owner (td)
        {
            setEditable (false, true, false);
        }
        
        void mouseDown (const juce::MouseEvent& event) override
        {
            owner.table.selectRowsBasedOnModifierKeys (row, event.mods, false);
            
            Label::mouseDown (event);
        }
        
        void textWasEdited() override
        {
            owner.setText (columnId, row, getText());
        }
        
        void setRowAndColumn (const int newRow, const int newColumn)
        {
            row = newRow;
            columnId = newColumn;
            setText (owner.getText(columnId, row), juce::dontSendNotification);
        }
        
        private:
        TableTutorialComponent& owner;
        int row, columnId;
        juce::Colour textColour;
    };
    
    //==============================================================================
    class SelectionColumnCustomComponent    : public Component
    {
        public:
        SelectionColumnCustomComponent (TableTutorialComponent& td)
        : owner (td)
        {
            addAndMakeVisible (toggleButton);
            
            toggleButton.onClick = [this] { owner.setSelection (row, (int) toggleButton.getToggleState()); };
        }
        
        void resized() override
        {
            toggleButton.setBoundsInset (juce::BorderSize<int> (2));
        }
        
        void setRowAndColumn (int newRow, int newColumn)
        {
            row = newRow;
            columnId = newColumn;
            toggleButton.setToggleState ((bool) owner.getSelection (row), juce::dontSendNotification);
        }
        
        private:
        TableTutorialComponent& owner;
        juce::ToggleButton toggleButton;
        int row, columnId;
    };
    
    //==============================================================================
    class TutorialDataSorter
    {
        public:
        TutorialDataSorter (const juce::String& attributeToSortBy, bool forwards)
        : attributeToSort (attributeToSortBy),
        direction (forwards ? 1 : -1)
        {}
        
        int compareElements (juce::XmlElement* first, juce::XmlElement* second) const
        {
            auto result = first->getStringAttribute (attributeToSort)
                .compareNatural (second->getStringAttribute (attributeToSort)); // [1]
            
            if (result == 0)
                result = first->getStringAttribute ("ID")
                .compareNatural (second->getStringAttribute ("ID"));             // [2]
            
            return direction * result;                                                          // [3]
        }
        
        private:
        juce::String attributeToSort;
        int direction;
    };
    
    //==============================================================================
    void loadData (juce::File tableFile)
    {
        if (tableFile == juce::File() || ! tableFile.exists())
            return;
        
        tutorialData = juce::XmlDocument::parse (tableFile);            // [3]
        
        dataList   = tutorialData->getChildByName ("DATA");
        columnList = tutorialData->getChildByName ("HEADERS");          // [4]
        
        numRows = dataList->getNumChildElements();                      // [5]
    }
    
    juce::String getAttributeNameForColumnId (const int columnId) const
    {
        for (auto* columnXml : columnList->getChildIterator())
        {
            if (columnXml->getIntAttribute ("columnId") == columnId)
                return columnXml->getStringAttribute ("name");
        }
        
        return {};
    }
    
    juce::FileChooser fileChooser { "Browse for TableData.xml",
        juce::File::getSpecialLocation (juce::File::invokedExecutableFile) };
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TableTutorialComponent)
};


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


class DemoTabbedComponent : public juce::TabbedComponent
{
    public:
    DemoTabbedComponent (): TabbedComponent (juce::TabbedButtonBar::TabsAtTop)
    {
        auto colour = findColour (juce::ResizableWindow::backgroundColourId);

        addTab ("Buttons",     colour, new TableTutorialComponent(), true);
        addTab ("Sliders",     colour, new TableTutorialComponent(), true);
        addTab ("Toolbars",    colour, new TableTutorialComponent(), true);
        addTab ("Misc",        colour, new TableTutorialComponent(), true);
        addTab ("Menus",       colour, new TableTutorialComponent(), true);
        addTab ("Tables",      colour, new TableTutorialComponent(), true);
        addTab ("Drag & Drop", colour, new TableTutorialComponent(), true);

        getTabbedButtonBar().getTabButton(5)->setExtraComponent(new CustomTabButton(),juce::TabBarButton::afterText);
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
    TableTutorialComponent table;
    DemoTabbedComponent tabbed;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
