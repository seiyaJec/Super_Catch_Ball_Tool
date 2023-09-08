// FPS�v���N���X�����t�@�C��

#include "fpscounter.h"


void FPSCounter::DrawFps()
{
	//FPS�J�E���g
	FpsCnt++;
	//���A���t���[�����v�Z
	if (timeGetTime() >= (unsigned)FpsTime) {
		FpsData = FpsCnt;
		FpsCnt = 0;
		FpsTime = (timeGetTime() + 1000);
	}

	char Buffer[1024];
	sprintf_s(Buffer, "Spe fps = %0.2f : real fps = %3d : deltaTime=%0.4f", _REFRESHRATE, FpsData, deltaTime);
	SetWindowText(GetActiveWindow(), (LPCSTR)Buffer);
}

bool FPSCounter::LoopControl() {
	float _FPS = 1000.0f / static_cast<float>(_REFRESHRATE);					//FPS�v�Z�p�o�b�t�@

	UpdateFPS();

	if (_REFRESHRATE != 0) {
		//				//�^����ꂽFPS����FrameTime���v�Z	
		if (timeGetTime() - _Time >= (unsigned)_FPS) {		//FrameTime���o�߂���܂ő҂�
			_Time = timeGetTime();
			return 1;
		}
		return 0;
	}
	else {
		return 1;
	}
}

int FPSCounter::GetFps()
{
	return (int)_REFRESHRATE;
}

// �R���X�g���N�^
FPSCounter::FPSCounter(float fps)
{
	// ���x���グ��
	timeBeginPeriod(1);

	_REFRESHRATE = fps;
	ONEFRAME_TIME = 1.0f / _REFRESHRATE;

	FpsTime = (timeGetTime() + 1000);
	FpsData = 0.0f;
	FpsCnt = 1;

	//FPS����p
	_Time = timeGetTime();	//�������Ԋi�[

	deltaTime = (timeGetTime() - oldTime) / 1000.f;
	oldTime = (float)timeGetTime();
}

// �f�X�g���N�^
FPSCounter::~FPSCounter(void)
{
	timeEndPeriod(1);    // �^�C�}�[�̐��x��߂�
}

// FPS���X�V
double FPSCounter::UpdateFPS()
{
	deltaTime = (timeGetTime() - oldTime) / 1000.f;
	oldTime = timeGetTime();
	return deltaTime;
}


//
////float		FrameTime;
////const int	REFRESHRATE = 60;
////const float	ONEFRAME_TIME = 1.0f / REFRESHRATE;
////---------------------------------------------------------------------------------------
//// ���[�v����
////---------------------------------------------------------------------------------------
//bool MyGameEngine::LoopControl(int FPS) {
//	if (FPS != 0) {
//		_FPS = 1000.0f / static_cast<float>(FPS);		//�^����ꂽFPS����FrameTime���v�Z	
//		if (timeGetTime() - _Time > (unsigned)_FPS) {		//FrameTime���o�߂���܂ő҂�
//			_Time = timeGetTime();
//			return 1;
//		}
//		return 0;
//	}
//	else {
//		return 1;
//	}
//}
//
//void MyGameEngine::Draw_ShowFps(int x, int y)
//{
//	static DWORD FpsTime = (timeGetTime() + 1000);
//	static double FpsData = 0.0;
//	static int FpsCnt = 1;
//	//1�b�o�߂ɂ��fps�̌v��
//	if (timeGetTime() > (unsigned)FpsTime) {
//		FpsData = FpsCnt;
//		FpsCnt = 0;
//		FpsTime = (timeGetTime() + 1000);
//	}
//	//FPS�J�E���g
//	FpsCnt++;
//	char Buffer[1024];
//	sprintf_s(Buffer, "fps=%3.2f:dt=%0.3f", FpsData, ge->deltaTime);
//	ML::Box2D draw(x, y, 480, 30);
//
//	//		this->debugFont->Draw(draw, Buffer, ML::Color(1, 1, 1, 1));
//	Dbg_ToTitle((LPCSTR)Buffer);
//}


