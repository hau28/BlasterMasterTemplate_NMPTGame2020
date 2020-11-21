#include "SophiaAnimationSystem.h"
#include "Textures.h"

const string SOPHIA_INPUT_TEXTURE = "[TEXTURE]";
const string SOPHIA_INPUT_DIMENSIONS = "[DIMENSIONS]";
const string SOPHIA_INPUT_SPRITES = "[SPRITES]";
const string SOPHIA_INPUT_SEQUENCES = "[SEQUENCES]";

CSophiaAnimationSystem* CSophiaAnimationSystem::__instance = nullptr;

void CSophiaAnimationSystem::_ParseSection_TEXTURE(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() != 2) return; // skip invalid lines

	numberOfRowTexture = atoi(tokens[0].c_str());
	numberOfColumnTexture = atoi(tokens[1].c_str());

	texSophia = CTextures::GetInstance()->Get(TEXTURE_SOPHIA);

	// getting the size of texture
	D3DSURFACE_DESC surfaceDesc;
	texSophia->GetLevelDesc(0, &surfaceDesc);

	spriteWidth = surfaceDesc.Width / numberOfColumnTexture;
	spriteHeight = surfaceDesc.Height / numberOfRowTexture;
}

void CSophiaAnimationSystem::_ParseSection_DIMENSIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() != 4) return; // skip invalid lines

	directionDimension = atoi(tokens[0].c_str());
	gunDimension = atoi(tokens[1].c_str());
	bodyDimension = atoi(tokens[2].c_str());
	wheelDimension = atoi(tokens[3].c_str());
}

void CSophiaAnimationSystem::_ParseSection_SPRITES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() != 6) return; // skip invalid lines
	
	int row = atoi(tokens[0].c_str());
	int col = atoi(tokens[1].c_str());
	int directionState = atoi(tokens[2].c_str());
	int gunState = atoi(tokens[3].c_str());
	int bodyState = atoi(tokens[4].c_str());
	int wheelState = atoi(tokens[5].c_str());

	int t = row * spriteHeight;
	int l = col * spriteWidth;
	int b = t + spriteHeight - 1;
	int r = l + spriteWidth - 1;
	int id = EncodeDimensions(directionState, gunState, bodyState, wheelState);

	LPSPRITE sprite = new CSprite(id, l, t, r, b, texSophia);
	sprites[id] = sprite;

	DebugOut(L"Sophia sprite added with id: %d = (%d, %d, %d, %d)\n", id, directionState, gunState, bodyState, wheelState);
}

void CSophiaAnimationSystem::_ParseSection_SEQUENCES(string line)
{
	// Hau & Thao to do...
}

int CSophiaAnimationSystem::EncodeDimensions(int directionState, int gunState, int bodyState, int wheelState)
{
	int result = directionState;
	result = result * gunDimension + gunState;
	result = result * bodyDimension + bodyState;
	result = result * wheelDimension + wheelState;

	return result;
}

LPSPRITE CSophiaAnimationSystem::GetSprite(int combinedState)
{
	return sprites[combinedState];
}

void CSophiaAnimationSystem::Input(LPCWSTR filePath)
{
	DebugOut(L"[INFO] Start loading SOPHIA from %s \n", filePath);

	ifstream f;
	f.open(filePath);

	string currentTag = "[UNKNOWN]";

	const int MAX_CHAR_IN_LINE = 1024;
	char str[MAX_CHAR_IN_LINE];
	while (f.getline(str, MAX_CHAR_IN_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line[0] == '[') { currentTag = line; continue; }

		//
		// data section
		//
		if (currentTag == SOPHIA_INPUT_TEXTURE) { _ParseSection_TEXTURE(line); continue; }
		if (currentTag == SOPHIA_INPUT_DIMENSIONS) { _ParseSection_DIMENSIONS(line); continue; }
		if (currentTag == SOPHIA_INPUT_SPRITES) { _ParseSection_SPRITES(line); continue; }
		if (currentTag == SOPHIA_INPUT_SEQUENCES) { _ParseSection_SEQUENCES(line); continue; }
	}

	f.close();

	//CTextures::GetInstance()->Add(ID_TEX_BBOX, L"textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));

	DebugOut(L"[INFO] Done loading scene resources %s\n", filePath);
}

LPSPRITE CSophiaAnimationSystem::GetSprite(int directionState, int gunState, int bodyState, int wheelState)
{
	int combinedState = EncodeDimensions(directionState, gunState, bodyState, wheelState);
	return GetSprite(combinedState);
}

CSophiaAnimationSystem* CSophiaAnimationSystem::GetInstance()
{
	if (__instance == nullptr)
	{
		__instance = new CSophiaAnimationSystem();

		// CuteTN Note: putting this initialization here is very very dirty...
		__instance->Input(L"SOPHIA_input.txt");
	}

	return __instance;
}
