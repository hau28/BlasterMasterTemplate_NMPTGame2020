#pragma once
#include "GameObject.h"

class CTileArea :
    public CGameObject
{
private:
    int width;
    int height;

    /// <summary>
    /// Use class id to know the exact tile type
    /// </summary>

public:
    void SetSize(int width, int height);
    void GetSize(int& width, int& height);

    CTileArea() {}
    CTileArea(int classId, int x, int y, int width, int height);

    virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
    virtual void Render();
};

typedef CTileArea* LPTILE_AREA;
