//-------------------------------------------------------------------
//�{�[��
//-------------------------------------------------------------------
#include  "../../../Engine/myPG/myPG.h"
#include  "Task_Ball.h"


namespace  Ball
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

		//json�t�@�C���ǂݍ���
		json j;
		if (useBinary_ == false) {
			//�t�@�C���p�X
			std::ifstream fin("./data/gamedata/ball.json");
			if (!fin) { return json(); }
			fin >> j;
			//�t�@�C���ǂݍ��ݏI��
			fin.close();
		}
		else {
			j = ge->gameData["data"][this->myArg];
		}
		//----------------------------------
		//���f�[�^�ǂݍ��ݏ���
		this->pos = j["position"];
		this->moveVec = j["moveVec"];
		this->hitBase = j["hitBase"];
		this->modelFilePath = j["modelFilePath"];
		this->comboScoreTable = j.at("comboScoreTable").get<vector<int>>();

		this->drawHitBase = j["drawHitBase"];
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
		this->model = ML::Model3D::Create(this->data->modelFilePath);

		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		this->model.reset();
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
		this->state = State::IDLE;
		this->prePos = this->res->data->pos;
		this->pos = this->res->data->pos;
		this->moveVec = this->res->data->moveVec;
		this->hitBase = this->res->data->hitBase;
		this->moveCnt = 0;
		this->flagCarried = false;

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
	void  Object::UpDateGame()
	{
		this->Judge();
		this->Action();

		++moveCnt;
	}
	//-------------------------------------------------------------------
	//�u�R�c�`��v�P�t���[�����ɍs������
	void  Object::Render3D_L0()
	{
		this->res->model->DrawStd(this->pos);

		//�����蔻��`��
		if (this->res->data->drawHitBase) {
			ML::DrawBox3D(this->CallHitBox(), ML::Color(0.5f, 0, 1, 0));
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs�������i3D�`���ɍs����j
	void  Object::Render2D_AF()
	{
	}
	//-------------------------------------------------------------------
	//�֐���`
	//-------------------------------------------------------------------
	//�����蔻����Ăяo��
	ML::Obb3 Object::CallHitBox() const {
		return this->hitBase.OffsetCopy(this->pos);
	}
	//---------------------------------------------
	//��ʂɉ����ď�Ԃ�ω�������
	void Object::Judge() {
		State nextState = this->state;

		switch (this->state) {
		case  State::NON:
			break;
		case  State::IDLE:
			if (this->flagCarried == true) {
				nextState = State::CARRIED;
			}
			break;
		case  State::THROWN:
			if (this->IsMoving() == false) {
				nextState = State::IDLE;
			}
			if (this->flagCarried == true) {
				nextState = State::CARRIED;
			}
			break;
		case  State::CARRIED:
			if (this->flagCarried == false) {
				nextState = State::IDLE;
				if (this->IsMoving() == true) {
					nextState = State::THROWN;
				}
			}
			break;
		}

		this->UpDateState(nextState);
	}
	//---------------------------------------------
	//��Ԃ̍X�V
	void Object::UpDateState(State state_) {
		if (this->state == state_) {
			return;
		}
		else {
			this->state = state_;
			this->moveCnt = 0;
		}
	}
	//---------------------------------------------
	//��Ԃɍ��킹�ē��삳����
	void Object::Action() {
		this->prePos = this->pos;

		switch (this->state) {
		case  State::NON:
			if (this->moveCnt == 0) {
				this->moveVec = ML::Vec3();
				this->combo = 0;
			}
			break;
		case  State::IDLE:
			if (this->moveCnt == 0) {
				this->moveVec = ML::Vec3();
				this->combo = 0;
			}
			break;
		case  State::THROWN:
			this->pos += moveVec;
			break;
		case  State::CARRIED:
			if (this->moveCnt == 0) {
				this->moveVec = ML::Vec3();
				this->combo = 0;
			}
			break;
		}

		//�t���O�����Z�b�g
		this->ResetFlag();
	}
	//---------------------------------------------
	//�t���O�̃��Z�b�g
	void Object::ResetFlag() {
		this->flagCarried = false;
	}
	//---------------------------------------------

	//�ړ�����������1�~�����Ƃɓ����蔻����s���B
	bool Object::CheckHitPreposToPos(const ML::Obb3& you_) {
		ML::Vec3 est = this->pos - this->prePos;
		//�ړ����Ȃ������ꍇ�̏���
		if (est == 0.0f) {
			ML::Obb3 me = this->CallHitBox();
			return ML::CheckHit(me, you_);
		}
		//X��
		while (est.x != 0.0f) {
			if (est.x >= 1.0f) { --est.x; }
			else if (est.x <= -1.0f) { ++est.x; }
			else { est.x = 0.0f; }

			//�����蔻��
			if (ML::CheckHit(this->hitBase.OffsetCopy(this->pos - est), you_)
				== true) {
				return true;
			}
		}
		//Z��
		while (est.z != 0.0f) {
			if (est.z >= 1.0f) { --est.z; }
			else if (est.z <= -1.0f) { ++est.z; }
			else { est.z = 0.0f; }

			//�����蔻��
			if (ML::CheckHit(this->hitBase.OffsetCopy(this->pos - est), you_)
				== true) {
				return true;
			}
		}
		//Y��
		while (est.y != 0.0f) {
			if (est.y >= 1.0f) { --est.y; }
			else if (est.y <= -1.0f) { ++est.y; }
			else { est.y = 0.0f; }

			//�����蔻��
			if (ML::CheckHit(this->hitBase.OffsetCopy(this->pos - est), you_)
				== true) {
				return true;
			}
		}


		return false;
	}
	//---------------------------------------------
	//�����^��
	void Object::Carry(const ML::Vec3& carryPos_) {
		this->pos = carryPos_;
		this->flagCarried = true;
	}
	//---------------------------------------------
	//�ړ�����
	bool Object::IsMoving() {
		return this->moveVec != 0.0f;
	}
	//---------------------------------------------
	//�󂯎�鏈��
	void Object::HitToTarget() {
		//�A���q�b�g���ɉ����ē��_��ǉ�
		auto& cmbScTable = this->res->data->comboScoreTable;
		int addScore = cmbScTable[min(combo, (int)cmbScTable.size() - 1)];
		ge->qa_scoreJudge->addScore(addScore);
		++this->combo;
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