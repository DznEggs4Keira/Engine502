
#ifndef _SOUNDCLASS_H_
#define _SOUNDCLASS_H_

// INCLUDES
#include <windows.h>
#include <mmsystem.h>
#include <dsound.h>
#include <stdio.h>

//sound is called in the system class

class Sound
{
private:

	//.WAV audio struct header
	//try implementing MP3 later --- if time

	struct WaveHeaderType
	{
		char chunkId[4]; /* char is 1 byte but array is 4 (0 - 4)*/
		unsigned long chunkSize; /*long is 4 (4 - 8)*/
		char format[4]; /*8 - 12*/
		char subChunkId[4]; /*12 - 16*/
		unsigned long subChunkSize; /*16 - 20*/
		unsigned short audioFormat; /* short is 2 bytes 20 - 22*/
		unsigned short numChannels; /*22 - 24*/
		unsigned long sampleRate; /*24 - 28*/
		unsigned long bytesPerSecond; /*28 - 32*/
		unsigned short blockAlign; /*32 - 34*/
		unsigned short bitsPerSample; /*34 - 36*/
		char dataChunkId[4]; /*36 - 40*/
		unsigned long dataSize; /*40 - 44*/
	};


public:
	//constructors
	Sound();
	Sound(const Sound&);

	//destructors
	~Sound();

	//initialise will create everything needed for sound
	bool Initialize(HWND);

	//shutdown will release everything
	void Shutdown();

private:

	//private functions to load direct sound, the audio files and use them as needed accordingly
	bool InitializeDirectSound(HWND);
	void ShutdownDirectSound();

	bool LoadWaveFile(char*, IDirectSoundBuffer8**);
	void ShutdownWaveFile(IDirectSoundBuffer8**);

	bool PlayWaveFile();

private:

	//we need two buffers for sound
	//a primary one and a secondary one
	IDirectSound8* m_DirectSound;
	IDirectSoundBuffer* m_primaryBuffer;
	IDirectSoundBuffer8* m_secondaryBuffer1;
};
#endif
