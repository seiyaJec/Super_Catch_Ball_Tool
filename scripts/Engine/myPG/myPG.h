#pragma once
#include "../myGame/myGame.h"
#include "useTaskInMYPG.h"
#include "../MyLibrary/MyLibrary.h"
#include <fstream>
#include <sstream>

//�Q�[�����S��Ŏg�p����\���̂Ȃǂ��`����
//-----------------------------------------------------------------------------------------------

#include "../fpscounter/fpscounter.h"
#include "../GameData/GameData.h"

//-----------------------------------------------------------------------------------------------
namespace  myPG
{
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	class myGameEngine : public myEngine
	{
	public:
		myGameEngine();
		//�Q�[���G���W���ɒǉ��������̂̏������ƊJ��
		bool Initialize();//(HWND wnd_);
		~myGameEngine();
		//�Q�[���G���W���ɒǉ��������̂̃X�e�b�v����
		void UpDate();
		//���͏��̍X�V����
		void UpDateInput();
		//3DPG1�Ή��ɂ��ǉ�
		//2D�`����̃f�t�H���g�p�����[�^�ݒ�
		void Set2DRenderState(DWORD l_);
		//3D�`����̃f�t�H���g�p�����[�^�ݒ�
		void Set3DRenderState(DWORD l_);


		//�Q�[���G���W���ɒǉ����������͉̂��ɉ�����
		//----------------------------------------------
		//������������������������
		//�ϐ�
		//������������������������
		const int camerasMax = 4;
		vector<bool> camerasActive;
		ML::Camera::SP*		cameras;		//	�J����

		GameManager::Object::SP		qa_gameManager;		//�Q�[���}�l�[�W���[�^�X�N
		Ball::Object::SP			qa_ball;			//�{�[���^�X�N
		NumberGUI::Object::SP		qa_numberGUI;		//�����`��^�X�N
		ScoreJudge::Object::SP		qa_scoreJudge;		//�X�R�A�^�X�N

		vector<BData::SP>		data;		//�Q�[���f�[�^
		const float				gravity = 0.3f;	//�Q�[���S�̂̏d��

		ML::InMouse					inpms;			//�}�E�X���͏��			
		ML::InKeyBoard				inpkey;			//�L�[�{�[�h���͏��


		//������������������������
		//�֐�
		//������������������������

		//�Q�[���f�[�^�ǂݍ���
		void CreateGameData();
		//�X�N���[���̒��S��Ԃ�
		ML::Vec2 screenCenter();
		

		//----------------------------------------------

		//�Q�[���f�[�^�ǉ�
		json gameData;

		void LoadGameDataJson();
		void LoadGameDataBson();
		

		//�f�o�b�O�֐�
		void printToDebugFile(std::string debugstr_);


		//��������������������������������������������������������������������
		//�ȉ�FPSCounter�֌W�i��{�ύX�s�v�j
		//��������������������������������������������������������������������
		FPSCounter* c;


		int stage;

		enum COUNTER_FLAGS {
			NONE,
			ACTIVE,//�ғ���
			LIMIT,//�I������
			END,//�I��
		};

		struct COUNTER {
			COUNTER_FLAGS flag;
			int count;
		};
		std::map < std::string, COUNTER > counterMap;


		void InitCounter();
		void ActCounter();
		void AllEndCounter();
		COUNTER_FLAGS getCounterFlag(string str);
		void StartCounter(string str, int count);


	};
}
extern myPG::myGameEngine* ge;


