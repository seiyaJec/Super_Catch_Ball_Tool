//-------------------------------------------------------------------
//�Q�[���{�ҊǗ�
//-------------------------------------------------------------------
#include  "../../../Engine/myPG/myPG.h"
#include  "Task_GameManager.h"
#include  "../Task_Border/Task_Border.h"
#include "../../Effects/Task_Effect2D/Effect2DIncludes.h"

namespace  GameManager
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
			std::ifstream fin("./data/gamedata/gameManager.json");
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

		this->player1pos = j["player1pos"];
		this->player1defaultAngle = j["player1defaultAngle"];
		this->ground1pos = j["ground1pos"];
		this->border1posZ = j["border1posZ"];
		this->border1checkArea = j["border1checkArea"];

		this->player2pos = j["player2pos"];
		this->player2defaultAngle = j["player2defaultAngle"];
		this->ground2pos = j["ground2pos"];
		this->border2posZ = j["border2posZ"];
		this->border2checkArea = j["border2checkArea"];

		this->playerMoveAreaW = j["playerMoveAreaW"];
		this->playerMoveAreaD = j["playerMoveAreaD"];

		this->beforeGameTime = j["beforeGameTime"];
		this->gameTime = j["gameTime"];

		this->drawMoveArea = j["drawMoveArea"];

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
		ge->qa_gameManager = ge->GetTask<GameManager::Object>("�Q�[���Ǘ�", "NoName");
		this->debugFont = ML::Font::Create("MS UI Gothic", 50);

		//���^�X�N�̐���
		////�v���C���[
		//auto p1 = PlayerBase::Object::Create(true);
		//p1->pos = this->res->data->player1pos;
		//p1->defaultAngle = this->res->data->player1defaultAngle;
		//p1->num = 0;
		//auto p2 = PlayerBase::Object::Create(true);
		//p2->pos = this->res->data->player2pos;
		//p2->defaultAngle = this->res->data->player2defaultAngle;
		//p2->num = 1;

		//�n��
		auto g1 = Ground::Object::Create(true);
		g1->pos = this->res->data->ground1pos;
		auto g2 = Ground::Object::Create(true);
		g1->pos = this->res->data->ground2pos;

		////��O����
		//auto bd1 = Border::Object::Create(true);
		//bd1->posZ = this->res->data->border1posZ;
		//bd1->checkArea = this->res->data->border1checkArea;
		//bd1->linkingGround = g2;
		//auto bd2 = Border::Object::Create(true);
		//bd2->posZ = this->res->data->border2posZ;
		//bd2->checkArea = this->res->data->border2checkArea;
		//bd2->linkingGround = g1;

		////�{�[��
		//ge->qa_ball = Ball::Object::Create(true);


		//����
		ge->qa_numberGUI = NumberGUI::Object::Create(true);

		//��ʕ����g
		Frame::Object::Create(true);

		//����
		this->beforeGameTimer = TimeCount::Object::Create(true);
		this->beforeGameTimer->SetTimeMax(this->res->data->beforeGameTime);
		this->beforeGameTimer->SetDraw(true);
		this->timer = TimeCount::Object::Create(true);
		this->timer->SetTimeMax(this->res->data->gameTime);
		this->timer->SetCounting(false);
		this->timer->SetDraw(false);

		////�X�R�A
		//ge->qa_scoreJudge = ScoreJudge::Object::Create(true);

		//�Q�[�����[�h�ɐ؂�ւ��i���j
		this->SetGameBaseTaskMode(GameBase::StateOfGame::BeforeGame);

		//�N���G�C�^�[
		auto creator = Creator::Object::Create(true);
		creator->pos.x = g1->pos.x;
		creator->pos.y = this->res->data->ground2pos.z - this->res->data->ground1pos.z;
		creator->pos.z = g2->pos.z + g1->pos.z / 2;


		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���
		ge->qa_ball.reset();
		ge->qa_numberGUI.reset();
		ge->qa_scoreJudge.reset();
		this->timer.reset();

		//�Q�[���^�X�N�L��
		auto gameTasks = this->GetGameTask();
		for (auto& gameTask : *gameTasks) {
			gameTask->Kill();
		}

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//�������p���^�X�N�̐���
			ResultManager::Object::Create(true);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	//�[�[�Q�[���O�[�[//
	void Object::UpDateBeforeGame()
	{

		//�L�[�����Ń��[�h�ؑ�
		if (ge->inpkey.Down(KEY_INPUT_Z)) {
			this->stateOfGame = StateOfGame::InGame;
			this->SetGameBaseTaskMode(StateOfGame::InGame);
		}
			
	}

	//�[�[���C���Q�[���[�[//
	void  Object::UpDateGame()
	{
		//�L�[�����Ń��[�h�ؑ�
		if (ge->inpkey.Down(KEY_INPUT_Z)) {
			this->stateOfGame = StateOfGame::BeforeGame;
			this->SetGameBaseTaskMode(StateOfGame::BeforeGame);
		}
	}
	//-------------------------------------------------------------------
	//�u�R�c�`��v�P�t���[�����ɍs�������A���C���[0
	void Object::Render3D_L0()
	{


		//�f�o�b�O���̒ǉ��`�揈��
		if (this->res->data->drawMoveArea == true) {
			//�v���C���[�ړ��\�͈͐ݒ�
			ML::Obb3 drawArea1(
				this->res->data->player1pos,
				ML::Size3(this->res->data->playerMoveAreaW, 100.0f, this->res->data->playerMoveAreaD)
			);
			ML::Obb3 drawArea2 = drawArea1.SetPosCopy(this->res->data->player2pos);

			//�`��
			ML::DrawBox3D(drawArea1, ML::Color(1, 1, 0, 0));
			ML::DrawBox3D(drawArea2, ML::Color(1, 1, 0, 0));
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs�������A3D�`���ɍs����
	void  Object::Render2D_AF()
	{
		if (this->stateOfGame == StateOfGame::BeforeGame) {
			this->debugFont->Draw(ML::Vec2(1100, 1000), "EditorMode");
		}
		else {
			this->debugFont->Draw(ML::Vec2(1100, 1000), "GameMode");
		}
	}
	//-------------------------------------------------------------------
	//�v���C���[���͈͊O�ɏo�Ă�����߂�����
	void Object::CheckPlayerInArea(ML::Vec3& playerPos_, int playerNum_) {
		//���S���W�擾
		ML::Vec3 centerPos;
		if (playerNum_ == 0) {
			centerPos = this->res->data->player1pos;
		}
		else {
			centerPos = this->res->data->player2pos;
		}
		//�ړ��\�͈͌v�Z
		ML::Rect2 moveArea(
			centerPos.x - (this->res->data->playerMoveAreaW / 2),
			centerPos.x + (this->res->data->playerMoveAreaW / 2 - 1),
			centerPos.z - (this->res->data->playerMoveAreaD / 2),
			centerPos.z + (this->res->data->playerMoveAreaD / 2 - 1)
		);

		//��O�A���A���E�Ŕ���
		//��
		if (playerPos_.x < moveArea.l) {
			playerPos_.x = moveArea.l;
		}
		//��O
		if (playerPos_.z < moveArea.t) {
			playerPos_.z = moveArea.t;
		}
		//�E
		if (playerPos_.x > moveArea.r) {
			playerPos_.x = moveArea.r;
		}
		//��
		if (playerPos_.z > moveArea.b) {
			playerPos_.z = moveArea.b;
		}

	}

	//�Q�[���{�҃^�X�N���󂯎��i���ŏ����Ȃǂ̈ꊇ�ݒ�ȂǂɎg�p����j
	shared_ptr<vector<BTask::SP>> Object::GetGameTask() {
		auto gameTasks = make_shared<vector<BTask::SP>>();	//�^�X�N�z��
		shared_ptr<vector<BTask::SP>> oneGroupTasks;
		//�Q�[���^�X�N�e�[�u���i�^�X�N���������炱�̔z����ǉ�����j
		string gameTaskGroups[] = {
			"�{�[��",
			"��O����",
			"������ʗp�̘g",
			"�n��",
			"����GUI",
			"�v���C���[",
			"�X�R�A���Z�l",
			"�X�R�A����",
			"�I",
			"�I����",
			"���Ԍv��",
		};
		//�e�[�u���Ɉ�v����O���[�v���̃^�X�N���A�^�X�N�z��ɒǉ����Ă���
		for (auto& group : gameTaskGroups) {
			oneGroupTasks = ge->GetTasks<BTask>(group);
			gameTasks->insert(gameTasks->end(), oneGroupTasks->begin(), oneGroupTasks->end());
		}

		return gameTasks;
	}


	//�Q�[���x�[�X�^�X�N�̃X�e�[�^�X�؂�ւ�
	void Object::SetGameBaseTaskMode(GameBase::StateOfGame state_) {
		auto gameTasks = this->GetGameTask();	//�Q�[���^�X�N�ǉ�
		for (auto& gameTask : *gameTasks) {
			//�X�e�[�^�X�����^�X�N�Ȃ�؂�ւ�
			GameBase* gameBaseTask = dynamic_cast<GameBase*>(gameTask.get());
			if (gameBaseTask != nullptr) {
				gameBaseTask->stateOfGame = state_;
			}
		}
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