/**
 * ofxTimeline
 * openFrameworks graphical timeline addon
 *
 * Copyright (c) 2011-2012 James George
 * Development Supported by YCAM InterLab http://interlab.ycam.jp/en/
 * http://jamesgeorge.org + http://flightphase.com
 * http://github.com/obviousjim + http://github.com/flightphase
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#pragma once

#include "ofMain.h"

enum ModifierKey {
	MODIFIER_KEY_SHIFT = 1,
	MODIFIER_KEY_CTRL = 2,
 	MODIFIER_KEY_ALT  = 4,
	MODIFIER_KEY_SPECIAL = 8
};

//platform specific
bool getModifierPressed(ModifierKey key);

//convention for elements to be selected
//COMMAND or SHIFT for OSX, just CTRL on windows
bool getModifierSelection();


//COMMAND on OS X, CTRL on Windows
//----------------------------------------
bool getModifierShortcutKeyPressed();

//----------------------------------------
static bool getModifierAltPressed(){
	return getModifierPressed(MODIFIER_KEY_ALT);
}

//----------------------------------------
static bool getModifierShiftPressed(){
	return getModifierPressed(MODIFIER_KEY_SHIFT);
}

//----------------------------------------
static bool getModifierControlPressed(){
	return getModifierPressed(MODIFIER_KEY_CTRL);
}

//----------------------------------------
static bool getModifierSpecialPressed(){
	return getModifierPressed(MODIFIER_KEY_SPECIAL);
}