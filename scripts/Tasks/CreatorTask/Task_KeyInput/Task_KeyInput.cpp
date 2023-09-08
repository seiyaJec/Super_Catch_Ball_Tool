//-------------------------------------------------------------------
//���������
//-------------------------------------------------------------------
#include  "../../../Engine/myPG/myPG.h"
#include  "Task_KeyInput.h"

namespace  KeyInput
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

		//��jpos = j[�L�[�̖��O]

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
		this->Inputing = false;

		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		return true;
	}

	//��������������������������������������������������������������������
	// �I�u�W�F�N�g
	//��������������������������������������������������������������������
	//-------------------------------------------------------------------
	//�u�������v�^�X�N�������ɂP�񂾂��s������
	bool  Object::Initialize(bool useCansel_, bool SingleCharOnly_, bool NumCharOnly_)
	{
		//�X�[�p�[�N���X������
		__super::Initialize(defGroupName, defName, true);
		//���\�[�X�N���X����or���\�[�X���L
		this->res = Resource::Create();

		//���f�[�^������
		this->state = State::IDLE;
		this->pos = ML::Vec2();
		this->inputing = false;
		this->inputResult = 0;
		this->handle = 0;
		this->moveCnt = 0;
		this->waitSendFlag = false;
		this->useCansel = useCansel_;
		this->singleCharOnly = SingleCharOnly_;
		this->numberCharOnly = NumCharOnly_;

		this->buttonActive = true;
		this->button.Resize(100, 100);
		this->button.Set(this->pos);
		this->buttonColNormal = ML::Color(1, 1, 1, 1);
		this->buttonColTargeting = ML::Color(1, 1, 1, 0);
		//���^�X�N�̐���

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���
		DxLib::DeleteKeyInput(this->handle);

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//�������p���^�X�N�̐���
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		this->JudgeState();
		this->Move();

		++this->moveCnt;
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
		//�{�^���L���Ȃ�`��
		if (this->buttonActive) {
			this->DrawButton();
		}

		switch (this->state) {
		case State::IDLE:
			break;
		case State::INPUTING:
			DrawKeyInputString((int)this->pos.x, (int)this->pos.y, this->handle);
			break;
		case State::WAITSEND:
			break;
		}
	}
	//-------------------------------------------------------------------
	// �֐���`
	//-------------------------------------------------------------------
	//���݂̏�Ԃɍ��킹�Ď��̏�Ԃ𔻒肷��
	void Object::JudgeState() {
		//��Ԃ��Ƃɔ���
		State nextState = this->state;

		switch (this->state) {
		case State::INVALID:
			if (this->res->Inputing == false) {
				nextState = State::IDLE;
			}
			break;
		case State::IDLE:
			//�{�^���������ꂽ����͂ɐ؂�ւ�
			if (ge->inpms.Down(MOUSE_INPUT_1)) {
				if (this->ButtonCheckHitMouse()) {
					nextState = State::INPUTING;
				}
			}
			//���̃^�X�N�����͒��Ȃ疳����
			if (this->res->Inputing == true) {
				nextState = State::INVALID;
			}
			break;
		case State::INPUTING: 
			//���͒��ȊO�Ȃ瑗�M�҂��ɐ؂�ւ�
			if (this->inputResult != 0) {
				nextState = State::WAITSEND;
			}
			break;
		case State::WAITSEND:
			if (this->waitSendFlag == false) {
				nextState = State::IDLE;
			}
			break;
		}

		this->UpDateState(nextState);
	}
	//-----------------------------
	//��Ԃ��X�V����
	void Object::UpDateState(State nextState_) {
		if (nextState_ != this->state) {
			this->state = nextState_;
			this->moveCnt = 0;
		}
	}
	//-----------------------------
	//��Ԃɍ��킹�čs������
	void Object::Move() {
		//���ʂ̏���
		this->button.Set(this->pos);



		switch (this->state) {
		case State::INVALID:
			this->buttonActive = false;
			break;
		case State::IDLE:
			if (this->moveCnt == 0) {
				this->res->Inputing = false;
				this->buttonActive = true;
			}
			break;
		case State::INPUTING:
			if (this->moveCnt == 0) {
				this->res->Inputing = true;
				this->buttonActive = false;
				this->handle = MakeKeyInput(this->charMax, this->useCansel, this->singleCharOnly, this->numberCharOnly);
				DxLib::SetActiveKeyInput(this->handle);
			}
			this->inputResult = CheckKeyInput(this->handle);

			//�}�E�X�̃N���b�N�������ꂽ��L�����Z��
			if (this->moveCnt != 0) {
				if (ge->inpms.Down(MOUSE_INPUT_1)) {
					this->inputResult = 2;
				}
			}
			break;
		case State::WAITSEND:
			if (this->moveCnt == 0) {
				this->waitSendFlag = true;
			}
			//���M�悪������Ȃ������ꍇ�͑��M�҂��I��
			else {
				this->waitSendFlag = false;
			}
			//�L�����Z�����ꂽ�ꍇ�͑��M���Ȃ�
			if (this->inputResult == 2) {
				this->waitSendFlag = false;
			}
			break;
		}
	}
	//-----------------------------
	//�{�^���`��
	void Object::DrawButton() {
		//�{�^���������Ȃ�s��Ȃ�
		if (this->buttonActive == false) {
			return;
		}

		//�J�[�\����Ə���
		if (this->ButtonCheckHitMouse() == false) {
			ML::DrawBox(this->button, this->buttonColNormal);
		}
		//�Ə���
		else {
			ML::DrawBox(this->button, this->buttonColTargeting);
		}
	}
	//-----------------------------
	//�{�^���ւ̃}�E�X�̃N���b�N����
	bool Object::ButtonCheckHitMouse() {
		ML::Box2D msHit(0, 0, 1, 1);
		msHit.Set(ge->inpms.getPos());
		return msHit.Hit(this->button);
	}
	//-----------------------------
	//���͏��𑗂�AKill����i�����j
	bool Object::GetInputStr(string& str_) {
		//���M�҂��o�Ȃ����false
		if(this->waitSendFlag == false){
			return false;
		}

		char* buff = new char[this->charMax];
		DxLib::GetKeyInputString(buff, this->handle);
		str_ = buff;
		delete[] buff;
		this->waitSendFlag = true;
		return true;
	}

	//-----------------------------
	//���͏��𑗂�AKill����i���l�j
	bool Object::GetInputNum(float& num_) {
		//���M�҂��o�Ȃ����false
		if (this->waitSendFlag == false) {
			return false;
		}
		num_ = DxLib::GetKeyInputNumberToFloat(this->handle);
		this->waitSendFlag = true;
		return true;
	}
	//-----------------------------
	//���W�ݒ�
	void Object::SetPos(ML::Vec2 pos_) {
		this->pos = pos_;
	}
	//-----------------------------
	//���p�p���̗L���E�����ݒ�
	void Object::SetSingleCharOnly(bool flagSingleCharOnly_) {
		this->singleCharOnly = flagSingleCharOnly_;
	}
	//-----------------------------
	//���p�����̗L���E�����ݒ�
	void Object::SetNumberCharOnly(bool flagNumberCharOnly_) {
		this->numberCharOnly = flagNumberCharOnly_;
	}
	//������������������������������������������������������������������������������������
	//�ȉ��͊�{�I�ɕύX�s�v�ȃ��\�b�h
	//������������������������������������������������������������������������������������
	//-------------------------------------------------------------------
	//�^�X�N��������
	Object::SP  Object::Create(int charMax_, bool useCansel_, bool SingleCharOnly_, bool NumCharOnly_, bool flagGameEnginePushBack_)
	{
		Object::SP  ob = Object::SP(new  Object(charMax_));
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//�Q�[���G���W���ɓo�^
				
			}
			if (!ob->B_Initialize(useCansel_, SingleCharOnly_, NumCharOnly_)) {
				ob->Kill();//�C�j�V�����C�Y�Ɏ��s������Kill
			}
			return  ob;
		}
		return nullptr;
	}
	//-------------------------------------------------------------------
	bool  Object::B_Initialize(bool useCansel_, bool SingleCharOnly_, bool NumCharOnly_)
	{
		return  this->Initialize(useCansel_, SingleCharOnly_, NumCharOnly_);
	}
	//-------------------------------------------------------------------
	Object::~Object() { this->B_Finalize(); }
	bool  Object::B_Finalize()
	{
		auto  rtv = this->Finalize();
		return  rtv;
	}
	//-------------------------------------------------------------------
	Object::Object(int charMax_)
		:charMax(charMax_)
	{}
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