/*
  =====================================================================================================

    sd_TextFormatState.h
    Created  : 9 Apr 2022 11:10:29pm
    Author   : Marcel Huibers
    Project  : SD Toolkit
    Company  : Sound Development
    Copyright: Marcel Huibers (c) 2022 All Rights Reserved

  =====================================================================================================
*/

#pragma once


namespace sd
{

/**
 * @brief Class holding text formatting state.
 *
 * This includes: font family name, font height,
 * font style and font colour.
 */
class TextFormatState
{
public:
    static constexpr auto         kDefaultFontHeight { 15.0F };
    static constexpr auto         kMinimumFontHeight { 1.0F };
    static constexpr auto         kMaximumFontHeight { 200.0F };
    static constexpr juce::uint32 kDefaultColour { 0xffffffffU };
    static constexpr juce::uint32 kColourIDlength { 7 };

    enum class StateChanged
    {
        Yes,
        No
    };

    using Parser = std::map<juce::String, std::function<StateChanged( const juce::String&, bool )>>;

    TextFormatState( const juce::Colour& defaultColour = juce::Colour { kDefaultColour } ) noexcept
      : m_colour( defaultColour ), m_defaultColour( defaultColour )
    {
    }

    /**
     * @brief Parse a format state from the token.
     *
     * This parses the token and returns a new format state with
     * the corresponding format.
     *
     * @param token The BBCode token to be parsed.
     * @return      The newly created format state on successful parsing. Otherwise std::nullopt.
     */
    std::optional<TextFormatState> withToken( const juce::String& token ) const;

    /**
     * @brief Get the font, formatted according to the format state.
     *
     * @return The font with the correct style, size and font-family.
     *
     * @see getColour
     */
    [[nodiscard]] juce::Font getFont() const;

    /**
     * @brief Get the font colour, formatted according to the format state.
     *
     * @return The colour of the font.
     *
     * @see getFont
     */
    [[nodiscard]] juce::Colour getColour() const noexcept { return m_colour; }

private:
    Parser       m_parser;
    float        m_fontHeight { kDefaultFontHeight };
    int          m_styleFlags { juce::Font::plain };
    juce::String m_fontName {};
    juce::Colour m_colour { kDefaultColour };
    juce::Colour m_defaultColour { juce::Colours::black };

    /**
     * Parse the supplied token.
     * @return If the state has been changed (and needs pushing/popping).
     */
    StateChanged parseToken( const juce::String& token );
    void         initialiseParser();

    StateChanged setFont( const juce::String& fontName, bool enable );
    StateChanged setHeight( const juce::String& height, bool enable ) noexcept;
    StateChanged setStyle( int style, bool enable ) noexcept;
    StateChanged setColour( const juce::String& colour, bool enable );

    static std::optional<juce::Colour> getBBcolor( const juce::String& colour );

    JUCE_LEAK_DETECTOR( TextFormatState )
};


namespace BBCode
{

static constexpr auto kTokenStart       = "[";
static constexpr auto kTokenEnd         = "]";
static constexpr auto kCloseTokenPrefix = "/";
static constexpr auto kValueDelimiter   = "=";
static constexpr auto kBoldToken        = "b";
static constexpr auto kItalicToken      = "i";
static constexpr auto kUnderlineToken   = "u";
static constexpr auto kCodeToken        = "code";
static constexpr auto kAlignToken       = "align";
static constexpr auto kColourToken      = "color";
static constexpr auto kSizeToken        = "size";
static constexpr auto kFontToken        = "font";
static constexpr auto kQuoteToken       = "quote";
static constexpr auto kBulletToken      = "*";

}  // namespace BBCode

}  // namespace sd
