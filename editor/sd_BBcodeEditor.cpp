/*
  =====================================================================================================

    sd_BBCodeEditor.cpp
    Created  : 10 Apr 2022 1:08:13am
    Author   : Marcel Huibers
    Project  : SD Toolkit
    Company  : Sound Development
    Copyright: Marcel Huibers (c) 2022 All Rights Reserved

  =====================================================================================================
*/

namespace sd
{

void BBCodeEditor::initialise( )
{   
    setJustification( kDefaultJustification );
    m_listPrefix  = false;
    m_quotePrefix = false;
    m_stateQueue.clear();
    m_stateQueue.emplace_back( sd::TextFormatState( findColour( juce::TextEditor::textColourId ) ) );
    clear();
}
//==============================================================================

void BBCodeEditor::setBBText( const juce::String& bbText )
{
    initialise();

    auto plainText     = bbText.upToFirstOccurrenceOf( BBCode::kTokenStart, false, false );
    auto remainingText = bbText.fromFirstOccurrenceOf( BBCode::kTokenStart, false, false );
    addBBText( plainText );

    while( remainingText.isNotEmpty() )
    {
        // Ignore superfluous '['...
        if( remainingText.startsWith( BBCode::kTokenStart ) )
        {
            remainingText = remainingText.substring( 1 );
            addBBText( BBCode::kTokenStart );
            continue;
        }

        // Check for 'quote' tokens...
        juce::String value {};
        if( remainingText.startsWith( BBCode::kQuoteToken ) )
        {
            value = remainingText.upToFirstOccurrenceOf( BBCode::kTokenEnd, false, false ).fromFirstOccurrenceOf( BBCode::kValueDelimiter, false, false );
            m_quotePrefix = true;
        }

        // Process lists...
        bool succesfullyParsed = true;
        if( remainingText.startsWith( BBCode::kBulletToken ) )
        {
            remainingText = remainingText.substring( 1 );
            m_listPrefix  = true;
        }
        else
        {
            const auto token = remainingText.upToFirstOccurrenceOf( BBCode::kTokenEnd, false, false );

            if( token.contains( BBCode::kTokenStart ) )
            {
                succesfullyParsed = false;
            }
            // Parse justification (juce::TextEditor only has global justification)...
            else if( token.startsWith( BBCode::kAlignToken ) )
            {
                parseJustification( token, !token.startsWith( BBCode::kCloseTokenPrefix ) );
            }
            // Parse other tokens...
            else if( auto newState = m_stateQueue.back().withToken( token ) )
            {
                // End token pops state...
                if( token.startsWith( BBCode::kCloseTokenPrefix ) )
                {
                    if( m_stateQueue.size() > 1 )
                        m_stateQueue.pop_back();
                }
                // Start token pushes state...
                else
                {
                    m_stateQueue.emplace_back( newState.operator*() );
                }
            }
            else
            {
                succesfullyParsed = false;
            }
        }

        // Assemble plain text to be added to the editor...
        if( succesfullyParsed )
            plainText = remainingText.fromFirstOccurrenceOf( BBCode::kTokenEnd, false, false ).upToFirstOccurrenceOf( BBCode::kTokenStart, false, false );
        else
            plainText = BBCode::kTokenStart + remainingText.upToFirstOccurrenceOf( BBCode::kTokenStart, false, false );

        // Trailing newlines for CODE blocks...
        if( remainingText.startsWith( BBCode::kCodeToken ) )
            plainText = juce::newLine + juce::newLine + kTabCharacter + plainText + juce::newLine + juce::newLine;

        addBBText( plainText, value );

        // Progress the parser....
        remainingText = remainingText.fromFirstOccurrenceOf( BBCode::kTokenStart, false, false );
    }
}
//==============================================================================

void BBCodeEditor::addBBText( const juce::String& text, const juce::String& value /*= {}*/ )
{
    if( text.isEmpty() )
        return;

    sd::TextFormatState textFormatState { findColour( juce::TextEditor::textColourId ) };  // Default state.
    if( !m_stateQueue.empty() )
        textFormatState = m_stateQueue.back();

    setColour( juce::TextEditor::textColourId, textFormatState.getColour() );
    setFont( textFormatState.getFont() );
    moveCaretToEnd();
    
    // Add quote...
    if( m_quotePrefix )
    {
        insertTextAtCaret( juce::newLine + juce::newLine + "|" + kTabCharacter );
        if( value.isNotEmpty() )
        {
            const auto previousFont = getFont();
            const auto wasBold      = previousFont.isBold();
            setFont( previousFont.boldened() );
            insertTextAtCaret( value + ": " );
            if( !wasBold )
                setFont( previousFont.withStyle( previousFont.getStyleFlags() & ~juce::Font::bold ) );  // NOLINT
        }
        insertTextAtCaret( juce::String::fromUTF8( kOpenQuotes ) + text + juce::String::fromUTF8( kCloseQuotes ) );
    }
    // Add bullet list item...
    else if( m_listPrefix )
    {
        insertTextAtCaret( juce::String::fromUTF8( kBulletCharacter ) + kTabCharacter + text );
    }
    // Add plain text...
    else
    {
        insertTextAtCaret( text );
    }
    m_listPrefix  = false;
    m_quotePrefix = false;
}
//==============================================================================

void BBCodeEditor::parseJustification( const juce::String& token, bool enable )
{
    m_justification = kDefaultJustification;
    if( enable )
    {
        if( token.endsWithIgnoreCase( "right" ) )
            m_justification = juce::Justification::topRight;
        else if( token.endsWithIgnoreCase( "center" ) )
            m_justification = juce::Justification::centredTop;
        else if( token.endsWithIgnoreCase( "justify" ) )
            m_justification = juce::Justification::horizontallyJustified;
        else
            m_justification = juce::Justification::topLeft;
    }

    setJustification( m_justification );
}

}  // namespace sd
