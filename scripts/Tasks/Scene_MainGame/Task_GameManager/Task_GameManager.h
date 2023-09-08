#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//�Q�[���{�ҊǗ�
//-------------------------------------------------------------------
#include "../../../Engine/myGame/myGame.h"
#include "../Task_TimeCount/Task_TimeCount.h"

namespace  GameManager
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("�Q�[���Ǘ�");	//�O���[�v��
	const  string  defName("NoName");	//�^�X�N��
	//��������������������������������������������������������������������
	// �Q�[���f�[�^
	//��������������������������������������������������������������������
	//-------------------------------------------------------------------
	class Data : public BData
	{
	public:
		using SP = shared_ptr<Data>;

		json LoadDataJson(bool useBinary_)						override;	//�e�L�X�g�f�[�^�ǂݍ���
		Data();			//�R���X�g���N�^�i���ɏ����Ȃ��j
		~Data();		//�f�X�g���N�^  �i���ɏ����Ȃ��j
		static Data::SP UseData();	//�Q�[���G���W������f�[�^���󂯎��
		static int myNum;			//�Q�[���G���W���ɂ���f�[�^�z��̒��ŁA���g�̗v�f�������ԍ�
		//�i�����g�p����ꍇ�AmyPG.cpp�̓ǂݍ��݊֐��ɂ��̃N���X��ǉ�����̂��Y�ꂸ�ɁI�����j 
		//-----------------------------
		//�f�[�^�ϐ��͈ȉ��ɒǉ�����
		//�v���C���[1�T�C�h
		ML::Vec3	player1pos;				//�v���C���[1�������W
		ML::Vec3	player1defaultAngle;	//�v���C���[1�̕W���̌���
		ML::Vec3	ground1pos;				//�n��1�̍��W
		float		border1posZ;			//��O����1��z���W
		string		border1checkArea;		//��O����1�̔���G���A�i"z+"��������"z-"�j
		//�v���C���[2�T�C�h
		ML::Vec3	player2pos;				//�v���C���[2�������W
		ML::Vec3	player2defaultAngle;	//�v���C���[2�̕W���̌���
		ML::Vec3	ground2pos;				//�n��2�̍��W
		float		border2posZ;			//��O����2��z���W
		string		border2checkArea;		//��O����2�̔���G���A�i"z+"��������"z-"�j

		//�v���C���[�ړ��\�͈�
		float			playerMoveAreaW;		//�v���C���[�ړ��\�͈́Q����
		float			playerMoveAreaD;		//�v���C���[�ړ��\�͈́Q���s

		float		beforeGameTime;	//�Q�[���O����
		float		gameTime;		//�Q�[����������

		//�G���A�\���i�f�o�b�O�j
		bool		drawMoveArea;			//�v���C���[�ړ��\�͈͕`��

		//-----------------------------
	};

	//��������������������������������������������������������������������
	// ���\�[�X
	//��������������������������������������������������������������������
	//-------------------------------------------------------------------
	class  Resource : public BResource
	{
		bool  Initialize()	override;
		bool  Finalize()	override;
		Resource();
	public:
		~Resource();
		typedef  shared_ptr<Resource>	SP;
		typedef  weak_ptr<Resource>		WP;
		Data::SP		data;
		static   WP  instance;
		static  Resource::SP  Create();
		//���L����ϐ��͂����ɒǉ�����
		
	};

	//��������������������������������������������������������������������
	// �I�u�W�F�N�g
	//��������������������������������������������������������������������
	//-------------------------------------------------------------------
	class  Object : public  GameBase
	{
	//�ύX�s����������������������������������������������������
	public:
		virtual  ~Object();
		typedef  shared_ptr<Object>		SP;
		typedef  weak_ptr<Object>		WP;
		//�������� ������true�Ń^�X�N�V�X�e���֎����o�^
		static  Object::SP  Create(bool flagGameEnginePushBack_);
		Resource::SP	res;
	private:
		Object();
		bool  B_Initialize();
		bool  B_Finalize();
		bool  Initialize();	//�u�������v�^�X�N�������ɂP�񂾂��s������
		void  UpDateBeforeGame()	override;//�u�Q�[���O�����v
		void  UpDateGame()			override;//�u���C���Q�[���v
		void  Render3D_L0()		override;//�u3D�`��v�P�t���[�����ɍs�������A���C���[0
		void  Render2D_AF()		override;//�u2D�`��v�P�t���[�����ɍs�������A3D�`���ɍs����
		bool  Finalize();		//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	//�ύX������������������������������������������������������
	public:
		//�ǉ��������ϐ��E���\�b�h�͂����ɒǉ�����
		TimeCount::Object::SP	beforeGameTimer;//	�Q�[���O���Ԍv��
		TimeCount::Object::SP	timer;		//	���C���Q�[�����Ԍv��

		ML::Font::SP debugFont;			//�f�o�b�O�p�t�H���g

		//�v���C���[���͈͊O�ɏo�Ă�����߂�����
		void CheckPlayerInArea(ML::Vec3& playerPos_, int playerNum_);
		//���C���Q�[���Ŏg�p����Ă���^�X�N��S�Ċ܂�vector��Ԃ�
		std::shared_ptr<std::vector<BTask::SP>> GetGameTask();
		//�Q�[���x�[�X�^�X�N�̃X�e�[�^�X�ؑ�
		void SetGameBaseTaskMode(StateOfGame state_);
	};
}