/*
  =====================================================================================================

    sd_BBCodeEditor.h
    Created  : 10 Apr 2022 1:08:13am
    Author   : Marcel Huibers
    Project  : SD Library
    Company  : Sound Development
    Copyright: Marcel Huibers (c) 2020 All Rights Reserved

  =====================================================================================================
*/

#pragma once


namespace sd
{

class BBCodeEditor : public juce::TextEditor
{
public:
    static constexpr auto kDefaultJustification { juce::Justification::topLeft };
    static constexpr auto kBulletCharacter { u8"\u2022" };
    static constexpr auto kOpenQuotes { u8"\u201C" };
    static constexpr auto kCloseQuotes { u8"\u201D" };
    static constexpr auto kTabCharacter { "    " };

    using juce::TextEditor::TextEditor;

    void setBBText( const juce::String& bbText );

private:
    std::vector<sd::TextFormatState> m_stateQueue;
    bool                             m_listPrefix { false };
    bool                             m_quotePrefix { false };
    juce::Justification              m_justification { kDefaultJustification };

    void initialise();
    void addBBText( const juce::String& text, const juce::String& value = {} );
    void parseJustification( const juce::String& justification, bool enable );

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR( BBCodeEditor )
};


}  // namespace sd
