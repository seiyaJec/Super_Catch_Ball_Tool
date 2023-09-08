#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//�I
//-------------------------------------------------------------------
#include "../../../Engine/myGame/myGame.h"

namespace  Target
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("�I");	//�O���[�v��
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
		ML::Obb3 hitBase;			//�����蔻��
		ML::Vec3 modelAngleBase;	//���f���̏����̌���
		string   modelFilePath;		//���f���ւ̃t�@�C���p�X
		bool	 drawHitBase;		//�����蔻��`��


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
		ML::Model3D::SP model;		//���f��

		ML::Color selectingColor;		//�I�𒆂̐F�i�N���G�C�^�[�j
		ML::Color movingColor;			//�ړ����̐F�i�N���G�C�^�[�j
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
		void  UpDateBeforeGame()	override;//�u�Q�[���O�X�V�v1�t���[�����Ƃɍs������
		void  UpDateGame()			override;//�u�Q�[�������v�P�t���[�����ɍs������
		void  Render3D_L0()		override;//�u3D�`��v�P�t���[�����ɍs�������A���C���[0
		void  Render2D_AF()		override;//�u2D�`��v�P�t���[�����ɍs�������A3D�`���ɍs����
		bool  Finalize();		//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	//�ύX������������������������������������������������������
	public:
		//�ǉ��������ϐ��E���\�b�h�͂����ɒǉ�����
		enum class State{ ALIVE, HIT};
		State			state;
		ML::Vec3		basePos;		//��ƂȂ���W
		ML::Vec3		pos;			//���W
		ML::Vec3		angle;			//����
		ML::Obb3		hitbase;		//�����蔻��
		ML::Model3DClone::SP modelCl;	//���f��
		float			addRotate;		//�ړ��̒���
		float			moveCnt;		//�ړ��J�E���^
		float			moveSpeed;		//�ړ����x
		float			moveScaleW;		//�ړ�����

		bool			selectFlag;			//�I�𒆂��i�N���G�C�^�[�j
		bool			moveFlag;			//�ړ������i�N���G�C�^�[�j

		ML::Obb3 CallHitBox() const;			//�����蔻��Ăяo��
		void Judge();		//���[�V�����ύX
		void UpDateState(State ns_);	//���[�V�����X�V
		void Action();		//���[�V�����ɉ����čs��
		void MoveLR();		//�ړ�
		void SetSelecting(bool flag_);	//�I�𒆁i�N���G�C�^�[�j
		void SetMoving(bool flag_);		//�ړ����i�N���G�C�^�[�j
		void ResetMove();				//�ړ������Z�b�g�i�N���G�C�^�[�j
	};
}