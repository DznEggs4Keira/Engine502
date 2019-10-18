#include "Sound.h"

#ifdef _XBOX //Big-Endian
#define fourccRIFF 'RIFF'
#define fourccDATA 'data'
#define fourccFMT 'fmt '
#define fourccWAVE 'WAVE'
#define fourccXWMA 'XWMA'
#define fourccDPDS 'dpds'
#endif

#ifndef _XBOX //Little-Endian
#define fourccRIFF 'FFIR'
#define fourccDATA 'atad'
#define fourccFMT ' tmf'
#define fourccWAVE 'EVAW'
#define fourccXWMA 'AMWX'
#define fourccDPDS 'sdpd'
#endif

Sound::Sound()
{
	pXAudio2 = nullptr;
	pMasterVoice = nullptr;
	pSourceVoice = nullptr;
}

Sound::Sound(const Sound&)
{
}

Sound::~Sound()
{
	// Shutdown the XAudio2 API.
	Shutdown();
}


#pragma region Init

//public function
bool Sound::Initialize(HWND hwnd)
{
	HRESULT hr;
	if (FAILED(hr = XAudio2Create(&pXAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR)))
		return hr;

	if (FAILED(hr = pXAudio2->CreateMasteringVoice(&pMasterVoice)))
		return hr;

	bool result;

	result = InitializeXAudio2(hwnd);
	if (!result)
	{
		return false;
	}

	if(FAILED(hr = PlaySoundFile()))
		return hr;

	return true;
}

//private functions
bool Sound::InitializeXAudio2(HWND hwnd)
{
	// Open the audio file with CreateFile.
	LPCWSTR strFileName = L"../Engine/data/Sounds/Relaxing_Music.wav";

	// Open the file
	HANDLE hFile = CreateFile(
								strFileName,
								GENERIC_READ,
								FILE_SHARE_READ,
								NULL,
								OPEN_EXISTING,
								0,
								NULL);

	if (INVALID_HANDLE_VALUE == hFile)
		return HRESULT_FROM_WIN32(GetLastError());

	if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN))
		return HRESULT_FROM_WIN32(GetLastError());

	// Locate the 'RIFF' chunk in the audio file, and check the file type.
	DWORD dwChunkSize;
	DWORD dwChunkPosition;

	//check the file type, should be fourccWAVE or 'XWMA'
	FindChunk(hFile, fourccRIFF, dwChunkSize, dwChunkPosition);

	DWORD filetype;
	ReadChunkData(hFile, &filetype, sizeof(DWORD), dwChunkPosition);
	if (filetype != fourccWAVE)
		return S_FALSE;

	// Locate the 'fmt ' chunk, and copy its contents into a WAVEFORMATEXTENSIBLE structure.
	FindChunk(hFile, fourccFMT, dwChunkSize, dwChunkPosition);
	ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);

	// Locate the 'data' chunk, and read its contents into a buffer.
	//fill out the audio data buffer with the contents of the fourccDATA chunk
	FindChunk(hFile, fourccDATA, dwChunkSize, dwChunkPosition);
	BYTE * pDataBuffer = new BYTE[dwChunkSize];
	ReadChunkData(hFile, pDataBuffer, dwChunkSize, dwChunkPosition);

	// Populate an XAUDIO2_BUFFER structure.
	buffer.AudioBytes = dwChunkSize;  //buffer containing audio data
	buffer.pAudioData = pDataBuffer;  //size of the audio buffer in bytes
	buffer.Flags = XAUDIO2_END_OF_STREAM; // tell the source voice not to expect any data after this buffer
	buffer.LoopCount = XAUDIO2_LOOP_INFINITE; // tell the source voice to loop the audio
}

HRESULT Sound::FindChunk(HANDLE hFile, DWORD fourcc, DWORD & dwChunkSize, DWORD & dwChunkDataPosition)
{
	HRESULT hr = S_OK;
	if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN))
		return HRESULT_FROM_WIN32(GetLastError());

	DWORD dwChunkType;
	DWORD dwChunkDataSize;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType;
	DWORD bytesRead = 0;
	DWORD dwOffset = 0;

	while (hr == S_OK)
	{
		DWORD dwRead;
		if (0 == ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL))
			hr = HRESULT_FROM_WIN32(GetLastError());

		if (0 == ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL))
			hr = HRESULT_FROM_WIN32(GetLastError());

		switch (dwChunkType)
		{
		case fourccRIFF:
			dwRIFFDataSize = dwChunkDataSize;
			dwChunkDataSize = 4;
			if (0 == ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL))
				hr = HRESULT_FROM_WIN32(GetLastError());
			break;

		default:
			if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT))
				return HRESULT_FROM_WIN32(GetLastError());
		}

		dwOffset += sizeof(DWORD) * 2;

		if (dwChunkType == fourcc)
		{
			dwChunkSize = dwChunkDataSize;
			dwChunkDataPosition = dwOffset;
			return S_OK;
		}

		dwOffset += dwChunkDataSize;

		if (bytesRead >= dwRIFFDataSize) return S_FALSE;

	}

	return S_OK;

}

HRESULT Sound::ReadChunkData(HANDLE hFile, void * buffer, DWORD buffersize, DWORD bufferoffset)
{
	HRESULT hr = S_OK;
	if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, bufferoffset, NULL, FILE_BEGIN))
		return HRESULT_FROM_WIN32(GetLastError());
	DWORD dwRead;
	if (0 == ReadFile(hFile, buffer, buffersize, &dwRead, NULL))
		hr = HRESULT_FROM_WIN32(GetLastError());
	return hr;
}

#pragma endregion

#pragma region Play

bool Sound::PlaySoundFile()
{
	bool result;
	HRESULT hr;

	if (FAILED(hr = pXAudio2->CreateSourceVoice(&pSourceVoice, (WAVEFORMATEX*)&wfx))) 
		return hr;

	if (FAILED(hr = pSourceVoice->SubmitSourceBuffer(&buffer)))
		return hr;

	if (FAILED(hr = pSourceVoice->Start(0)))
		return hr;
}

#pragma endregion


#pragma region Shutdown

void Sound::Shutdown()
{
	pXAudio2 = nullptr;
	pMasterVoice = nullptr;
	pSourceVoice = nullptr;
	buffer = { 0 };
}

#pragma endregion



