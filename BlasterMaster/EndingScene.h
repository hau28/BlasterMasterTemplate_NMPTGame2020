#pragma once
#include "Scene.h"
#include "Animations.h"

enum class StateEnding
{
	RED,
	EARTHQUAKE,
	PEACE,
	FILM,
	CREDIT,
};

class Letter {
public :
	float x, y;
	char letter;
	Letter(float x, float y, char ch)
	{
		this->x = x;
		this->y = y;
		this->letter = ch;
	}
};

class CEndingScene : public CScene
{
	//Sanh code Input Sence
	StateEnding state;
	int ID_STATE; 
	int flag_EarthQuake = 0;
	int R = 255, G = 255, B = 255;
	float MountainX = 0, MountainY = 0;
	float posLetterX, posLetterY; 
	int timer = 0;
	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_STATE_ANIMATION(string line);
	void _ParseSection_OBJECT_ANIMATIONS(string line);
	map<char, int> mapLetter;
	vector <Letter> Paragraph;

public:
	void setState(StateEnding state);
	CObjectAnimationHanlders Film;
	CObjectAnimationHanlders Mountain;
	CObjectAnimationHanlders Red;
	CObjectAnimationHanlders Credit;
	CObjectAnimationHanlders TheEnd;

	void HandleKeyEnter();
	CEndingScene(int id, LPCWSTR filePath, int startupSection = -1);
	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	//Tool Credit
	void init_MapLetter();
	void add_LineCredit(string line);
	void init_LetterCredit();
	void render_DragonFollowCamera();
	void render_LetterCredit();
	void move_Camera();
	bool checkValidLetter(char & ch);
};

