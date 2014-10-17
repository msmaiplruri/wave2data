#pragma once

class Wave2data {
private:
	HMMIO hMmio;
	WAVEFORMATEX wFmt;
	MMCKINFO formatChunk, dataChunk;

	HRESULT readData(char* ret);
	void outputData(const char* fn, const void* _Str, size_t _Size, size_t _Count) const;
	void writeLog(const char* output) const;
public:
	Wave2data();
	virtual ~Wave2data();

	HRESULT InitMMio(const char* fn);
	HRESULT Update(const char* output);
};