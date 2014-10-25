#include "wave2data.h"

Wave2data::Wave2data()
{
	ZeroMemory(&this->wFmt, sizeof(WAVEFORMATEX));
}

Wave2data::~Wave2data()
{
	mmioClose(this->hMmio, 0);
}

// mmio�֐��Q��p����Wave�t�@�C������f�[�^�����o��
HRESULT Wave2data::InitMMio(const char* fn) {
	using namespace std;
	MMIOINFO mmioInfo;
	MMCKINFO riffChunk;
	MMRESULT mmRes;
	DWORD formatSize, formatReadSize;

	// Wave�t�@�C���I�[�v��
	ZeroMemory(&mmioInfo, sizeof(MMIOINFO));
	this->hMmio = mmioOpen((LPSTR)fn, &mmioInfo, MMIO_READ);
	if (!this->hMmio) {
		 printf("%s: Failed Open\n", fn);
		return E_FAIL;
	}
	// RIFF�`�����N����
	riffChunk.fccType = mmioFOURCC('W', 'A', 'V', 'E'); // �`�����N�̎��: WAVE
	mmRes = mmioDescend(this->hMmio, &riffChunk, NULL, MMIO_FINDRIFF);
	if (mmRes != MMSYSERR_NOERROR) {
		printf("Failed: RIFF��񌟍�\n");
		return E_FAIL;
	}
	// �t�H�[�}�b�g�`�����N����
	this->formatChunk.ckid = mmioFOURCC('f', 'm', 't', ' ');
	mmRes = mmioDescend(this->hMmio, &this->formatChunk, &riffChunk, MMIO_FINDCHUNK);
	if (mmRes != MMSYSERR_NOERROR) {
		printf("Failed: �t�H�[�}�b�g�`�����N��񌟍�\n");
		return E_FAIL;
	}
	// WAVEFORMATEX�\���̎擾
	formatSize = this->formatChunk.cksize;
	formatReadSize = mmioRead(this->hMmio, (HPSTR)&this->wFmt, formatSize);
	if (formatSize != formatReadSize) {
		printf("Failed: WAVEFORMATEX�\���̊i�[\n");
		return E_FAIL;
	}
	// RIFF�`�����N�ɖ߂�
	mmioAscend(this->hMmio, &this->formatChunk, 0);

	// �f�[�^�`�����N����
	this->dataChunk.ckid = mmioFOURCC('d', 'a', 't', 'a');
	mmRes = mmioDescend(this->hMmio, &this->dataChunk, &riffChunk, MMIO_FINDCHUNK);
	if (mmRes != MMSYSERR_NOERROR) {
		printf("Failed: �f�[�^�`�����N����\n");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT Wave2data::Update(const char* output)
{
	auto data = new char[this->dataChunk.cksize];

	if (FAILED(this->readData(data))) {
		delete[] data;
		return E_FAIL;
	}

	this->outputData(output, data, sizeof(char), this->dataChunk.cksize);

	delete[] data;

	this->writeLog(output);

	return S_OK;
}

HRESULT Wave2data::readData(char* ret)
{
	LONG count = 0;

	mmioSeek(this->hMmio, this->dataChunk.dwDataOffset, SEEK_SET);
	count = mmioRead(this->hMmio, (HPSTR)ret, this->dataChunk.cksize);

	if (count == -1) {
		printf("Failed: �ȃf�[�^�ǂݏo��\n");
		return E_FAIL;
	}

	return S_OK;
}

void Wave2data::outputData(const char* fn, const void* _Str, size_t _Size, size_t _Count) const
{
	FILE* f;
	fopen_s(&f, fn, "wb");
	fwrite(_Str, _Size, _Count, f);
	fclose(f);
}

void Wave2data::writeLog(const char* output) const
{
	printf("Data Size: %d\n", this->dataChunk.cksize);
	printf("Output: %s\n", output);
}
