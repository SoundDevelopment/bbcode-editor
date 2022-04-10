/*
    ==============================================================================

    This file is part of the bbcode_editor JUCE module
    Copyright (c) 2019 - 2022 Sound Development - Marcel Huibers
    All rights reserved.

    ------------------------------------------------------------------------------

    bbcode_editor is provided under the terms of The MIT License (MIT):

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sub-license, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.

    ==============================================================================
*/

/*******************************************************************************
 The block below describes the properties of this module, and is read by
 the Projucer to automatically generate project code that uses it.
 For details about the syntax and how to create or use a module, see the
 JUCE Module Format.txt file.

#if 0
BEGIN_JUCE_MODULE_DECLARATION

    ID:                 bbcode_editor
    vendor:             Sound Development
    name:               TextEditor that accepts BBCode.
    version:            0.8.0
    description:        TextEditor descendant that parses BBCode and formats accordingly.
    website:            https://www.sounddevelopment.nl
    license:            MIT
    minimumCppStandard: 17
    dependencies:       juce_gui_basics, juce_events, juce_graphics
END_JUCE_MODULE_DECLARATION

#endif

@defgroup bbcode_editor

 This juce module contains a BBCodeEditor component to format text according to BBCode.

*******************************************************************************/

#ifndef BBCODE_EDITOR_HEADER_H
#define BBCODE_EDITOR_HEADER_H

// clang-format off
#include <juce_gui_basics/juce_gui_basics.h>
#include <optional>

#include "editor/sd_TextFormatState.h"

#include "editor/sd_BBcodeEditor.h"
// clang-format on

#endif  // BBCODE_EDITOR_HEADER_H
