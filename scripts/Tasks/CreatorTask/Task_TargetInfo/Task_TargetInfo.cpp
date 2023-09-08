//-------------------------------------------------------------------
//�I�ݒ���
//-------------------------------------------------------------------
#include  "../../../Engine/myPG/myPG.h"
#include  "Task_TargetInfo.h"

namespace  TargetInfo
{
	//��������������������������������������������������������������������
	// �Q�[���f�[�^
	//��������������������������������������������������������������������
	//-------------------------------------------------------------------
	//�Q�[���G���W���ɂ���f�[�^�z��̒��ŁA���g�̗v�f�������ԍ�
	int Data::myNum = BData::NODATA;
	//-------------------------------------------------------------------
	
	//�Q�[�����[�h��ǂݍ���
	json Data::LoadDataJson(bool useBinary_) {
		json j;

		if (useBinary_ == false) {
			//�t�@�C���p�X
			std::ifstream fin("none");
			if (!fin) { return json(); }
			//JSON�t�@�C���ǂݍ���
			fin >> j;
			//�t�@�C���ǂݍ��ݏI��
			fin.close();
		}
		else {
			j = ge->gameData["data"][this->myArg];
		}
		//----------------------------------
		//���f�[�^�ǂݍ��ݏ���



		//----------------------------------
		//�i�����g�p����ꍇ��myPG.cpp�̓ǂݍ��݊֐��ɂ��̃N���X��ǉ�����̂��Y�ꂸ�ɁI�����j 

		return j;
	}

	//��������������������������������������������������������������������
	// ���\�[�X
	//��������������������������������������������������������������������
	//-------------------------------------------------------------------
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		//�f�[�^�N���X������󂯎��
		this->data = Data::UseData();

		//���f�[�^������
		this->font = ML::Font::Create("MS UI Gothic", 80);

		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		this->font.reset();
		return true;
	}

	//��������������������������������������������������������������������
	// �I�u�W�F�N�g
	//��������������������������������������������������������������������
	//-------------------------------------------------------------------
	//�u�������v�^�X�N�������ɂP�񂾂��s������
	bool  Object::Initialize()
	{
		//�X�[�p�[�N���X������
		__super::Initialize(defGroupName, defName, true);
		//���\�[�X�N���X����or���\�[�X���L
		this->res = Resource::Create();

		//���f�[�^������
		int inputCharMax = 10;
		ML::Vec2 viewDistance(-100, 0);
		this->viewPosX.Create(inputCharMax, true, viewDistance);
		this->viewPosZ.Create(inputCharMax, true, viewDistance);
		this->viewMoveSpeed.Create(inputCharMax, true, viewDistance);
		this->viewMoveScale.Create(inputCharMax, true, viewDistance);
		this->viewAddRot.Create(inputCharMax, true, viewDistance);



		this->viewPosX.SetData(ML::Vec2(1100, 200), "X�F");
		this->viewPosZ.SetData(			ML::Vec2(1100, 350), "Z�F");
		this->viewMoveSpeed.SetData(	ML::Vec2(1100, 500), "MoveSpeed�F");
		this->viewMoveScale.SetData(	ML::Vec2(1100, 650), "MoveScale�F");
		this->viewAddRot.SetData(		ML::Vec2(1100, 800), "AddRot�F");
		this->viewingTarget.reset();

		//���^�X�N�̐���

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���


		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//�������p���^�X�N�̐���
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		this->LinkTargetToView();
	}
	//-------------------------------------------------------------------
	//�u�R�c�`��v�P�t���[�����ɍs�������A���C���[0
	void Object::Render3D_L0()
	{

	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs�������A3D�`���ɍs����
	void  Object::Render2D_AF()
	{
	}
	//-------------------------------------------------------------------
	//�֐���`
	//-------------------------------------------------------------------
	//---------------------------------
	//�r���[�̒l��I�𒆂̓I�ƃ����N������
	void Object::LinkTargetToView() {
		//�I�𒆂̓I������ꍇ�̂ݍs��
		auto target = this->viewingTarget.lock();
		if (target) {
			this->viewPosX.LinkValue(target->basePos.x);
			this->viewPosZ.LinkValue(target->basePos.z);
			this->viewMoveSpeed.LinkValue(target->moveSpeed);
			this->viewMoveScale.LinkValue(target->moveScaleW);
			this->viewAddRot.LinkValue(target->addRotate);
		}
	}
	//�\������I��ݒ肷��
	void Object::SetViewingTarget(Target::Object::SP target_) {
		this->viewingTarget = target_;
	}
	//-------------------------------------------------------------------
	//�r���[�N���X���\�b�h��`
	//-------------------------------------------------------------------
	//����
	void Object::View::Create(int inputCharMax_, bool flagGameEnginePushBack_, const ML::Vec2& posDistance_) {
		this->info = Infomation::Object::Create(flagGameEnginePushBack_);
		this->input = KeyInput::Object::Create(inputCharMax_, true, true, true, flagGameEnginePushBack_);
		this->posDistance = posDistance_;
	}
	//���W�E�������͐ݒ�
	void Object::View::SetData(const ML::Vec2& pos_, const std::string& data_) {
		this->info->SetData(pos_, data_);
		ML::Vec2 inputPos = pos_ + this->posDistance;
		this->input->SetPos(inputPos);
	}
	//�w�肵���ϐ��Ɠ���������
	void Object::View::LinkValue(float& value_) {
		//���͂�����Ύ擾
		float newValue;
		if (this->input->GetInputNum(newValue) == true) {
			value_ = newValue;
		}
		//�l��\��������
		this->info->SetValue(value_);
	}
	//������������������������������������������������������������������������������������
	//�ȉ��͊�{�I�ɕύX�s�v�ȃ��\�b�h
	//������������������������������������������������������������������������������������
	//-------------------------------------------------------------------
	//�^�X�N��������
	Object::SP  Object::Create(bool  flagGameEnginePushBack_)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//�Q�[���G���W���ɓo�^
				
			}
			if (!ob->B_Initialize()) {
				ob->Kill();//�C�j�V�����C�Y�Ɏ��s������Kill
			}
			return  ob;
		}
		return nullptr;
	}
	//-------------------------------------------------------------------
	bool  Object::B_Initialize()
	{
		return  this->Initialize();
	}
	//-------------------------------------------------------------------
	Object::~Object() { this->B_Finalize(); }
	bool  Object::B_Finalize()
	{
		auto  rtv = this->Finalize();
		return  rtv;
	}
	//-------------------------------------------------------------------
	Object::Object() {	}
	//-------------------------------------------------------------------
	//���\�[�X�N���X�̐���
	Resource::SP  Resource::Create()
	{
		if (auto sp = instance.lock()) {
			return sp;
		}
		else {
			sp = Resource::SP(new  Resource());
			if (sp) {
				sp->Initialize();
				instance = sp;
			}
			return sp;
		}
	}
	//-------------------------------------------------------------------
	Resource::Resource() {}
	//-------------------------------------------------------------------
	Resource::~Resource() { this->Finalize(); }
	//-------------------------------------------------------------------
	//�Q�[���G���W������f�[�^���󂯎��
	Data::SP Data::UseData() {


		//-------------------------------------
		//�g�p���Ȃ��Ƃ����ݒ�imyNum��NOUSE�j�̏ꍇ
		if (myNum == BData::NOUSE) {
			return nullptr;
		}
		//-------------------------------------
		//myNum���ݒ肳��Ă��Ȃ��ꍇ�̏���
		//�Q�[���G���W���ɂ���f�[�^�̉��Ԗڂ���������������
		if (myNum == BData::NODATA) {
			for (auto& it : ge->data) {
				++myNum;
				//���g�̃f�[�^����������J�E���g�I���E�|�C���^��Ԃ�
				if (typeid(*it) == typeid(Data)) {
					return dynamic_pointer_cast<Data>(it);
				}
			}
			//������Ȃ������ꍇ�A�g�p���Ȃ��ݒ�ɐ؂�ւ���
			myNum = BData::NOUSE;
			return nullptr;

		}
		//-------------------------------------
		//���łɒl�����܂��Ă���ꍇ�̏���
		return dynamic_pointer_cast<Data>(ge->data[myNum]);
	}
	//-------------------------------------------------------------------
	Data::Data() { }		//�R���X�g���N�^�i���ɏ����Ȃ��j
	//-------------------------------------------------------------------
	Data::~Data() { }		//�f�X�g���N�^�i���ɏ����Ȃ��j
	//-------------------------------------------------------------------


}