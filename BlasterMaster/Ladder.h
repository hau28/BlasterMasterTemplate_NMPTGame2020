#pragma once
#include "TileArea.h"

//enum class CLadder { Top, Body, Bottom, Null };

class CLadder :
    public CTileArea
{
private:

    int height;
    float top, bottom;

public:

    CLadder() {}
    CLadder(int classId, int x, int y, int width, int height, int sectionId);

};

typedef CLadder* LPLADDER;