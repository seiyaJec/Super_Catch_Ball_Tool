#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//�I�ݒ���
//-------------------------------------------------------------------
#include "../../../Engine/myGame/myGame.h"
#include "../Task_KeyInput/Task_KeyInput.h"
#include "../Task_Infomation/Task_Infomation.h"

namespace  TargetInfo
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("�I�ݒ���");	//�O���[�v��
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
		ML::Font::SP  font;
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
		//�r���[�N���X
		struct View {
			Infomation::Object::SP info;
			KeyInput::Object::SP	input;
			ML::Vec2				posDistance;	//info�^�X�N�����_�Ƃ����Ainput�^�X�N�̍��W

			//����
			void Create(int inputCharMax_, bool flagGameEnginePushBack_, const ML::Vec2& posDistance_ = ML::Vec2());

			//���W�E�������͐ݒ�
			void SetData(const ML::Vec2& pos, const std::string& data_);

			//�w�肵���ϐ��Ɠ���������
			void LinkValue(float& value_);
		};


		View	viewPosX;		//�I�̏������WX
		View	viewPosZ;		//�I�̏������WZ
		View	viewMoveSpeed;	//�I�̉����̑���
		View	viewMoveScale;	//�I�̉����̑傫��
		View	viewAddRot;		//�I�̒ǉ��̉�]�@�W����0

		Target::Object::WP viewingTarget;	//�\�����̓I

		void LinkTargetToView();//�r���[�̒l��I�𒆂̓I�ƃ����N������
		void SetViewingTarget(Target::Object::SP target_);//�\������I��ݒ肷��
	};
}