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


#ifdef BBCODE_EDITOR_HEADER_H
   /** When you add this cpp file to your project, you mustn't include it in a file where you've
        already included any other headers - just put it inside a file on its own, possibly with your config
        flags preceding it, but don't include anything else. That also includes avoiding any automatic prefix
        header files that the compiler may be using.
    */
   #error "Incorrect use of bbcode_editor cpp file"
#endif

#include "bbcode_editor.h"

#include "editor/sd_TextFormatState.cpp"
#include "editor/sd_BBcodeEditor.cpp"