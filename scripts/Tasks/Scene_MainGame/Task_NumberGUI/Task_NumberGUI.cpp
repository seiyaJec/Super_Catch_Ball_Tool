//-------------------------------------------------------------------
//�Q�[��UI
//-------------------------------------------------------------------
#include  "../../../Engine/myPG/myPG.h"
#include  "Task_NumberGUI.h"

namespace  NumberGUI
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
			std::ifstream fin("./data/gamedata/numberGUI.json");
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

		this->scorePos = j["scorePos"];
		this->num_oneOfImgSize = j["num_oneOfImageSize"];
		this->num_imagesNumSize = j["num_imagesNumSize"];
		this->num_imagesNumAll = j["num_imagesNumAll"];
		this->num_filePath = j["num_filePath"];

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
		//����
		numImgs = ML::PartImage::Create(
			this->data->num_filePath		//�摜�t�@�C���p�X
			, this->data->num_imagesNumAll		//��������
			, (int)this->data->num_imagesNumSize.w	//���̕�����
			, (int)this->data->num_imagesNumSize.h	//�c�̕�����
			, (int)this->data->num_oneOfImgSize.w		//���������摜�̉���
			, (int)this->data->num_oneOfImgSize.h		//���������摜�̏c��
		);
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		this->numImgs.reset();
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
		this->render2D_Priority[1] = 0.3f;
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
		//�v���C���[�\��
		//DrawNumber(this->scorePos, ge->qa_playerBase->score);

	}
	//-------------------------------------------------------------------
	//�֐���`
	//�����̕`��
	ML::Vec2 Object::DrawNumber(const ML::Vec2& pos_, int num_, bool useSign_, int digit_, float alpha_) {
		int digit = 0;
		int checkDigit = 1;		//���肷���
		//�������v�Z
		if (num_ != 0) {
			while (num_ / checkDigit != 0) {
				++digit;
				checkDigit *= 10;
			}
		}
		//������0�̏ꍇ�̏���
		else {
			digit = 1;
		}
		//���̐��̏ꍇ�A���ɕύX�i�������}�C�i�X�ɂȂ邱�Ƃ͂Ȃ����߁j
		digit = (int)fabs(digit);

		//�����̌����̂ق����傫����΍��킹��
		digit = max(digit, digit_);


		//1���ځi��̈ʁj�̍�����W���v�Z
		ML::Size2 imgSize(
			(float)this->res->numImgs->get_xSize()
			, (float)this->res->numImgs->get_ySize()
		);
		int numSizeW = (int)imgSize.w * digit;
		ML::Vec2 numDrawPos(
			pos_.x + numSizeW / 2 - imgSize.w
			, pos_.y - imgSize.h
		);

		//�����ɍ��킹���ʒu�ɐ�����`��
		int drawNum = num_;
		for (int i = 0; i < digit; ++i) {
			this->res->numImgs->Draw(numDrawPos, drawNum % 10, true, alpha_);
			numDrawPos.x -= imgSize.w;
			drawNum /= 10;
		}
		//�����̕\��������ꍇ�̏���
		if (useSign_) {
			if (num_ > 0) {
				this->res->numImgs->Draw(numDrawPos, 10, true, alpha_);
			}
			else if (num_ < 0) {
				this->res->numImgs->Draw(numDrawPos, 11, true, alpha_);
			}
		}

		
		return numDrawPos;
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