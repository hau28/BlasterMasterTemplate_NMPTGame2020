#include "Sound.h"
#include "Utils.h"
#include "Game.h"

Sound* Sound::instance = nullptr;

Sound* Sound::getInstance()
{
	if (instance == nullptr) {
		instance = new Sound(CGame::GetInstance()->getCurrentHWND());
		instance->loadGameSounds();
	}

	return instance;
}

Sound::Sound(HWND hWnd)
{
	primaryBuffer = 0;
	HRESULT result;

	DSBUFFERDESC bufferDesc; //describe the buffer

	result = DirectSoundCreate8(NULL, &pDevice, NULL);

	if (FAILED(result))
	{
		DebugOut(L"Can not create device");
	}

	result = pDevice->SetCooperativeLevel(hWnd, DSSCL_PRIORITY); // set the cooperative level.

	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME;
	bufferDesc.dwBufferBytes = 0;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = NULL;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

	result = pDevice->CreateSoundBuffer(&bufferDesc, &primaryBuffer, NULL);

	if (FAILED(result))
	{
		DebugOut(L"Can not create primaryBuffer");
	}
	volume = 100.0f;
	isMute = false;
}

void Sound::loadGameSounds() {
	Sound::getInstance()->loadSound((char*)"Sound/intro.wav", "intro");
	Sound::getInstance()->loadSound((char*)"Sound/enter.wav", "enter");
	Sound::getInstance()->loadSound((char*)"Sound/area2.wav", "area2");
	Sound::getInstance()->loadSound((char*)"Sound/credit.wav", "credit");
	Sound::getInstance()->loadSound((char*)"Sound/earthquake.wav", "earthquake");
	Sound::getInstance()->loadSound((char*)"Sound/peace.wav", "peace");
	//Sound::getInstance()->play("intro", false, 1);

	//bullet sounds
	Sound::getInstance()->loadSound((char*)"Sound/small_pink_bullet_to_wall.wav", "small_pink_bullet_to_wall");

	// player sound
	Sound::getInstance()->loadSound((char*)"Sound/sophia_fall_ground.wav", "sophia_fall_ground");
	Sound::getInstance()->loadSound((char*)"Sound/jump.wav", "sophia_jump");
	Sound::getInstance()->loadSound((char*)"Sound/sophia_bullet_explosion.wav", "sophia_bullet_explosion");
	Sound::getInstance()->loadSound((char*)"Sound/sophia_shoot.wav", "sophia_shoot");
	Sound::getInstance()->loadSound((char*)"Sound/sophia_explosion.wav", "sophia_explosion");
	Sound::getInstance()->loadSound((char*)"Sound/jason_got_hit.wav", "jason_got_hit");
	Sound::getInstance()->loadSound((char*)"Sound/jason_sideview_shoot.wav", "jason_sideview_shoot");
	Sound::getInstance()->loadSound((char*)"Sound/bullet_explosion.wav", "bullet_explosion");
	Sound::getInstance()->loadSound((char*)"Sound/swap_player.wav", "swap_player");
	Sound::getInstance()->loadSound((char*)"Sound/enemy_die.wav", "enemy_die");
	Sound::getInstance()->loadSound((char*)"Sound/scene_change.wav", "scene_change");
	Sound::getInstance()->loadSound((char*)"Sound/lava.wav", "lava");
	Sound::getInstance()->loadSound((char*)"Sound/thunder.wav", "thunder");
	Sound::getInstance()->loadSound((char*)"Sound/multiwarhead.wav", "multiwarhead");

	// enemies
	Sound::getInstance()->loadSound((char*)"Sound/worm_moving.wav", "worm_moving");
	Sound::getInstance()->loadSound((char*)"Sound/insect_fly_down.wav", "insect_fly_down");
	Sound::getInstance()->loadSound((char*)"Sound/enemy_jump.wav", "enemy_jump");
	Sound::getInstance()->loadSound((char*)"Sound/skull_bomb.wav", "skull_bomb");
	Sound::getInstance()->loadSound((char*)"Sound/mine.wav", "mine");
	Sound::getInstance()->loadSound((char*)"Sound/dome_jump.wav", "dome_jump");
	Sound::getInstance()->loadSound((char*)"Sound/teleport.wav", "teleport");
	Sound::getInstance()->loadSound((char*)"Sound/teleporter_shoot.wav", "teleporter_shoot");
	Sound::getInstance()->loadSound((char*)"Sound/entering_boss_scene.wav", "entering_boss_scene");

	Sound::getInstance()->setVolume(85, "");
	//Sound::getInstance()->setVolume(90, "area2");
	Sound::getInstance()->setVolume(90, "sophia_explosion");
	Sound::getInstance()->setVolume(90, "sophia_fall_ground");
	Sound::getInstance()->setVolume(90, "sophia_bullet_explosion");
	Sound::getInstance()->setVolume(90, "lava");
}

Sound::~Sound()
{
	for (auto it = soundBufferMap.begin(); it != soundBufferMap.end(); it++)
	{
		if (it->second)
		{
			it->second->Release();
			it->second = NULL;
		}

	}
	if (primaryBuffer)
		primaryBuffer->Release();
	if (pDevice)
		pDevice->Release();
	pDevice = NULL;

	primaryBuffer = NULL;


}

void Sound::create(HWND hWnd)
{
	if (instance == nullptr)
	{
		instance = new Sound(hWnd);
	}
}

float Sound::getVolume()
{
	return volume;
}

void Sound::loadSound(char* fileName, std::string name)
{
	if (soundBufferMap.find(name) != soundBufferMap.end())
		return;
	FILE* filePtr;
	WaveHeaderStruct waveHeaderStruct;
	IDirectSoundBuffer* tempBuffer;
	DSBUFFERDESC bufferDesc;
	WAVEFORMATEX waveFormat;
	unsigned char* wavData;
	unsigned char* bufferPtr;
	unsigned long bufferSize;

	int error = fopen_s(&filePtr, fileName, "rb");
	if (error != 0)
	{
		DebugOut(L" Can not load: %s", fileName);
		return;
	}

	fread(&waveHeaderStruct, sizeof(WaveHeaderStruct), 1, filePtr);
	//fread(&waveFileHeader, sizeof(waveFileHeader), 1, filePtr);

	if ((waveHeaderStruct.format[0] != 'W') || (waveHeaderStruct.format[1] != 'A') ||
		(waveHeaderStruct.format[2] != 'V') || (waveHeaderStruct.format[3] != 'E'))
	{
		DebugOut(L" file format does not support: %s", fileName);
	}

	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = waveHeaderStruct.sampleRate;
	waveFormat.wBitsPerSample = waveHeaderStruct.bitsPerSample;
	waveFormat.nChannels = waveHeaderStruct.numChannels;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;

	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_CTRLVOLUME | DSBCAPS_GLOBALFOCUS;
	bufferDesc.dwBufferBytes = waveHeaderStruct.dataSize;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = &waveFormat;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

	IDirectSoundBuffer8* secondaryBuffer = 0;
	IDirectSoundBuffer8** pSecondaryBuffer = &secondaryBuffer;
	//IDirectSoundBuffer8

	//long result = pDevice->CreateSoundBuffer(&bufferDesc, &secondaryBuffer, NULL);

	pDevice->CreateSoundBuffer(&bufferDesc, &tempBuffer, NULL);

	long result = tempBuffer->QueryInterface(IID_IDirectSoundBuffer8, (void**)&(*pSecondaryBuffer));

	tempBuffer->Release();
	tempBuffer = 0;

	if (FAILED(result))
	{
		DebugOut(L" Can not create secondaryBuffer ");
		return;
	}

	//fseek(filePtr, sizeof(WaveHeaderStruct), SEEK_SET); // move the filePointer cursor to data section

	fseek(filePtr, sizeof(WaveHeaderStruct), SEEK_SET);


	wavData = new unsigned char[waveHeaderStruct.dataSize];

	fread(wavData, waveHeaderStruct.dataSize, 1, filePtr);

	error = fclose(filePtr);
	if (error != 0)
	{
		DebugOut(L" Can not close file ");
	}

	result = (*pSecondaryBuffer)->Lock(0, waveHeaderStruct.dataSize, (void**)&bufferPtr, (DWORD*)&bufferSize, NULL, 0, 0);

	if (FAILED(result))
	{
		return;
	}

	memcpy(bufferPtr, wavData, waveHeaderStruct.dataSize);

	(*pSecondaryBuffer)->Unlock((void*)bufferPtr, bufferSize, NULL, 0);

	if (wavData != NULL)
		delete wavData;
	wavData = 0;
	long tempVolume = (volume) / 100 * (-DSBVOLUME_MIN) + DSBVOLUME_MIN;
	(*pSecondaryBuffer)->SetVolume(tempVolume);

	soundBufferMap[name] = secondaryBuffer;


}

void Sound::play(std::string name, bool infiniteLoop, int times)
{
	if (isMute)
	{
		return;
	}
	std::map< std::string, IDirectSoundBuffer8*> ::iterator it;
	it = soundBufferMap.find(name);
	if (it == soundBufferMap.end())
		return;
	if (infiniteLoop)
	{
		it->second->Play(0, 0, DSBPLAY_LOOPING);
	}
	else
	{
		it->second->Stop();
		it->second->SetCurrentPosition(0);
		it->second->Play(0, 0, times - 1);
	}

}

void Sound::stop(std::string name)
{
	if (name == "")
	{
		for (std::map< std::string, IDirectSoundBuffer8*> ::iterator it = soundBufferMap.begin(); it != soundBufferMap.end(); it++)
		{
			it->second->Stop();
			it->second->SetCurrentPosition(0);
		}
	}
	else
	{
		std::map< std::string, IDirectSoundBuffer8*> ::iterator it;
		it = soundBufferMap.find(name);
		if (it == soundBufferMap.end())
			return;
		else it->second->Stop();
	}
}

void Sound::setVolume(float percentage, std::string name)
{
	//volume = percentage;
	if (name == "")
	{
		long volumne = (percentage) / 100 * (-DSBVOLUME_MIN) + DSBVOLUME_MIN;
		for (std::map< std::string, IDirectSoundBuffer8*> ::iterator it = soundBufferMap.begin(); it != soundBufferMap.end(); it++)
		{
			it->second->SetVolume(volumne);
		}
	}
	else
	{
		std::map< std::string, IDirectSoundBuffer8*> ::iterator it;
		it = soundBufferMap.find(name);
		if (it == soundBufferMap.end())
			return;
		long volumne = (percentage) / 100 * (-DSBVOLUME_MIN) + DSBVOLUME_MIN;
		it->second->SetVolume(volumne);
	}
}

void Sound::mute()
{
	isMute = true;
	Sound::getInstance()->stop();
}

void Sound::unMute()
{
	isMute = false;
}

void Sound::cleanUp()
{
	delete this;
}
