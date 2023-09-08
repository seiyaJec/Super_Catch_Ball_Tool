//-------------------------------------------------------------------
//�I
//-------------------------------------------------------------------
#include  "../../../Engine/myPG/myPG.h"
#include  "Task_Target.h"

namespace  Target
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
			std::ifstream fin("./data/gamedata/target.json");
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
		this->hitBase = j["hitBase"];
		this->modelAngleBase = j["modelAngleBase"];
		this->modelFilePath = j["modelFilePath"];
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
		this->selectingColor = ML::Color(1, 1, 1, 0);
		this->movingColor = ML::Color(1, 1, 0, 0);

		//���f�[�^������
		this->model = ML::Model3D::Create(this->data->modelFilePath);


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
	bool  Object::Initialize()
	{
		//�X�[�p�[�N���X������
		__super::Initialize(defGroupName, defName, true);
		//���\�[�X�N���X����or���\�[�X���L
		this->res = Resource::Create();

		//���f�[�^������
		this->pos = ML::Vec3(0, 580, 5000);
		this->modelCl = ML::Model3DClone::Create(this->res->model);
		this->hitbase = this->res->data->hitBase;
		this->addRotate = 0;
		this->moveScaleW = 0;
		this->moveSpeed = 1;
		this->state = State::ALIVE;
		this->stateOfGame = StateOfGame::BeforeGame;


		this->selectFlag = false;
		this->moveFlag = false;
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
	//�u�Q�[���O�X�V�v�P�t���[�����Ƃɍs������
	void  Object::UpDateBeforeGame() {
		this->pos = this->basePos;
		this->moveCnt = 0;
	}
	//�u�Q�[�������v�P�t���[�����ɍs������
	void  Object::UpDateGame()
	{


		this->Judge();
		this->Action();

		++moveCnt;
	}
	//-------------------------------------------------------------------
	//�u�R�c�`��v�P�t���[�����ɍs�������A���C���[0
	void Object::Render3D_L0()
	{
		//�I�𒆂̐F�ύX
		if (this->moveFlag) {
			this->modelCl->SetDiffuse(this->res->movingColor);
			this->modelCl->SetAmbient(this->res->movingColor);
		}
		//�ړ����̐F�ύX
		else if (this->selectFlag) {
			this->modelCl->SetDiffuse(this->res->selectingColor);
			this->modelCl->SetAmbient(this->res->selectingColor);
		}
		//�ʏ�̐F�ύX
		else {
			ML::Color col(1, 1, 1, 1);
			this->modelCl->SetDiffuse(col);
			this->modelCl->SetAmbient(col);

		}
		this->modelCl->DrawStd(this->pos, this->angle + this->res->data->modelAngleBase);
		if (this->res->data->drawHitBase) {
			ML::DrawBox3D(this->CallHitBox(), ML::Color(0.5f, 0, 0, 1));
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs�������A3D�`���ɍs����
	void  Object::Render2D_AF()
	{
	}
	//-------------------------------------------------------------------
	//�֐���`
	//-------------------------------------------------------------------
	//�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[
	//�����蔻��Ăяo��
	ML::Obb3 Object::CallHitBox() const {
		return this->hitbase.OffsetCopy(this->pos);
	}
	//�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[
	//���[�V�����ύX
	void Object::Judge() {
		State nextState = this->state;

		switch (this->state) {
		case  State::ALIVE:
			if (!ge->qa_ball) {
				break;
			}
			if (ge->qa_ball->CheckHitPreposToPos(this->CallHitBox())) {
				nextState = State::HIT;
			}
			break;
		case  State::HIT:
			break;
		}

		//���[�V�����X�V
		this->UpDateState(nextState);
	}
	//�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[
	//���[�V�����X�V
	void Object::UpDateState(State ns_) {
		if (ns_ != this->state) {
			this->moveCnt = 0;
			this->state = ns_;
		}
	}
	//�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[
	//���[�V�����ɉ������ړ�
	void Object::Action() {
		switch (this->state) {
		case  State::ALIVE:
			this->MoveLR();
			break;
		case  State::HIT:
			if (!ge->qa_ball) {
				break;
			}
			ge->qa_ball->HitToTarget();
			this->Kill();
			break;
		}
	}
	//�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[
	//���ړ�
	void Object::MoveLR() {
		this->pos = this->basePos;
		float addPosX = this->moveCnt * this->moveSpeed;
		this->pos.x += cosf(ML::ToRadian(addPosX + this->addRotate)) * this->moveScaleW;

		if (fabsf(addPosX) > 360) {
			this->moveCnt = 0;
		}
	}
	//�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[
	//�I�𒆁i�N���G�C�^�[�j
	void Object::SetSelecting(bool flag_) {
		this->selectFlag = flag_;
	}
	//�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[
	//�ړ����i�N���G�C�^�[�j
	void Object::SetMoving(bool flag_) {
		this->moveFlag = flag_;
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