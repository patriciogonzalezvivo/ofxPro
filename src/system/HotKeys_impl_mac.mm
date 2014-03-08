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

#include "HotKeys.h"

#ifdef TARGET_OSX

#include <Cocoa/Cocoa.h>

bool getModifierSelection(){
	return getModifierShiftPressed() || getModifierSpecialPressed();
}

bool getModifierShortcutKeyPressed(){
#ifdef MAC_USE_CONTROL
//		cout << "using command" << endl;
	return getModifierControlPressed();
#else
//		cout << "using control" << endl;
	return getModifierSpecialPressed();
#endif
}

bool getModifierPressed(ModifierKey mod) {
	unsigned int t = 0;

	if ((MODIFIER_KEY_CTRL & mod) == MODIFIER_KEY_CTRL)
		t += NSControlKeyMask;
	
	if ((MODIFIER_KEY_ALT & mod) == MODIFIER_KEY_ALT)
		t += NSAlternateKeyMask;
	
	if ((MODIFIER_KEY_SHIFT & mod) == MODIFIER_KEY_SHIFT)
		t += NSShiftKeyMask;

	if ((MODIFIER_KEY_SPECIAL & mod) == MODIFIER_KEY_SPECIAL)
		t += NSCommandKeyMask;

	return [[NSApp currentEvent] modifierFlags] & t;
}

#endif