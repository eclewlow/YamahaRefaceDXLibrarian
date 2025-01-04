/*
 ==============================================================================
 
 SysexValidator.h
 Created: 3 Jan 2025 2:12:29pm
 Author:  Eugene Clewlow
 
 ==============================================================================
 */

#pragma once

#include "Util.h"

//==============================================================================
class SysexValidator   : public juce::Component, public juce::FileDragAndDropTarget
{
    public:
    
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
        //        printf("file dropped\n");
        try {
            if(!files.isEmpty()) {
//                DBG(files[0]);
                for(int i = 0; i < files.size(); i++) {
                    if(!files[i].endsWith(".syx")) {
                        showError("A dropped file does not end in the .syx extension!");
                        throw "File does not end in .syx!";
                    }
                    
                    juce::File f(files[i]);

                    validate(f);
                }
            } else {
//                showError("No files were dropped!");
                throw "No files were dropped!";
            }
        } catch(const char* msg) {
            DBG(msg);
        }
    }
    
    //==============================================================================
    SysexValidator()
    {
        //        addAndMakeVisible (table);
        
        setSize (1200, 600);
    }
    
    // https://yamahamusicians.com/forum/viewtopic.php?t=6864
    bool validateChecksum(char* data, int length) {
        int sum = 0;
        for(int i = 7; i < length - 2; i++)
        {
            sum += data[i];
        }
        
        int checksum = (~sum + 1) & 0x7F;
        
        if(checksum != data[length - 2]) {
            showError("Checksum validation failed!");
            throw "Checksum validation failed!";
        } else {
            DBG("Check sum validation succeeded!");
        }
        return true;
    }
    
    bool validate(juce::File f) {

        if (! f.existsAsFile()) // [1]
        {
            showError("File does not exist!");
            throw "File does not exist!";
        }

        if (f.getSize() != 0 && f.getSize() % 241 != 0) // [1]
        {
            showError("File size is not a multiple of 241!\nYamaha Reface DX sysex files are multiples of 241 bytes.");
            throw "File size is not a multiple of 241!\nYamaha Reface DX sysex files are multiples of 241 bytes.";
        }
        
        juce::MemoryBlock * result = new juce::MemoryBlock();
        bool loaded = f.loadFileAsData(*result);

        if(!loaded) {
            showError("Error loading file as data.");
            throw "Error loading file as data.";
        }
        
        char *data = (char*)result->getData();

        for(int i = 0; i < result->getSize(); i += 241) {
//            DBG(juce::String::toHexString(&data[i], 13));
//            DBG(juce::String::toHexString(&data[i + 241 - 13], 13));

            // if an edit buffer header is found
            if(juce::String::toHexString(&data[i], 13).equalsIgnoreCase("f0 43 00 7f 1c 00 04 05 0e 0f 00 5e f7")) {
                
                validateChecksum(&data[i], 13);
                // assert the matching footer
                if(!juce::String::toHexString(&data[i + 241 - 13], 13).equalsIgnoreCase("f0 43 00 7f 1c 00 04 05 0f 0f 00 5d f7")) {
                    showError("Malformed Yamaha sysex. An edit buffer header was found, but no matching footer.");
                    throw "Malformed Yamaha sysex. An edit buffer header was found, but no matching footer.";
                }

                validateChecksum(&data[i + 241 - 13], 13);

                // assert voice data
                if(!juce::String::toHexString(&data[i + 13], 11).equalsIgnoreCase("F0 43 00 7F 1C 00 2A 05 30 00 00")) {
                    showError("Malformed Yamaha sysex. Edit buffer data was found, but the voice data was malformed.");
                    throw "Malformed Yamaha sysex. Edit buffer data was found, but the voice data was malformed.";
                }

                validateChecksum(&data[i + 13], 51);

                //
                DBG("successfully parsed edit buffer sysex!");
            }
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
            //            DBG(juce::String::toHexString(data[i]));
            
        }

        //        DBG(f.getFileName());
//        if (! f.existsAsFile())
//                    return;  // file doesn't exist
 
//        if (std::unique_ptr<juce::FileInputStream> inputStream { f.createInputStream() })
//        {
////                    textContent->clear();
//            DBG(inputStream->getTotalLength());
//
//            while (! inputStream->isExhausted())
//            {
////                auto nextWord = readUpToNextSpace (*inputStream);
////                textContent->setColour (juce::TextEditor::textColourId, getRandomColour (0.75f));
////                textContent->insertTextAtCaret (nextWord);
////                printf("\n");
//                char c = inputStream->readByte();
////                        printf("%02X\n",  (unsigned)(unsigned char)c);
////                DBG(c);
//            }
//        }

        
//        printf("%02x\n", f.loadFileAsString().toStdString().c_str());
//        DBG
//        DBG(f.loadFileAsString());

        //        printf((char*)result.getData());
//        for(int i = 0; result.getSize(); i++) {
//        DBG(result->getSize());
//            printf((char)result->getData()[0]);
//        printf("%02X\n", ((char*)result->getData())[1]);
//        }
//        DBG(juce::String::toHexString);
//        DBG(result.toBase64Encoding());
        
//        for(int i = 0; i < f.getSize(); i+= 241)
//        {
//            DBG(juce::String::toHexString(result.getData());
//        }
//        char *t = (char*)result.getData();
//        char data = (char)t[0];
        
//        juce::String s = f.loadFileAsString();
        
//                for(int i = 0; i < f.getSize(); i++)
//                {
//                    DBG(data[i]);
//                    char c = (char)*(data[i]);
//                    DBG(juce::String::toHexString;
//                }
//        DBG((char*)&data[0]);
//        DBG(juce::String::toHexString(t);
//        DBG(s);

//        DBG(juce::String::toHexString(result.getData()));
        return true;
    }
    
    void paint (juce::Graphics& g) override
    {
        g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    }
    
    void resized() override
    {
        //        table.setBounds (getLocalBounds());
        //        tabbed.setBounds (getLocalBounds());
    }
    
    void showError(const juce::String &msg) {
        juce::AlertWindow::showMessageBox (
                                           juce::AlertWindow::WarningIcon,
                                           "Error",
                                           msg,
                                           "Dismiss"
                                           );
        
    }
    
    private:
    //==============================================================================
    std::unique_ptr<juce::BubbleMessageComponent> bubbleMessage;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SysexValidator)
};
