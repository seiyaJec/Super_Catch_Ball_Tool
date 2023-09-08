#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//���������
//-------------------------------------------------------------------
#include "../../../Engine/myGame/myGame.h"

namespace  KeyInput
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("���������");	//�O���[�v��
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
		bool	Inputing;		//�����^�X�N�̂ǂꂩ�����͒��̎�true�ɂ���
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
		static  Object::SP  Create(int charMax_, bool useCansel_, bool SingleCharOnly_, bool NumCharOnly_,bool flagGameEnginePushBack_);
		Resource::SP	res;
	private:
		Object(int charMax_);
		bool  B_Initialize(bool useCansel_, bool SingleCharOnly_, bool NumCharOnly_);
		bool  B_Finalize();
		bool  Initialize(bool useCansel_, bool SingleCharOnly_, bool NumCharOnly_);	//�u�������v�^�X�N�������ɂP�񂾂��s������
		void  UpDate()			override;//�u���s�v�P�t���[�����ɍs������
		void  Render3D_L0()		override;//�u3D�`��v�P�t���[�����ɍs�������A���C���[0
		void  Render2D_AF()		override;//�u2D�`��v�P�t���[�����ɍs�������A3D�`���ɍs����
		bool  Finalize();		//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	//�ύX������������������������������������������������������
	public:
		//�ǉ��������ϐ��E���\�b�h�͂����ɒǉ�����
		//���
		enum class State { INVALID, IDLE, INPUTING, WAITSEND };
		State state;

		const int charMax;
		bool inputing;			//���͂��I��������
		int inputResult;		//���͌���
		ML::Vec2 pos;			//�ʒu
		int   handle;			//���̓n���h��
		int	  moveCnt;			//�s���J�E���g
		bool  waitSendFlag;			//���͌��ʂ𑗐M������
		bool useCansel;			//esc�L�����Z���L��/����
		bool singleCharOnly;	//���p�p������t���O
		bool numberCharOnly;	//���p��������t���O



		bool		buttonActive;		//�{�^���̗L��/����
		ML::Box2D	button;				//�{�^��
		ML::Color	buttonColNormal;	//�ʏ펞�̃{�^���̐F
		ML::Color	buttonColTargeting;	//�Ə����̃{�^���̐F


		void JudgeState();						//���݂̏�Ԃɍ��킹�Ď��̏�Ԃ𔻒肷��
		void UpDateState(State nextState_);		//��Ԃ��X�V����
		void Move();							//��Ԃɍ��킹�čs������

		//�{�^���֘A
		void DrawButton();						//�{�^���`��
		bool ButtonCheckHitMouse();				//�{�^���ւ̃}�E�X�̃N���b�N����

		//�O������̃A�N�Z�X�p
		bool GetInputStr(std::string& str_);	//���͏��𑗂�AKill����i�����j
		bool GetInputNum(float& num_);	//���͏��𑗂�AKill����i���l�j
		void SetPos(ML::Vec2 pos_);		//���W�ݒ�
		void SetSingleCharOnly(bool flagSingleCharOnly_);		//���p�p������̗L���E�����ݒ�
		void SetNumberCharOnly(bool flagNumberCharOnly_);		//���p��������̗L���E�����ݒ�
	};
}