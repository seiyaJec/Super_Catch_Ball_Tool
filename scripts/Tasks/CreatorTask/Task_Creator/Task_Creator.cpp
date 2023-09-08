//-------------------------------------------------------------------
//�����
//-------------------------------------------------------------------
#include  "../../../Engine/myPG/myPG.h"
#include  "Task_Creator.h"

namespace  Creator
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
		this->targetSampleSideR = ML::Model3D::Create("./data/mesh/target.mv1");
		this->targetSampleSideL = ML::Model3DClone::Create(this->targetSampleSideR);
		this->targetSampleMoveRot = ML::Model3DClone::Create(this->targetSampleSideR);
		this->sampleColorSide = ML::Color(0.5f, 0.5f, 0.5f, 1);
		this->sampleColorRot = ML::Color(1, 1, 0.5f, 0.5f);
		this->targetSampleSideR->SetDiffuse(this->sampleColorSide);
		this->targetSampleSideL->SetDiffuse(this->sampleColorSide);
		this->targetSampleMoveRot->SetDiffuse(this->sampleColorRot);

		this->ptHeight = 500;
		this->moveSpeed = 100;
		this->movingTarget = nullptr;
		this->myCamera = ge->cameras[0];
		this->hitBaseMs.Resize(ML::Size3(100, 100, 100));
		this->moveArea = ML::Rect2(0, (int)ge->screenWidth / 2, 0, ge->screenHeight);
		this->tgMoveTime = 30;
		this->msBtOnCnt = 0;


		this->defaultTgMoveSpeed = 1;
		this->defaultTgMoveScale = 1000;
		this->defaultTgAddRot = 0;


		this->SetCamera();
		//���^�X�N�̐���
		TargetInfo::Object::Create(true);
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
		if (ge->inpms.On(MOUSE_INPUT_1)) {
			++this->msBtOnCnt;
		}
		else {
			this->msBtOnCnt = 0;
		}


		this->Move();
		this->TargetLinkToMouse();
		this->LinkToInfo();
		this->SetCamera();

		if (ge->inpkey.Down(KEY_INPUT_X)) {
			this->SaveToJsonFile();
		}
	}
	//-------------------------------------------------------------------
	//�u�R�c�`��v�P�t���[�����ɍs�������A���C���[0
	void Object::Render3D_L0()
	{
		ML::DrawBox3D(this->CallHitBoxMs(), ML::Color(1, 0, 0, 1));
		this->DrawSample();
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs�������A3D�`���ɍs����
	void  Object::Render2D_AF()
	{
		ML::Vec2 msPos = ge->inpms.getPos();	//�}�E�X���W
		ML::Vec3 scPos(msPos.x, msPos.y, 0);	//�X�N���[�����W

		VECTOR RaySt = DxLib::ConvScreenPosToWorldPos(scPos);
		scPos.z = 1.0f;
		VECTOR RayEn = DxLib::ConvScreenPosToWorldPos(scPos);

		ML::Vec3 RayVec(
			RayEn.x - RaySt.x,
			RayEn.y - RaySt.y,
			RayEn.z - RaySt.z
		);
		RayVec = RayVec.Norm();

		auto newRay = RayVec* (this->pos.y / -RayVec.y);
		ge->debugFont->Draw(ML::Vec2(10, 10), newRay.DebugPrint().c_str());
	}
	//-------------------------------------------------------------------
	// �֐���`
	//-------------------------------------------------------------------
	// �����蔻��Ăяo��
	ML::Obb3 Object::CallHitBoxMs() const {
		return this->hitBaseMs.OffsetCopy(this->ptPos);
	}
	//--------------------------------------
	//�I�ړ��\�͈͂ɃJ�[�\�������邩
	bool Object::IsMouseInMoveArea() {
		ML::Vec2 msPos = ge->inpms.getPos();
		return
			msPos.x >= this->moveArea.l
			&& msPos.x <= this->moveArea.r
			&& msPos.y >= this->moveArea.t
			&& msPos.y <= this->moveArea.b;
	}
	//--------------------------------------
	//�ړ�
	void Object::Move() {
		//�O
		if (ge->inpkey.On(KEY_INPUT_W)) {
			this->pos.z += this->moveSpeed;
		}
		//���
		if (ge->inpkey.On(KEY_INPUT_S)) {
			this->pos.z -= this->moveSpeed;
		}
		//�E
		if (ge->inpkey.On(KEY_INPUT_D)) {
			this->pos.x += this->moveSpeed;
		}
		//��
		if (ge->inpkey.On(KEY_INPUT_A)) {
			this->pos.x -= this->moveSpeed;
		}

		//�㉺
		this->pos.y += ge->inpms.GetWheelRotVol() * 100;

	}
	//--------------------------------------
	//�J�����ݒ�
	void Object::SetCamera() {
		//�ʒu
		this->myCamera->pos = this->pos;
		//����
		this->myCamera->target.x = ge->cameras[0]->pos.x;
		this->myCamera->target.y = 0;
		this->myCamera->target.z = ge->cameras[0]->pos.z;
		//�����
		this->myCamera->up = ML::Vec3(0, 0, 1);
	}
	//--------------------------------------
	// �I�𐶐�����
	void Object::TargetSetting() {
		//�}�E�X���ړ��\�͈͂ɂȂ���Ώ������s��Ȃ�
		if (this->IsMouseInMoveArea() == false) {
			auto targets = ge->GetTasks<Target::Object>("�I");
			for (auto& tg : *targets) {
				tg->SetSelecting(false);
				tg->SetMoving(false);
			}
			return;
		}

		bool isMouseDown = ge->inpms.Down(MOUSE_INPUT_1);
		bool isDelete = ge->inpms.Off(MOUSE_INPUT_1) && ge->inpms.On(MOUSE_INPUT_2);
		
		bool foundTarget = false;
		
		auto targets = ge->GetTasks<Target::Object>("�I");
		for (auto& tg : *targets) {
			tg->SetSelecting(false);
			tg->SetMoving(false);

			//�I��I�����A�}�E�X�̃{�^���������ꂽ��
			if (ML::CheckHit(this->CallHitBoxMs(), tg->CallHitBox())) {
				if (isMouseDown == true) {
					//�I������I���X�V
					this->movingTarget = tg;
					foundTarget = true;
					break;
				}
				else if (isDelete == true) {
					//�I���폜
					this->movingTarget.reset();
					tg->Kill();
					foundTarget = true;
				}
			}
		}
		
		//�}�E�X�̃{�^����������A�I��I�����Ă��Ȃ�������V��������
		if (isMouseDown == true) {
			if (foundTarget == false) {
				this->movingTarget = Target::Object::Create(true);
				this->movingTarget->basePos = this->ptPos;
				this->movingTarget->pos = this->ptPos;
				this->movingTarget->moveSpeed = this->defaultTgMoveSpeed;
				this->movingTarget->moveScaleW = this->defaultTgMoveScale;
				this->movingTarget->addRotate = this->defaultTgAddRot;
			}
		}

		//�I��I�𒆂Ȃ�I�̃��[�h��ς���
		if (this->movingTarget) {
			this->movingTarget->SetSelecting(true);
		}

	}
	//--------------------------------------
	//�I���}�E�X�̈ʒu�ɍ��킹��
	void Object::TargetLinkToMouse() {
		ML::Vec3 RayVec = ge->inpms.GetRayNormOfScreen(this->myCamera, ge->screenCenter().y);
		this->ptPos = this->pos + RayVec * ((this->pos.y - this->ptHeight) / -RayVec.y);

		//�I�̐����E�ړ�
		this->TargetSetting();

		//�I���ړ����łȂ���΍s��Ȃ�
		if(!this->movingTarget){
			return;
		}

		//�}�E�X��������Ă���Ԃ͈ړ�
		if (this->msBtOnCnt >= this->tgMoveTime) {
			if (this->IsMouseInMoveArea() == true) {
				this->movingTarget->SetMoving(true);
				this->movingTarget->basePos = this->ptPos;
			}
		}
	}
	//--------------------------------------
	//�I�̏���\���p�^�X�N�ɑ���
	void Object::LinkToInfo() {
		auto info = ge->GetTask<TargetInfo::Object>("�I�ݒ���");
		info->SetViewingTarget(this->movingTarget);
	}
	//--------------------------------------
	//��]���ړ��������I��\��
	void Object::DrawSample() {
		if (this->movingTarget) {
			ML::Vec3 addPos = this->movingTarget->basePos;
			addPos.x += cosf(ML::ToRadian(0)) * this->movingTarget->moveScaleW;
			this->targetSampleSideR->DrawStd(addPos, this->movingTarget->angle + this->movingTarget->res->data->modelAngleBase, ML::Vec3(0.9f, 0.9f, 0.9f));
			addPos = this->movingTarget->basePos;
			addPos.x += cosf(ML::ToRadian(180)) * this->movingTarget->moveScaleW;
			this->targetSampleSideL->DrawStd(addPos, this->movingTarget->angle + this->movingTarget->res->data->modelAngleBase, ML::Vec3(0.9f, 0.9f, 0.9f));
			addPos = this->movingTarget->basePos;
			addPos.x += cosf(ML::ToRadian(this->movingTarget->addRotate)) * this->movingTarget->moveScaleW;
			this->targetSampleMoveRot->DrawStd(addPos, this->movingTarget->angle + this->movingTarget->res->data->modelAngleBase, ML::Vec3(1.1f, 1.1f, 1.1f));
		}
	}
	//--------------------------------------
	//JSON�t�@�C���ɕۑ�
	void Object::SaveToJsonFile() {
		json AllgenDatas;	//�t�@�C���ɕۑ�����json�f�[�^

		//��������Ă���I��S��json�ɕۑ�
		auto AllTargets = ge->GetTasks<Target::Object>("�I");
		for (const auto& target : *AllTargets) {
			json genData;
			genData["basePosition"] = target->basePos;
			genData["moveSpeed"] = target->moveSpeed;
			genData["moveScaleW"] = target->moveScaleW;
			genData["addRotate"] = target->addRotate;
			AllgenDatas["generateData"].push_back(genData);
		}



		int fileNum = 0;		//�t�@�C���̏㏑��������邽�߂̃J�E���g
		bool foundData = true;	//�������O�̃t�@�C��������������
		string copyName = "";	//���O���d�������Ȃ����߂ɂ���R�s�[��

		//�����̃t�@�C�������邩�m�F
		while (foundData) {
			if (fileNum > 0) {
				copyName = "(" + to_string(fileNum) + ")";
			}
			ifstream ifs("./data/generateData/targetGenerator" + copyName + ".json");
			if (!ifs) {
				foundData = false;
			}
			else {
				foundData = true;
				++fileNum;
			}
			ifs.close();

		}

		ofstream ofs("./data/generateData/targetGenerator" + copyName + ".json");
		ofs << AllgenDatas.dump(4) << endl;
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