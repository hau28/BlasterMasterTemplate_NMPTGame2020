#pragma once
#include "Game.h"
#include "Animations.h"

class CSophiaAnimationSystem
{
    /// CuteTN Note:
    /// Well how about making this a Singleton?

private:
    static CSophiaAnimationSystem* __instance;

    unordered_map<int, LPSPRITE> sprites;

    LPDIRECT3DTEXTURE9 texSophia;
    int numberOfRowTexture, numberOfColumnTexture;
    int spriteWidth, spriteHeight;

    /// <summary>
    /// Number of states in each dimension
    /// </summary>
    int directionDimension, gunDimension, bodyDimension, wheelDimension;
    
	void _ParseSection_TEXTURE(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_DIMENSIONS(string line);
	void _ParseSection_SEQUENCES(string line);

    virtual int EncodeDimensions(int directionState, int gunState, int bodyState, int wheelState);
	virtual LPSPRITE GetSprite(int state);

public:
    CSophiaAnimationSystem() {};

    /// <summary>
    /// <para> Input SOPHIA sprite information from a file </para>
    /// <para> CuteTN Note: It's very ugly to put the input function here. </para>
    /// <para> But you know, I don't wanna touch anything else but this class :) </para>
    /// </summary>
    virtual void Input(LPCWSTR filePath);

    virtual LPSPRITE GetSprite(int directionState, int gunState, int bodyState, int wheelState);

    static CSophiaAnimationSystem* GetInstance();
};

