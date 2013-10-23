#pragma once

#include "ofMain.h"
#include <iterator>

#include "TextShape.h"
#include "UIClass.h"

typedef struct {
    string      rawWord;
    float       width;
    float       height;
} WordBlock;

typedef struct {
    vector<int> wordsID;
    float       width;
    float       height;
} LineBlock;

class TextBlock : public TextShape {
public:
    TextBlock();

    void    loadFont(string _fontLocation, float _fontSize, int _dpi = 90);

    void    setText(string _inputText);
    void    setWrapping(bool _bWraping){ bWraping = _bWraping;};

    float   getTextWidth();
    float   getTextHeight();

    void    draw();
    void    draw(float _x, float _y, float _w = -1, float _h = -1);

protected:
    void    _loadWords();

    void    _trimLineSpaces();
    float   _getWidthOfWords();
    int     _getLinedWords();

    int     _wrapTextX(float lineWidth);                 //Returns the number of lines it formed.
    void    _wrapTextArea(float rWidth, float rHeight);
    bool    _wrapTextForceLines(int linesN);

    void    _forceScale(float _scale);

    vector<WordBlock>       words;
    vector<LineBlock>       lines;
    WordBlock               blankSpaceWord;

    bool                    bWraping;
};

