# BBCodeEditor
[Juce](https://juce.com/) BBCodeEditor module.<br>
<img align="right" src="https://www.sounddevelopment.nl/sd/resources/images/sound_meter/sound_meter_demo.gif">
*by Marcel Huibers | [Sound Development](https://www.sounddevelopment.nl) 2022 | Published under the [MIT License](https://en.wikipedia.org/wiki/MIT_License)*

A descendant of `juce::TextEditor` that accepts [BBCode](https://www.bbcode.org/reference.php) formatted text.
Supports all formatting options that `juce::TextEditor` supports.

Features:
- Font family, height and colour.
- **Bold**, *italic* and underline.
- Bullet lists.
- Quotes.
- Code segments.

<br>
An example project, demonstrating BBCodeEditor can be found [here](https://github.com/SoundDevelopment/sound_meter-example)...

# Usage

BBCodeEditor can be easily swapped with any existing `juce::TextEditor`.

Declare the editor:
```
sd::BBCodeEditor m_codeEditor;
```
Add it in your constructor:
```
addAndMakeVisible(m_codeEditor);
```
Add BBCode formatted text to the editor:
```
m_codeEditor.setBBtext( bbText );
```
<br><br>

-----
*Sound Development 2022*
