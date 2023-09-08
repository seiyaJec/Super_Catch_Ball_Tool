#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//�N���G�C�^�[
//-------------------------------------------------------------------
#include "../../../Engine/myGame/myGame.h"

namespace  Creator
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("�N���G�C�^�[");	//�O���[�v��
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
		ML::Model3D::SP		targetSampleSideR;				//�I�T���v����
		ML::Model3DClone::SP	targetSampleSideL;			//�I�T���v���E
		ML::Model3DClone::SP	targetSampleMoveRot;	//�I�T���v�������ʒu
		ML::Color			sampleColorSide;	//�T���v���[�̐F
		ML::Color			sampleColorRot;	//�T���v�����̐F
		ML::Vec3 pos;				//���W
		ML::Vec3		ptPos;		//�|�C���^�[�̍��W
		float			   ptHeight;		//�|�C���^�[��y���̍����i�ݒu����I�̍����ɂȂ�j
		float	 moveSpeed;			//�J�����ړ����x
		ML::Rect2		moveArea;	//�I���ړ�������͈�
		ML::Obb3				hitBaseMs;				//�}�E�X�̓����蔻�蒼����
		ML::Camera::SP		myCamera;			//���g�����J����
		Target::Object::SP movingTarget;		//�ړ����̓I

		int tgMoveTime;					//�I����������悤�ɂȂ钷��������
		int msBtOnCnt;					//�}�E�X�{�^�����������Ԍv��

		float defaultTgMoveSpeed;		//�����̈ړ����x
		float defaultTgMoveScale;		//�����̈ړ���
		float defaultTgAddRot;			//�����̉�]��



		ML::Obb3 CallHitBoxMs() const;	//�����蔻��Ăяo��
		bool IsMouseInMoveArea();		//�I�ړ��\�͈͂ɃJ�[�\�������邩
		void Move();					//�ړ�
		void TargetSetting();			//�I�̐ݒ�
		void TargetLinkToMouse();		//�I�̍��W���}�E�X�Ɠ���������
		void SetCamera();				//�J�����̈ʒu�E�W�I�E����W��ݒ�
		void LinkToInfo();				//���^�X�N�Ɠ���
		void SaveToJsonFile();			//���^�X�N�Ɠ���
		void DrawSample();				//��]���ړ��������I��\��
	};
}