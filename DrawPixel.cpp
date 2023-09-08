#include "DxLib.h"
#include "scripts/Engine/myPG/myPG.h"
#include <iostream>
#include <map>
#include <string>
#include <tuple>

// �v���O������ WinMain ����n�܂�܂�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	SetWindowSizeExtendRate(1.0);
	SetFontSize(60);
	ChangeWindowMode(TRUE);
	SetGraphMode(1920, 1080, 32);
	DxLib::SetUseDirect3D11AdapterIndex(0);

	if (DxLib_Init() == -1)		// �c�w���C�u��������������
	{
		return -1;			// �G���[���N�����璼���ɏI��
	}



	//�[�[�[�[�[�[�[�[�������烁�C���������s���[�[�[�[�[�[�[�[//
	SetDrawScreen(DX_SCREEN_BACK);


	ge = new myPG::myGameEngine();
	ge->B_Initialize();
	SetGlobalAmbientLight(ML::Color(1, 0.5f, 0.5f, 0.5f));
	SetLightDifColor(ML::Color(1, 1, 1, 1));
	SetLightAmbColor(ML::Color(1, 0, 0, 0));

	//�Q�[�������[�v�iesc�L�[�ŏI���j
	while (ge->FinishGame() == false) {
		if (ge->c->LoopControl()) {
			ge->B_Step();		//�Q�[������
			ProcessMessage();
		}

	}

	delete ge;



	//�[�[�[�[�[�[�[�[�[�[�[���C�������I���[�[�[�[�[�[�[�[�[�[//


	DxLib_End();				// �c�w���C�u�����g�p�̏I������

	return 0;				// �\�t�g�̏I�� 
}
