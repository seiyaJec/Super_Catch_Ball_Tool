#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//�{�[��
//-------------------------------------------------------------------
#include "../../../Bases/GameBase/GameBase.h"

namespace  Ball
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("�{�[��");	//�O���[�v��
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
		ML::Vec3	pos;		//���W
		ML::Vec3	moveVec;	//�ړ��x�N�g��
		ML::Obb3	hitBase;	//�����蔻�蒼����
		vector<int>	comboScoreTable;	//�A���q�b�g�̓��_�\
		std::string	modelFilePath;	//���f���ւ̃t�@�C���p�X

		bool  drawHitBase;		//true�Ȃ瓖���蔻��`��
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
		ML::Model3D::SP model;
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
		void  UpDateGame()		override;//�u���C���Q�[���v�P�t���[�����ɍs������
		void  Render3D_L0()		override;//�u3D�`��v�P�t���[�����ɍs������
		void  Render2D_AF()		override;//�u2D�`��v�P�t���[�����ɍs�������i3D�`���ɍs����j
		bool  Finalize();		//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	//�ύX������������������������������������������������������
	public:
		//�ǉ��������ϐ��E���\�b�h�͂����ɒǉ�����
		
		//�{�[���̏��
		enum class State{NON, IDLE, THROWN, CARRIED};

		State		state;		//�{�[���̏��
		ML::Vec3    prePos;		//1�t���[���O�̍��W
		ML::Vec3	pos;		//���W
		ML::Vec3	moveVec;	//�ړ��x�N�g��
		ML::Obb3	hitBase;	//�����蔻��
		int			combo;		//�A���q�b�g
		int			moveCnt;	//�ړ��J�E���g
		bool		flagCarried;	//�^�΂�Ă��邩

		//�����蔻����Ăяo��
		ML::Obb3 CallHitBox() const;

		//�ړ�����������1�~�����Ƃɓ����蔻����s��
		bool CheckHitPreposToPos(const ML::Obb3& you_);

		//��ʂɉ����ď�Ԃ�ω�������
		void Judge();

		//��Ԃ̍X�V
		void UpDateState(State state_);

		//��Ԃɍ��킹�ē��삳����
		void Action();

		//�t���O�����Z�b�g
		void ResetFlag();

		//�����^��
		void Carry(const ML::Vec3& carryPos_);

		//�ړ�����
		bool IsMoving();

		//�󂯎�鏈��
		void HitToTarget();
	};
}