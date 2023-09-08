#pragma once
#pragma comment(lib, "winmm.lib")


#include <windows.h>
#include <mmsystem.h>
#include <list>



using namespace std;



class FPSCounter
{
private:
	float _REFRESHRATE;
	float ONEFRAME_TIME;
	float		oldTime;			//1�t���[���O�̎��Ԃ��v��
	//�o�ߎ��Ԋϑ��p
	DWORD FpsTime;
	int FpsData;
	int FpsCnt;
	//FPS����p
	int _Time;	//�������Ԋi�[

public:
	float		deltaTime;			//�t���[���^�C��
	// FPS���X�V�@���t���[���Ăяo��
	double UpdateFPS();
	// FPS�l���擾�@���t���[���Ăяo��
	int GetFps();
	//FPS�\��
	void DrawFps();
	//�w��FPS�œ���
	bool LoopControl();

	FPSCounter(float);
	virtual ~FPSCounter(void);

};

