#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//���Ԍv��
//-------------------------------------------------------------------
#include "../../../Engine/myGame/myGame.h"

namespace  TimeCount
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("���Ԍv��");	//�O���[�v��
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
		ML::Vec2 timePos;			//���Ԍv���\���ʒu
		ML::Size2 clock_size;		//���v�T�C�Y
		string    clock_filePath;	//���v�t�@�C���p�X��
		int		 digitMin;			//�ŏ��̌���


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
		ML::Image::SP clockImg;
	};

	//��������������������������������������������������������������������
	// �I�u�W�F�N�g
	//��������������������������������������������������������������������
	//-------------------------------------------------------------------
	class  Object : public  BTask
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
		void  UpDate()			override;//�u���s�v�P�t���[�����ɍs������
		void  Render3D_L0()		override;//�u3D�`��v�P�t���[�����ɍs�������A���C���[0
		void  Render2D_AF()		override;//�u2D�`��v�P�t���[�����ɍs�������A3D�`���ɍs����
		bool  Finalize();		//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	//�ύX������������������������������������������������������
	public:
		//�ǉ��������ϐ��E���\�b�h�͂����ɒǉ�����
		float	timeMax;		//��������
		float	timeCnt;		//���ԃJ�E���g�_�E��
		float	oldTime;		//���Ԍv���ϐ�
		bool	isDraw;			//�`�悷�邩
		bool	isCounting;		//�J�E���g���邩

		void CountTime();		//���Ԍv��
		void SetTimeMax(float timeMax_);	//�������Ԑݒ�
		bool IsTimeZero();		//���Ԃ�0�ɂȂ�����
		void SetDraw(bool isDraw_);		//�`�悷�邩
		void SetCounting(bool isCounting_);	//�J�E���g���邩
	};
}