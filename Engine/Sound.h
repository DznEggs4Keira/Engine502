
#ifndef _SOUNDCLASS_H_
#define _SOUNDCLASS_H_

// INCLUDES
#include <windows.h>
#include <mmsystem.h>

#include <d3d11.h>
#include <xaudio2.h>

//sound is called in the system class

class Sound
{
public:
	//constructors
	Sound();
	Sound(const Sound&);

	//destructors
	~Sound();

	//initialise will create everything needed for sound
	bool Initialize(HWND);

private:

	//private functions to load direct sound, the audio files and use them as needed accordingly
	HRESULT FindChunk(HANDLE, DWORD, DWORD & , DWORD & );
	HRESULT ReadChunkData(HANDLE, void *, DWORD , DWORD );

	bool InitializeXAudio2(HWND);
	
	bool PlaySoundFile();

	void Shutdown();

private:

	IXAudio2* pXAudio2;
	IXAudio2MasteringVoice* pMasterVoice;

	WAVEFORMATEXTENSIBLE wfx = { 0 };
	XAUDIO2_BUFFER buffer = { 0 };

	IXAudio2SourceVoice* pSourceVoice;
};
#endif
