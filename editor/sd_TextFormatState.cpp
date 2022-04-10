/*
  =====================================================================================================

    sd_TextFormatState.cpp
    Created  : 9 Apr 2022 11:10:29pm
    Author   : Marcel Huibers
    Project  : SD Toolkit
    Company  : Sound Development
    Copyright: Marcel Huibers (c) 2022 All Rights Reserved

  =====================================================================================================
*/


namespace sd
{

//==============================================================================

void TextFormatState::initialiseParser()
{
    if( !m_parser.empty() )
        return;

    m_parser[BBCode::kBoldToken]      = [&]( const juce::String& /*value*/, bool endToken ) noexcept { return setStyle( juce::Font::bold, !endToken ); };
    m_parser[BBCode::kItalicToken]    = [&]( const juce::String& /*value*/, bool endToken ) noexcept { return setStyle( juce::Font::italic, !endToken ); };
    m_parser[BBCode::kUnderlineToken] = [&]( const juce::String& /*value*/, bool endToken ) noexcept { return setStyle( juce::Font::underlined, !endToken ); };
    m_parser[BBCode::kSizeToken]      = [&]( const juce::String& value, bool endToken ) noexcept { return setHeight( value, !endToken ); };
    m_parser[BBCode::kColourToken]    = [&]( const juce::String& value, bool endToken ) { return setColour( value, !endToken ); };
    m_parser[BBCode::kFontToken]      = [&]( const juce::String& value, bool endToken ) { return setFont( value, !endToken ); };
    m_parser[BBCode::kCodeToken]      = [&]( const juce::String& /*value*/, bool endToken ) { return setFont( "courier", !endToken ); };
    m_parser[BBCode::kQuoteToken]     = [&]( const juce::String& /*value*/, bool endToken ) { return setStyle( juce::Font::italic, !endToken ); };
}
//==============================================================================

juce::Font TextFormatState::getFont() const
{
    if( m_fontName.isNotEmpty() )
        return juce::Font( m_fontName, m_fontHeight, m_styleFlags );

    return juce::Font().withHeight( m_fontHeight ).withStyle( m_styleFlags );
}
//==============================================================================

std::optional<TextFormatState> TextFormatState::withToken( const juce::String& token ) const
{
    TextFormatState newState( *this );
    if( newState.parseToken( token ) == StateChanged::No )
        return std::nullopt;
    return newState;
}
//==============================================================================

TextFormatState::StateChanged TextFormatState::parseToken( const juce::String& token )
{
    initialiseParser();

    const auto actualToken = token.trimCharactersAtStart( BBCode::kCloseTokenPrefix ).upToFirstOccurrenceOf( BBCode::kValueDelimiter, false, false ).toLowerCase();

    if( m_parser.find( actualToken ) != m_parser.end() )
    {
        const auto value = token.fromFirstOccurrenceOf( BBCode::kValueDelimiter, false, false );
        return m_parser.at( actualToken )( value, token.startsWith( BBCode::kCloseTokenPrefix ) );
    }

    return StateChanged::No;
}
//==============================================================================

TextFormatState::StateChanged TextFormatState::setStyle( int style, bool enable ) noexcept
{
    if( !enable && ( ( m_styleFlags & style ) == 0 ) )
        return StateChanged::No;
    m_styleFlags = ( enable ? ( m_styleFlags | style ) : ( m_styleFlags & ~style ) );  // NOLINT
    return StateChanged::Yes;
}
//==============================================================================

TextFormatState::StateChanged TextFormatState::setHeight( const juce::String& height, bool enable ) noexcept
{
    auto fontHeight = kDefaultFontHeight;

    if( enable )
        fontHeight = std::clamp( height.getFloatValue(), kMinimumFontHeight, kMaximumFontHeight );

    if( m_fontHeight == fontHeight )
        return StateChanged::No;

    m_fontHeight = fontHeight;
    return StateChanged::Yes;
}
//==============================================================================

TextFormatState::StateChanged TextFormatState::setColour( const juce::String& colourName, bool enable )
{
    if( enable )
    {
        const auto filteredColour = colourName.retainCharacters( "#0123456789ABCDEFabcdef" );

        if( filteredColour.startsWith( "#" ) && filteredColour.length() == kColourIDlength )
        {
            m_colour = juce::Colour::fromString( filteredColour ).withAlpha( 1.0F );
        }
        else
        {
            if( auto bbColour = TextFormatState::getBBcolor( colourName ) )
                m_colour = bbColour.operator*();
        }
        return StateChanged::Yes;
    }

    if( m_colour == juce::Colour { kDefaultColour } )
        return StateChanged::No;

    m_colour = juce::Colour { kDefaultColour };
    return StateChanged::Yes;
}
//==============================================================================

TextFormatState::StateChanged TextFormatState::setFont( const juce::String& fontName, bool enable )
{
    auto newFontName { fontName };
    if( enable )
    {
        // Lookup font family in the system fonts...
        const auto systemFonts = juce::Font::findAllTypefaceNames();
        if( const auto* systemFont =
              std::find_if( systemFonts.begin(), systemFonts.end(), [&fontName]( const juce::String& x ) noexcept { return x.containsIgnoreCase( fontName ); } );
            systemFont != systemFonts.end() )
        {
            if( systemFont )
                newFontName = *systemFont;
        }
    }

    if( newFontName == m_fontName )
        return StateChanged::No;

    m_fontName = newFontName;
    return StateChanged::Yes;
}
//==============================================================================

std::optional<juce::Colour> TextFormatState::getBBcolor( const juce::String& colour )  // NOLINT
{
    if( colour.equalsIgnoreCase( "black" ) )
        return juce::Colour( 0xFF000000 );  // NOLINT
    if( colour.equalsIgnoreCase( "alicewhite" ) )
        return juce::Colour( 0xFFF0F8FF );  // NOLINT
    if( colour.equalsIgnoreCase( "antiquewhite" ) )
        return juce::Colour( 0xFFFAEBD7 );  // NOLINT
    if( colour.equalsIgnoreCase( "aqua" ) )
        return juce::Colour( 0xFF00FFFF );  // NOLINT
    if( colour.equalsIgnoreCase( "aquamarine" ) )
        return juce::Colour( 0xFF7FFFD4 );  // NOLINT
    if( colour.equalsIgnoreCase( "azure" ) )
        return juce::Colour( 0xFFF0FFFF );  // NOLINT
    if( colour.equalsIgnoreCase( "beige" ) )
        return juce::Colour( 0xFFF5F5DC );  // NOLINT
    if( colour.equalsIgnoreCase( "bisque" ) )
        return juce::Colour( 0xFFFFE4C4 );  // NOLINT
    if( colour.equalsIgnoreCase( "blanchedalmond" ) )
        return juce::Colour( 0xFFFFEBCD );  // NOLINT
    if( colour.equalsIgnoreCase( "blue" ) )
        return juce::Colour( 0xFF0000FF );  // NOLINT
    if( colour.equalsIgnoreCase( "blueviolet" ) )
        return juce::Colour( 0xFF8A2BE2 );  // NOLINT
    if( colour.equalsIgnoreCase( "brown" ) )
        return juce::Colour( 0xFFA52A2A );  // NOLINT
    if( colour.equalsIgnoreCase( "burlywood" ) )
        return juce::Colour( 0xFFDEB887 );  // NOLINT
    if( colour.equalsIgnoreCase( "cadetblue" ) )
        return juce::Colour( 0xFF5F9EA0 );  // NOLINT
    if( colour.equalsIgnoreCase( "chartreuse" ) )
        return juce::Colour( 0xFF7FFF00 );  // NOLINT
    if( colour.equalsIgnoreCase( "chocolate" ) )
        return juce::Colour( 0xFFD2691E );  // NOLINT
    if( colour.equalsIgnoreCase( "coral" ) )
        return juce::Colour( 0xFFFF7F50 );  // NOLINT
    if( colour.equalsIgnoreCase( "cornflowerblue" ) )
        return juce::Colour( 0xFF6495ED );  // NOLINT
    if( colour.equalsIgnoreCase( "cornsilk" ) )
        return juce::Colour( 0xFFFFF8DC );  // NOLINT
    if( colour.equalsIgnoreCase( "crimson" ) )
        return juce::Colour( 0xFFDC143C );  // NOLINT
    if( colour.equalsIgnoreCase( "cyan" ) )
        return juce::Colour( 0xFF00FFFF );  // NOLINT
    if( colour.equalsIgnoreCase( "darkblue" ) )
        return juce::Colour( 0xFF00008B );  // NOLINT
    if( colour.equalsIgnoreCase( "darkcyan" ) )
        return juce::Colour( 0xFF008B8B );  // NOLINT
    if( colour.equalsIgnoreCase( "darkgoldenrod" ) )
        return juce::Colour( 0xFFB8860B );  // NOLINT
    if( colour.equalsIgnoreCase( "darkgray" ) )
        return juce::Colour( 0xFFA9A9A9 );  // NOLINT
    if( colour.equalsIgnoreCase( "darkgreen" ) )
        return juce::Colour( 0xFF006400 );  // NOLINT
    if( colour.equalsIgnoreCase( "darkkhaki" ) )
        return juce::Colour( 0xFFBDB76B );  // NOLINT
    if( colour.equalsIgnoreCase( "darkmagenta" ) )
        return juce::Colour( 0xFF8B008B );  // NOLINT
    if( colour.equalsIgnoreCase( "darkolivegreen" ) )
        return juce::Colour( 0xFF556B2F );  // NOLINT
    if( colour.equalsIgnoreCase( "darkorange" ) )
        return juce::Colour( 0xFFFF8C00 );  // NOLINT
    if( colour.equalsIgnoreCase( "darkorchid" ) )
        return juce::Colour( 0xFF9932CC );  // NOLINT
    if( colour.equalsIgnoreCase( "darkred" ) )
        return juce::Colour( 0xFF8B0000 );  // NOLINT
    if( colour.equalsIgnoreCase( "darksalmon" ) )
        return juce::Colour( 0xFFE9967A );  // NOLINT
    if( colour.equalsIgnoreCase( "darkseagreen" ) )
        return juce::Colour( 0xFF8FBC8F );  // NOLINT
    if( colour.equalsIgnoreCase( "darkslateblue" ) )
        return juce::Colour( 0xFF483D8B );  // NOLINT
    if( colour.equalsIgnoreCase( "darkslategray" ) )
        return juce::Colour( 0xFF2F4F4F );  // NOLINT
    if( colour.equalsIgnoreCase( "darkturquoise" ) )
        return juce::Colour( 0xFF00CED1 );  // NOLINT
    if( colour.equalsIgnoreCase( "darkviolet" ) )
        return juce::Colour( 0xFF9400D3 );  // NOLINT
    if( colour.equalsIgnoreCase( "deeppink" ) )
        return juce::Colour( 0xFFFF1493 );  // NOLINT
    if( colour.equalsIgnoreCase( "deepskyblue" ) )
        return juce::Colour( 0xFF00BFFF );  // NOLINT
    if( colour.equalsIgnoreCase( "dimgray" ) )
        return juce::Colour( 0xFF696969 );  // NOLINT
    if( colour.equalsIgnoreCase( "dodgerblue" ) )
        return juce::Colour( 0xFF1E90FF );  // NOLINT
    if( colour.equalsIgnoreCase( "firebrick" ) )
        return juce::Colour( 0xFFB22222 );  // NOLINT
    if( colour.equalsIgnoreCase( "floralwhite" ) )
        return juce::Colour( 0xFFFFFAF0 );  // NOLINT
    if( colour.equalsIgnoreCase( "forestgreen" ) )
        return juce::Colour( 0xFF228B22 );  // NOLINT
    if( colour.equalsIgnoreCase( "fuchsia" ) )
        return juce::Colour( 0xFFFF00FF );  // NOLINT
    if( colour.equalsIgnoreCase( "gainsboro" ) )
        return juce::Colour( 0xFFDCDCDC );  // NOLINT
    if( colour.equalsIgnoreCase( "ghostwhite" ) )
        return juce::Colour( 0xFFF8F8FF );  // NOLINT
    if( colour.equalsIgnoreCase( "gold" ) )
        return juce::Colour( 0xFFFFD700 );  // NOLINT
    if( colour.equalsIgnoreCase( "goldenrod" ) )
        return juce::Colour( 0xFFDAA520 );  // NOLINT
    if( colour.equalsIgnoreCase( "gray" ) )
        return juce::Colour( 0xFF808080 );  // NOLINT
    if( colour.equalsIgnoreCase( "green" ) )
        return juce::Colour( 0xFF008000 );  // NOLINT
    if( colour.equalsIgnoreCase( "greenyellow" ) )
        return juce::Colour( 0xFFADFF2F );  // NOLINT
    if( colour.equalsIgnoreCase( "honeydew" ) )
        return juce::Colour( 0xFFF0FFF0 );  // NOLINT
    if( colour.equalsIgnoreCase( "hotpink" ) )
        return juce::Colour( 0xFFFF69B4 );  // NOLINT
    if( colour.equalsIgnoreCase( "indianred" ) )
        return juce::Colour( 0xFFCD5C5C );  // NOLINT
    if( colour.equalsIgnoreCase( "indigo" ) )
        return juce::Colour( 0xFF4B0082 );  // NOLINT
    if( colour.equalsIgnoreCase( "ivory" ) )
        return juce::Colour( 0xFFFFFFF0 );  // NOLINT
    if( colour.equalsIgnoreCase( "khaki" ) )
        return juce::Colour( 0xFFF0E68C );  // NOLINT
    if( colour.equalsIgnoreCase( "lavender" ) )
        return juce::Colour( 0xFFE6E6FA );  // NOLINT
    if( colour.equalsIgnoreCase( "lavenderblush" ) )
        return juce::Colour( 0xFFFFF0F5 );  // NOLINT
    if( colour.equalsIgnoreCase( "lawngreen" ) )
        return juce::Colour( 0xFF7CFC00 );  // NOLINT
    if( colour.equalsIgnoreCase( "lemonchiffon" ) )
        return juce::Colour( 0xFFFFFACD );  // NOLINT
    if( colour.equalsIgnoreCase( "lightblue" ) )
        return juce::Colour( 0xFFADD8E6 );  // NOLINT
    if( colour.equalsIgnoreCase( "lightcoral" ) )
        return juce::Colour( 0xFFF08080 );  // NOLINT
    if( colour.equalsIgnoreCase( "lightcyan" ) )
        return juce::Colour( 0xFFE0FFFF );  // NOLINT
    if( colour.equalsIgnoreCase( "lightgoldenrodyellow" ) )
        return juce::Colour( 0xFFFAFAD2 );  // NOLINT
    if( colour.equalsIgnoreCase( "lightgreen" ) )
        return juce::Colour( 0xFF90EE90 );  // NOLINT
    if( colour.equalsIgnoreCase( "lightgrey" ) )
        return juce::Colour( 0xFFD3D3D3 );  // NOLINT
    if( colour.equalsIgnoreCase( "lightpink" ) )
        return juce::Colour( 0xFFFFB6C1 );  // NOLINT
    if( colour.equalsIgnoreCase( "lightsalmon" ) )
        return juce::Colour( 0xFFFFA07A );  // NOLINT
    if( colour.equalsIgnoreCase( "lightseagreen" ) )
        return juce::Colour( 0xFF20B2AA );  // NOLINT
    if( colour.equalsIgnoreCase( "lightskyblue" ) )
        return juce::Colour( 0xFF87CEFA );  // NOLINT
    if( colour.equalsIgnoreCase( "lightslategray" ) )
        return juce::Colour( 0xFF778899 );  // NOLINT
    if( colour.equalsIgnoreCase( "lightsteelblue" ) )
        return juce::Colour( 0xFFB0C4DE );  // NOLINT
    if( colour.equalsIgnoreCase( "linen" ) )
        return juce::Colour( 0xFFFAF0E6 );  // NOLINT
    if( colour.equalsIgnoreCase( "magenta" ) )
        return juce::Colour( 0xFFFF00FF );  // NOLINT
    if( colour.equalsIgnoreCase( "maroon" ) )
        return juce::Colour( 0xFF800000 );  // NOLINT
    if( colour.equalsIgnoreCase( "mediumaquamarine" ) )
        return juce::Colour( 0xFF66CDAA );  // NOLINT
    if( colour.equalsIgnoreCase( "mediumblue" ) )
        return juce::Colour( 0xFF0000CD );  // NOLINT
    if( colour.equalsIgnoreCase( "mediumorchid" ) )
        return juce::Colour( 0xFFBA55D3 );  // NOLINT
    if( colour.equalsIgnoreCase( "mediumpurple" ) )
        return juce::Colour( 0xFF9370D8 );  // NOLINT
    if( colour.equalsIgnoreCase( "mediumseagreen" ) )
        return juce::Colour( 0xFF3CB371 );  // NOLINT
    if( colour.equalsIgnoreCase( "mediumslateblue" ) )
        return juce::Colour( 0xFF7B68EE );  // NOLINT
    if( colour.equalsIgnoreCase( "mediumspringgreen" ) )
        return juce::Colour( 0xFF00FA9A );  // NOLINT
    if( colour.equalsIgnoreCase( "mediumturquoise" ) )
        return juce::Colour( 0xFF48D1CC );  // NOLINT
    if( colour.equalsIgnoreCase( "mediumvioletred" ) )
        return juce::Colour( 0xFFC71585 );  // NOLINT
    if( colour.equalsIgnoreCase( "midnightblue" ) )
        return juce::Colour( 0xFF191970 );  // NOLINT
    if( colour.equalsIgnoreCase( "mintcream" ) )
        return juce::Colour( 0xFFF5FFFA );  // NOLINT
    if( colour.equalsIgnoreCase( "mistyrose" ) )
        return juce::Colour( 0xFFFFE4E1 );  // NOLINT
    if( colour.equalsIgnoreCase( "moccasin" ) )
        return juce::Colour( 0xFFFFE4B5 );  // NOLINT
    if( colour.equalsIgnoreCase( "navajowhite" ) )
        return juce::Colour( 0xFFFFDEAD );  // NOLINT
    if( colour.equalsIgnoreCase( "navy" ) )
        return juce::Colour( 0xFF000080 );  // NOLINT
    if( colour.equalsIgnoreCase( "oldlace" ) )
        return juce::Colour( 0xFFFDF5E6 );  // NOLINT
    if( colour.equalsIgnoreCase( "olive" ) )
        return juce::Colour( 0xFF808000 );  // NOLINT
    if( colour.equalsIgnoreCase( "olivedrab" ) )
        return juce::Colour( 0xFF688E23 );  // NOLINT
    if( colour.equalsIgnoreCase( "orange" ) )
        return juce::Colour( 0xFFFFA500 );  // NOLINT
    if( colour.equalsIgnoreCase( "orangered" ) )
        return juce::Colour( 0xFFFF4500 );  // NOLINT
    if( colour.equalsIgnoreCase( "orchid" ) )
        return juce::Colour( 0xFFDA70D6 );  // NOLINT
    if( colour.equalsIgnoreCase( "palegoldenrod" ) )
        return juce::Colour( 0xFFEEE8AA );  // NOLINT
    if( colour.equalsIgnoreCase( "palegreen" ) )
        return juce::Colour( 0xFF98FB98 );  // NOLINT
    if( colour.equalsIgnoreCase( "paleturquoise" ) )
        return juce::Colour( 0xFFAFEEEE );  // NOLINT
    if( colour.equalsIgnoreCase( "palevioletred" ) )
        return juce::Colour( 0xFFD87093 );  // NOLINT
    if( colour.equalsIgnoreCase( "papayawhip" ) )
        return juce::Colour( 0xFFFFEFD5 );  // NOLINT
    if( colour.equalsIgnoreCase( "peachpuff" ) )
        return juce::Colour( 0xFFFFDAB9 );  // NOLINT
    if( colour.equalsIgnoreCase( "peru" ) )
        return juce::Colour( 0xFFCD853F );  // NOLINT
    if( colour.equalsIgnoreCase( "pink" ) )
        return juce::Colour( 0xFFFFC0CB );  // NOLINT
    if( colour.equalsIgnoreCase( "plum" ) )
        return juce::Colour( 0xFFDDA0DD );  // NOLINT
    if( colour.equalsIgnoreCase( "powderblue" ) )
        return juce::Colour( 0xFFB0E0E6 );  // NOLINT
    if( colour.equalsIgnoreCase( "purple" ) )
        return juce::Colour( 0xFF800080 );  // NOLINT
    if( colour.equalsIgnoreCase( "red" ) )
        return juce::Colour( 0xFFFF0000 );  // NOLINT
    if( colour.equalsIgnoreCase( "rosybrown" ) )
        return juce::Colour( 0xFFBC8F8F );  // NOLINT
    if( colour.equalsIgnoreCase( "royalblue" ) )
        return juce::Colour( 0xFF4169E1 );  // NOLINT
    if( colour.equalsIgnoreCase( "saddlebrown" ) )
        return juce::Colour( 0xFF8B4513 );  // NOLINT
    if( colour.equalsIgnoreCase( "salmon" ) )
        return juce::Colour( 0xFFFA8072 );  // NOLINT
    if( colour.equalsIgnoreCase( "sandybrown" ) )
        return juce::Colour( 0xFFF4A460 );  // NOLINT
    if( colour.equalsIgnoreCase( "seagreen" ) )
        return juce::Colour( 0xFF2E8B57 );  // NOLINT
    if( colour.equalsIgnoreCase( "seashell" ) )
        return juce::Colour( 0xFFFFF5EE );  // NOLINT
    if( colour.equalsIgnoreCase( "sienna" ) )
        return juce::Colour( 0xFFA0522D );  // NOLINT
    if( colour.equalsIgnoreCase( "silver" ) )
        return juce::Colour( 0xFFC0C0C0 );  // NOLINT
    if( colour.equalsIgnoreCase( "skyblue" ) )
        return juce::Colour( 0xFF87CEEB );  // NOLINT
    if( colour.equalsIgnoreCase( "slateblue" ) )
        return juce::Colour( 0xFF6A5ACD );  // NOLINT
    if( colour.equalsIgnoreCase( "slategray" ) )
        return juce::Colour( 0xFF708090 );  // NOLINT
    if( colour.equalsIgnoreCase( "snow" ) )
        return juce::Colour( 0xFFFFFAFA );  // NOLINT
    if( colour.equalsIgnoreCase( "springgreen" ) )
        return juce::Colour( 0xFF00FF7F );  // NOLINT
    if( colour.equalsIgnoreCase( "steelblue" ) )
        return juce::Colour( 0xFF4682B4 );  // NOLINT
    if( colour.equalsIgnoreCase( "tan" ) )
        return juce::Colour( 0xFFD2B48C );  // NOLINT
    if( colour.equalsIgnoreCase( "teal" ) )
        return juce::Colour( 0xFF008080 );  // NOLINT
    if( colour.equalsIgnoreCase( "thistle" ) )
        return juce::Colour( 0xFFD8BFD8 );  // NOLINT
    if( colour.equalsIgnoreCase( "tomato" ) )
        return juce::Colour( 0xFFFF6347 );  // NOLINT
    if( colour.equalsIgnoreCase( "turquoise" ) )
        return juce::Colour( 0xFF40E0D0 );  // NOLINT
    if( colour.equalsIgnoreCase( "violet" ) )
        return juce::Colour( 0xFFEE82EE );  // NOLINT
    if( colour.equalsIgnoreCase( "wheat" ) )
        return juce::Colour( 0xFFF5DEB3 );  // NOLINT
    if( colour.equalsIgnoreCase( "white" ) )
        return juce::Colour( 0xFFFFFFFF );  // NOLINT
    if( colour.equalsIgnoreCase( "whitesmoke" ) )
        return juce::Colour( 0xFFF5F5F5 );  // NOLINT
    if( colour.equalsIgnoreCase( "yellow" ) )
        return juce::Colour( 0xFFFFFF00 );  // NOLINT
    if( colour.equalsIgnoreCase( "yellowgreen" ) )
        return juce::Colour( 0xFF9ACD32 );  // NOLINT

    return std::nullopt;
}
//==============================================================================

}  // namespace sd
