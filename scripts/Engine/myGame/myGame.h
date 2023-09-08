#pragma once
#include "DxLib.h"
#include "../fpscounter/fpscounter.h"
#include "../MyLibrary/MyLibrary.h"
#include <memory>
#include <vector>
#include <map>
#include <unordered_map>
#include <string>
#include <sstream>

//�^�X�N�f�[�^��{�N���X
class BData {
public:
	using SP = std::shared_ptr<BData>;
	enum { NOUSE = -2, NODATA };
	BData();
	static int argManager;		//�������ꂽ���Ԃ��J�E���g
	int myArg;					//�����̏��Ԃ��i�[
	virtual json LoadDataJson(bool useBinary = false) = 0;		//�e�L�X�g�f�[�^�ǂݍ���
};

//�^�X�N���\�[�X��{�N���X
class BResource
{
protected:
	//�R���X�g���N�^
	BResource(){}
public:
	//�f�X�g���N�^
	~BResource() {}
	//�������E�X�V����
	virtual bool Initialize() = 0;
	virtual bool Finalize() = 0;
	//�V�F�A�|�C���^�^���ύX
	using SP = shared_ptr<BResource>;
	using WP = weak_ptr<BResource>;
};

//�^�X�N��{�N���X
class BTask
{
public:
	//�V�F�A�|�C���^�^���ύX
	using SP = shared_ptr<BTask>;
	using WP = weak_ptr<BTask>;
	//�X�e�[�^�X�񋓑�
	enum class State :DWORD {
		Active,		//�ʏ�
		Stop,		//��~�i�X�e�b�v���s��~�j
		Kill,		//����
		Suspend,	//�T�X�y���h�i�ꎞ�����j
		StateNon = 0xFFFFFFFF
	};
private:
	static DWORD uKeyManager;	//���j�[�N�L�[�}�l�[�W��					��
	DWORD		uKey;			//�^�X�N���ƂɊ��蓖�Ă��郆�j�[�N�L�[	��
	DWORD		groupKey;		//�O���[�v�����玩���Ő��������L�[
	bool		detectible;		//GetTask�n���\�b�h�Ō��o�ł���
	int			taskStateCnt_Stop;			//1�ȏ�Ȃ��~
	int			taskStateCnt_Suspend;		//1�ȏ�Ȃ疳����
	int			taskStateCnt_Kill;			//1�ȏ�Ȃ����
//protected
public:
	string		groupName;		//�^�X�N�Ɋ��蓖�Ă�O���[�v���i�����Ɏg�p�j
	string		name;			//�^�X�N�Ɋ��蓖�Ă閼�O�i�����Ɏg�p�j
	bool		render2D_Active[2];		//�`��̗L���i[0]��3D�`��O�A[1]��3D�`���j
	bool		render3D_Active[4];		//�`��̗L��
	float		render2D_Priority[2];	//2D�p�v���C�I���e�B�i[0]��3D�`��O�A[1]��3D�`���j
	float		render3D_Priority[4];	//3D�p�v���C�I���e�B
	bool		render3D_Alpha;			//3D�p���߂̗L��
	bool		nextTaskCreate;			//finalize�����̒��Ń^�X�N�𐶐����邩�ۂ�

	DWORD UKey() { return uKey; }
	DWORD GroupKey() { return groupKey; }
	bool Detectible() { return detectible; }
	int TaskStateCnt_Stop() { return taskStateCnt_Stop; }
	int TaskStateCnt_Suspend() { return taskStateCnt_Suspend;}
	int TaskStateCnt_Kill() { return taskStateCnt_Kill; }

protected:
	BTask()
		:
		groupName(),
		name(),
		render3D_Alpha(false),
		detectible()
	{
		this->uKey = uKeyManager++;
		this->groupKey = 0;
	}
	bool Initialize(const string& gn_, const string& n_, bool det_)
	{
		this->groupName = gn_;
		this->name = n_;
		memcpy(&this->groupKey, gn_.c_str(), min(4, gn_.length()));
		this->detectible = det_;

		this->taskStateCnt_Kill = 0;
		this->taskStateCnt_Stop = 0;
		this->taskStateCnt_Suspend = 0;

		render2D_Priority[0] = 0; 
		render2D_Active[0] = true;

		render2D_Priority[1] = 0;
		render2D_Active[1] = true;

		render3D_Priority[0] = 0;
		render3D_Active[0] = true;

		render3D_Priority[1] = 0;
		render3D_Active[1] = true;

		render3D_Priority[2] = 0;
		render3D_Active[2] = true;

		render3D_Priority[3] = 0;
		render3D_Active[3] = true;

		nextTaskCreate = true;
		return true;
	}
public:
	virtual ~BTask() {
		////�e�Ɖ���؂�
		//this->SetParent(nullptr);
	}
	//�^�X�N�̍X�V����
	void T_Step() {
		//�L���A�������͖������̃J�E���g������Ώ����Ȃ�
		if (this->taskStateCnt_Kill > 0 ||
			this->taskStateCnt_Suspend > 0) {
		}
		//�ꎞ��~�Ȃ�ꎞ��~�����i�f�t�H���g�͏����Ȃ��j
		else if (this->taskStateCnt_Stop > 0) {
			UpDate_Stop();
		}
		//��L�J�E���g��������Βʏ�̍X�V����
		else {
			UpDate();
		}
	}

	void T_UpDate3DPriority() {
		if (this->taskStateCnt_Kill > 0 ||
			this->taskStateCnt_Stop > 0 ||
			this->taskStateCnt_Suspend > 0) {
			return;
		}
		UpDate3DPriority();
	}

	void T_Draw2D(DWORD ln_) {
		if (this->taskStateCnt_Kill > 0 ||
			this->taskStateCnt_Suspend > 0) {
			return;
		}
		switch (ln_) {
		case 0: 
			Render2D_BF();
			break;
		case 1:
			Render2D_AF();
			break;
		}
	}

	void T_Draw3D(DWORD ln_) {
		if(this->taskStateCnt_Kill>0||
			this->taskStateCnt_Suspend > 0) {
			return;
		}
		switch (ln_) {
		case 0:
			Render3D_L0();
			break;
		case 1:
			Render3D_L1();
			break;
		case 2:
			Render3D_L2();
			break;
		case 3:
			Render3D_L3();
			break;
		}
	}

	//���݂̏�Ԃ𕶎���ŕԂ�
	virtual string DebugOut()
	{
		stringstream ss;
		if (this->taskStateCnt_Kill > 0) { ss << "���ŏ���" << this->taskStateCnt_Kill << ":"; }
		if (this->taskStateCnt_Stop > 0) { ss << "��~" << this->taskStateCnt_Stop << ":"; }
		if (this->taskStateCnt_Suspend > 0) { ss << "�\����OFF" << this->taskStateCnt_Suspend << ":"; }
		if (this->taskStateCnt_Kill >= 0 &&
			this->taskStateCnt_Stop >= 0 &&
			this->taskStateCnt_Suspend >= 0
			) {
			ss << "�ʏ�";
		}
		return ss.str();
	}

	virtual void UpDate() = 0;
	virtual void UpDate_Stop() {};	//��~���Ɏg�킹��ǉ�����
	virtual void Render2D_BF() { render2D_Active[0] = false; };
	virtual void Render2D_AF() { render2D_Active[1] = false; };

	virtual void Render3D_L0() { render3D_Active[0] = false; };
	virtual void Render3D_L1() { render3D_Active[1] = false; };
	virtual void Render3D_L2() { render3D_Active[2] = false; };
	virtual void Render3D_L3() { render3D_Active[3] = false; };
	virtual void UpDate3DPriority() { };	//��̏���...?

	//��ԃJ�E���^����or����
	//���ł�����
	void Kill(bool  nt_ = true)
	{
		this->nextTaskCreate = nt_;
		this->taskStateCnt_Kill++;
		////�q������������
		//for (auto it = this->children.begin(); it != this->children.end(); it++) {
		//	if (auto  c = (*it).lock()) {
		//		c->Kill(nt_);
		//	}
		//}
	}
	//�T�X�y���h�ɂ���
	void Suspend(bool  f_ = true)
	{
		if (f_) { this->taskStateCnt_Suspend++; }
		else { this->taskStateCnt_Suspend = max(this->taskStateCnt_Suspend - 1, 0); }
		////�q������������
		//for (auto it = this->children.begin(); it != this->children.end(); it++) {
		//	if (auto  c = (*it).lock()) {
		//		c->Suspend(f_);
		//	}
		//}
	}
	//��~��Ԃɂ���
	void Stop(bool  f_ = true)
	{
		if (f_) { this->taskStateCnt_Stop++; }
		else { this->taskStateCnt_Stop = max(this->taskStateCnt_Stop - 1, 0); }
		////�q������������
		//for (auto it = this->children.begin(); it != this->children.end(); it++) {
		//	if (auto  c = (*it).lock()) {
		//		c->Stop(f_);
		//	}
		//}
	}
	//��Ԃ𒲂ׂ�i�������͗D��x�̍������̂��A��j
	State  CheckState() {
		if (this->taskStateCnt_Kill > 0) { return  State::Kill; }
		else if (this->taskStateCnt_Suspend > 0) { return  State::Suspend; }
		else if (this->taskStateCnt_Stop > 0) { return  State::Stop; }
		else { return  State::Active; }
	}
public:
	WP				me;				//�܂���ȃ|�C���^
	//WP				root;		//���[�g
	//WP				parent;		//�e
	//vector<WP>		children;	//�q

	//void   SetParent(SP  parent_)
	//{
	//	//���݂̐e���ݒ肳��Ă���ꍇ�A���؂���s��
	//	//����if���ŁAweak_ptr���Ď���Ԃ��ǂ������킩��
	//	if (auto  pa = this->parent.lock()) {
	//		//�e�Ɏq���Ƃ��Ẳ���؂点��
	//		auto  m = this->me.lock();
	//		for (auto it = pa->children.begin();
	//			it != pa->children.end();
	//			it++) {
	//			auto c = (*it).lock();
	//			if (c == m) {
	//				pa->children.erase(it);
	//				break;
	//			}
	//		}
	//		//�q�ɐe���̂Ă�����
	//		this->parent.reset();
	//		this->root = this->me;
	//	}

	//	//�e�q�֌W��o�^����
	//	if (parent_) {
	//		this->root = parent_->root;
	//		this->parent = parent_;
	//		parent_->children.push_back(this->me);
	//	}
	//}
	//void   SetParent(WP  parent_)
	//{
	//	SetParent(parent_.lock());
	//}
};










class myEngine
{
	static  const  DWORD    vrDef = 32;//�^�X�N���o���̃x�N�^�[�T�C�Y�����l

public:
	DWORD			frameCnt;			//�@�A�v���J�n����̃t���[���̌v��
	DWORD			screenWidth;		//	��ʃT�C�Y
	DWORD			screenHeight;		//	��ʃT�C�Y
	//float			viewScaleW;			//	�\���{��
	//float			viewScaleH;			//	�\���{��
	//DWORD			multiSample;		//	�}���`�T���v��
	//bool			screenMode;			//	�t���X�N���[�����[�h
	DWORD			screen2DWidth;		//	2D�`��p�𑜓x
	DWORD			screen2DHeight;		//	2D�`��p�𑜓x
	//const TCHAR* windowTitle;		//	�E�C���h�E�^�C�g��
	//const TCHAR* windowClassName;	//	�E�B���h�E�N���X��
	bool			quitRequest;		//	GameMain�ɏI���v�����鎞true�ɂ���i�f�t�H���g��esc�L�[�j
	bool			finishGame;			//�@DXlibrary�̃��[�v���I������t���O
	bool			layerEnable2D[2];	//	���C���[��ON�EOFF
	bool			layerEnable3D[4];	//	���C���[��ON�EOFF
	bool			debugMode;			//�@�f�o�b�O���[�h�ؑ�
	ML::Font::SP		debugFont;			//�@�f�o�b�O�p�̃t�H���g

public:
	//�A�b�v�f�[�g�ƃ����_�[�̌Ăяo���ɑ΂���ݒ�
	//�o�������l�̏ꍇ�A�o�����P�t���[���ɂP��Ăяo�����
	//�Ⴆ�΁A�Q�F�P�Őݒ肷��ƁA�A�b�v�f�[�g�Q��ɑ΂��ă����_�[�͂P��Ăяo�����悤�ɂȂ�
	WORD		updateCall;
	WORD		renderCall;
protected:
	WORD		updateCnt;
	WORD		renderCnt;

private:
	vector<pair<DWORD, BTask::SP>>	tasks_detect;	//���ݓo�^����Ă���S�^�X�N(���o�L���ȁj
	vector<BTask::SP>  tasks_undetect;		//���ݓo�^����Ă���S�^�X�N(���o�����ȁj
	vector<BTask::SP>  add_Tasks;	//���ݓo�^��҂��Ă���S�^�X�N
public:
	bool QuitFlag() { return quitRequest; }
	bool FinishGame() { return finishGame; }
	myEngine(
		DWORD				sw_,	//	��ʃT�C�Y
		DWORD				sh_,	//	��ʃT�C�Y
		//float				vs_,	//	�\���{��
		//DWORD				ms_,	//	�}���`�T���v���A���`�G�C���A�X�i1,2,4)
		//bool				sm_,	//	�t���X�N���[�����[�h
		DWORD				sw2D_,	//	2D�`��p�𑜓x
		DWORD				sh2D_,	//	2D�`��p�𑜓x
		//const TCHAR* wtn_,	//	�E�C���h�E�^�C�g��
		//const TCHAR* wcn_,	//	�E�B���h�E�N���X��
		WORD				ucall_ = 1,	//
		WORD			    rcall_ = 1)	//
		:
		screenWidth(sw_)
		, screenHeight(sh_)
		//, viewScaleW(vs_)
		//, viewScaleH(vs_)
		//, multiSample(ms_)
		//, screenMode(sm_)
		, screen2DWidth(sw_)
		, screen2DHeight(sh_)
		//, windowTitle(wtn_)
		//, windowClassName(wcn_)
		,quitRequest(false)
		,finishGame(false)
		//, frameCnt(0)
		,updateCall(ucall_)
		, renderCall(rcall_)
		, updateCnt(0)
		, renderCnt(0)

	{
		

		////�[���t���X�N���[�����[�h�ɂ��ύX
		//if (screenMode == true)
		//{
		//	//���݂̃X�N���[���T�C�Y���g�p����
		//	RECT dtr;
		//	if (GetWindowRect(GetDesktopWindow(), &dtr)) {
		//		this->viewScaleW = (float)dtr.right / (float)screenWidth;
		//		this->viewScaleH = (float)dtr.bottom / (float)screenHeight;
		//	}
		//}
	}
	//	�Q�[������
	void B_Step();
	void  B_StepSub_CallTaskUpDate();
	void  B_UpDateSub_CallTaskUpDate3DPriority();
	void  B_StepSub_CallTaskRender2D_BF();
	void  B_StepSub_CallTaskRender3D_4();
	void  B_StepSub_CallTaskRender2D_AF();
	void  B_StepSub_DebugOut();
	void  B_StepSub_DeleteTask();
	void  B_StepSub_Entry();
	//	������
	bool B_Initialize();
	//	���
	virtual ~myEngine();
	//	�^�X�N�̓o�^
	void PushBack(const BTask::SP& t_);
	//	�����̃^�X�N��S�č폜����
	bool KillAll_G(const string& gn_, bool  nt_ = true);
	bool KillAll_GN(const string& gn_, const string& n_, bool  nt_ = true);
	//	�����̃^�X�N��S�Ē�~����
	bool StopAll_G(const string& gn_, bool  m_ = true);
	bool StopAll_GN(const string& gn_, const string& n_, bool  m_ = true);
	//	�����̃^�X�N��S�Ĉꎞ�����ɂ���
	bool SuspendAll_G(const string& gn_, bool  m_ = true);
	bool SuspendAll_GN(const string& gn_, const string& n_, bool  m_ = true);
	//	���C���[�̗L��������ݒ�E�擾�i����j
	void Layer_2D(DWORD ln_, bool st_) { if (ln_ >= 0 && ln_ < 2) { layerEnable2D[ln_] = st_; } }
	bool Layer_2D(DWORD ln_) { if (ln_ >= 0 && ln_ < 2) { return layerEnable2D[ln_]; } else { return false; } }
	void Layer_3D(DWORD ln_, bool st_) { if (ln_ >= 0 && ln_ < 4) { layerEnable3D[ln_] = st_; } }
	bool Layer_3D(DWORD ln_) { if (ln_ >= 0 && ln_ < 4) { return layerEnable3D[ln_]; } else { return false; } }
	//�ꊇ�ݒ�
	void Layer_2D(bool s0_, bool s1_) { layerEnable2D[0] = s0_;	layerEnable2D[1] = s1_; }
	void Layer_3D(bool s0_, bool s1_, bool s2_, bool s3_) { layerEnable3D[0] = s0_;	layerEnable3D[1] = s1_;	layerEnable3D[2] = s2_;	layerEnable3D[3] = s3_; }
	DWORD FrameCnt() { return frameCnt; };

	//�f�o�b�O�@�\��ON/OFF
	void DebugMode(bool dm_) { debugMode = dm_; }

	//������������������������������������������������������������������������������������������
	//�b�����ōŏ��ɓo�ꂷ��^�X�N��Ԃ��i�Ώۂ��P�̂̎������g���Ȃ��j						  �b
	//�b����		�F2014/02/05	�{��N�V	�ύX										�@�b
	//�b����		�F2015/02/10	�{��N�V	�ύX										�@�b
	//�b����		�F2017/01/23	�{��N�V	�ύX										�@�b
	//�b����		�F2020/01/23	�{��N�V	�ύX	(���ȏ��J���[���ɔ������\�b�h�������ɖ߂����j									�@�b
	//������������������������������������������������������������������������������������������
	template <class T>
	shared_ptr<T>  GetTask(
		const  string& gn_	//�O���[�v��
		, const  string& n_		//�ŗL��
	) {
		DWORD  gKey = 0;
		memcpy(&gKey, gn_.c_str(), min(4, gn_.length()));
		for (auto it = tasks_detect.begin(); it != tasks_detect.end(); it++) {
			if ((*it).second == nullptr) { continue; }
			if ((*it).second->TaskStateCnt_Kill() > 0) { continue; }
			if ((*it).second->GroupKey() != gKey) { continue; }
			if ((*it).second->groupName != gn_) { continue; }
			if ((*it).second->name != n_) { continue; }
			return  static_pointer_cast<T>((*it).second);
		}
		//�o�^�҂��^�X�N�ɑ΂��Ă��������s��(2017/01/23)
		for (auto it = add_Tasks.begin(); it != add_Tasks.end(); it++) {
			if ((*it) == nullptr) { continue; }
			if ((*it)->Detectible() == false) { continue; }
			if ((*it)->TaskStateCnt_Kill() > 0) { continue; }
			if ((*it)->GroupKey() != gKey) { continue; }
			if ((*it)->groupName != gn_) { continue; }
			if ((*it)->name != n_) { continue; }
			return  static_pointer_cast<T>((*it));
		}
		return  nullptr;
	}
	//������������������������������������������������������������������������������������������
	//�b�����ōŏ��ɓo�ꂷ��^�X�N��Ԃ��i�Ώۂ��P�̂̎������g���Ȃ��j						  �b
	//�b����		�F2014/02/05	�{��N�V	�ύX										�@�b
	//�b����		�F2015/02/10	�{��N�V	�ύX										�@�b
	//�b����		�F2017/01/23	�{��N�V	�ύX										�@�b
	//�b����		�F2020/01/23	�{��N�V	�ύX	(���ȏ��J���[���ɔ������\�b�h�������ɖ߂����j									�@�b
	//������������������������������������������������������������������������������������������
	template <class T>
	shared_ptr<T> GetTask(
		const  string& gn_		//�O���[�v��
	) {
		DWORD  gKey = 0;
		memcpy(&gKey, gn_.c_str(), min(4, gn_.length()));
		for (auto it = tasks_detect.begin(); it != tasks_detect.end(); it++) {
			if ((*it).second == nullptr) { continue; }
			if ((*it).second->TaskStateCnt_Kill() > 0) { continue; }
			if ((*it).second->GroupKey() != gKey) { continue; }
			if ((*it).second->groupName != gn_) { continue; }
			return  static_pointer_cast<T>((*it).second);
		}
		//�o�^�҂��^�X�N�ɑ΂��Ă��������s��(2017/01/23)
		for (auto it = add_Tasks.begin(); it != add_Tasks.end(); it++) {
			if ((*it) == nullptr) { continue; }
			if ((*it)->Detectible() == false) { continue; }
			if ((*it)->TaskStateCnt_Kill() > 0) { continue; }
			if ((*it)->GroupKey() != gKey) { continue; }
			if ((*it)->groupName != gn_) { continue; }
			return  static_pointer_cast<T>((*it));
		}

		return  nullptr;
	}
	//������������������������������������������������������������������������������������������
	//�b�����ōŏ��ɓo�ꂷ��^�X�N��Ԃ��A����ɓ���̏����ɍ��v����x�N�^�[��Ԃ�			  �b
	//�bDxLib�ԂɂĐV�K�쐬�F�y�c����														�@�b
	//������������������������������������������������������������������������������������������
	template <class T>
	shared_ptr<T>  GetTaskIf(
		const  string& gn_,
		const  string& n_,
		bool   (*chk_)(T& task_),
		DWORD  vectorReserve_ = vrDef,
		DWORD  exclusionKey_ = 0)
	{
		shared_ptr<vector<shared_ptr<T>>>  w =
			shared_ptr<vector<shared_ptr<T>>>(new vector<shared_ptr<T>>());
		w->reserve(vectorReserve_);
		DWORD  gKey = 0;
		memcpy(&gKey, gn_.c_str(), min(4, gn_.length()));

		for (auto it = tasks_detect.begin(); it != tasks_detect.end(); it++) {
			if ((*it).second == nullptr) { continue; }
			if ((*it).second->TaskStateCnt_Kill() > 0) { continue; }
			if ((*it).second->UKey() == exclusionKey_) { continue; }
			if ((*it).second->GroupKey() != gKey) { continue; }
			if ((*it).second->groupName != gn_) { continue; }
			if ((*it).second->name != n_) { continue; }
			//�^�X�N���̏���������
			if (chk_(*(static_pointer_cast<T>((*it).second))) != true) { continue; }
			return  static_pointer_cast<T>((*it).second);
		}
		//�o�^�҂��^�X�N�ɑ΂��Ă��������s��(2017/01/23)
		for (auto it = add_Tasks.begin(); it != add_Tasks.end(); it++) {
			if ((*it) == nullptr) { continue; }
			if ((*it)->Detectible() == false) { continue; }
			if ((*it)->TaskStateCnt_Kill() > 0) { continue; }
			if ((*it)->GroupKey() != gKey) { continue; }
			if ((*it)->groupName != gn_) { continue; }
			if ((*it)->name != n_) { continue; }
			//�^�X�N���̏���������
			if (chk_(*(static_pointer_cast<T>(*it))) != true) { continue; }
			return  static_pointer_cast<T>((*it));
		}
		return w;
	}
	//������������������������������������������������������������������������������������������
	//�b���O���[�v���̑Ώۂ�S�Č��o���A���̃x�N�^�[��Ԃ�									  �b
	//�b����		�F2014/02/05	�{��N�V	�ύX										�@�b
	//�b����		�F2015/02/10	�{��N�V	�ύX										�@�b
	//�b����		�F2017/01/23	�{��N�V	�ύX										�@�b
	//�b����		�F2020/01/23	�{��N�V	�ύX	(���ȏ��J���[���ɔ������\�b�h�������ɖ߂����j									�@�b
	//������������������������������������������������������������������������������������������
	template <class T>
	shared_ptr<vector<shared_ptr<T>>>  GetTasks(
		const  string& gn_,
		const  string& n_,
		DWORD  vectorReserve_ = vrDef,
		DWORD  exclusionKey_ = 0)
	{
		shared_ptr<vector<shared_ptr<T>>>  w =
			shared_ptr<vector<shared_ptr<T>>>(new vector<shared_ptr<T>>());
		w->reserve(vectorReserve_);
		DWORD  gKey = 0;
		memcpy(&gKey, gn_.c_str(), min(4, gn_.length()));

		for (auto it = tasks_detect.begin(); it != tasks_detect.end(); it++) {
			if ((*it).second == nullptr) { continue; }
			if ((*it).second->TaskStateCnt_Kill() > 0) { continue; }
			if ((*it).second->UKey() == exclusionKey_) { continue; }
			if ((*it).second->GroupKey() != gKey) { continue; }
			if ((*it).second->groupName != gn_) { continue; }
			if ((*it).second->name != n_) { continue; }
			w->push_back(static_pointer_cast<T>((*it).second));
		}
		//�o�^�҂��^�X�N�ɑ΂��Ă��������s��(2017/01/23)
		for (auto it = add_Tasks.begin(); it != add_Tasks.end(); it++) {
			if ((*it) == nullptr) { continue; }
			if ((*it)->Detectible() == false) { continue; }
			if ((*it)->TaskStateCnt_Kill() > 0) { continue; }
			if ((*it)->GroupKey() != gKey) { continue; }
			if ((*it)->groupName != gn_) { continue; }
			if ((*it)->name != n_) { continue; }
			w->push_back(static_pointer_cast<T>((*it)));
		}
		return w;
	}

	//������������������������������������������������������������������������������������������
	//�b���O���[�v���̑Ώۂ�S�Č��o���A���̃x�N�^�[��Ԃ�									  �b
	//�b����		�F2014/02/05	�{��N�V	�ύX										�@�b
	//�b����		�F2015/02/10	�{��N�V	�ύX										�@�b
	//�b����		�F2017/01/23	�{��N�V	�ύX										�@�b
	//�b����		�F2020/01/23	�{��N�V	�ύX	(���ȏ��J���[���ɔ������\�b�h�������ɖ߂����j									�@�b
	//������������������������������������������������������������������������������������������
	template <class T>
	shared_ptr<vector<shared_ptr<T>>>  GetTasks(
		const  string& gn_,
		DWORD  vectorReserve_ = vrDef,
		DWORD  exclusionKey_ = 0)
	{
		shared_ptr<vector<shared_ptr<T>>>  w =
			shared_ptr<vector<shared_ptr<T>>>(new vector<shared_ptr<T>>());
		w->reserve(vectorReserve_);
		DWORD  gKey = 0;
		memcpy(&gKey, gn_.c_str(), min(4, gn_.length()));

		for (auto it = tasks_detect.begin(); it != tasks_detect.end(); it++) {
			if ((*it).second == nullptr) { continue; }
			if ((*it).second->TaskStateCnt_Kill() > 0) { continue; }
			if ((*it).second->UKey() == exclusionKey_) { continue; }
			if ((*it).second->GroupKey() != gKey) { continue; }
			if ((*it).second->groupName != gn_) { continue; }
			w->push_back(static_pointer_cast<T>((*it).second));
		}
		//�o�^�҂��^�X�N�ɑ΂��Ă��������s��(2017/01/23)
		for (auto it = add_Tasks.begin(); it != add_Tasks.end(); it++) {
			if ((*it) == nullptr) { continue; }
			if ((*it)->Detectible() == false) { continue; }
			if ((*it)->TaskStateCnt_Kill() > 0) { continue; }
			if ((*it)->GroupKey() != gKey) { continue; }
			if ((*it)->groupName != gn_) { continue; }
			w->push_back(static_pointer_cast<T>((*it)));
		}
		return w;
	}
	//������������������������������������������������������������������������������������������
	//�b���O���[�v���̑Ώۂ�S�Č��o���A����ɓ���̏����ɍ��v����x�N�^�[��Ԃ�			  �b
	//�bDxLib�ԂɂĐV�K�쐬�F�y�c����														�@�b
	//������������������������������������������������������������������������������������������
	template <class T>
	shared_ptr<vector<shared_ptr<T>>>  GetTasksIf(
		const  string& gn_,
		const  string& n_,
		bool   (*chk_)(T& task_),
		DWORD  vectorReserve_ = vrDef,
		DWORD  exclusionKey_ = 0)
	{
		shared_ptr<vector<shared_ptr<T>>>  w =
			shared_ptr<vector<shared_ptr<T>>>(new vector<shared_ptr<T>>());
		w->reserve(vectorReserve_);
		DWORD  gKey = 0;
		memcpy(&gKey, gn_.c_str(), min(4, gn_.length()));

		for (auto it = tasks_detect.begin(); it != tasks_detect.end(); it++) {
			if ((*it).second == nullptr) { continue; }
			if ((*it).second->TaskStateCnt_Kill() > 0) { continue; }
			if ((*it).second->UKey() == exclusionKey_) { continue; }
			if ((*it).second->GroupKey() != gKey) { continue; }
			if ((*it).second->groupName != gn_) { continue; }
			if ((*it).second->name != n_) { continue; }
			//�^�X�N���̏���������
			if (chk_(*(static_pointer_cast<T>((*it).second))) != true) { continue; }
			w->push_back(static_pointer_cast<T>((*it).second));
		}
		//�o�^�҂��^�X�N�ɑ΂��Ă��������s��(2017/01/23)
		for (auto it = add_Tasks.begin(); it != add_Tasks.end(); it++) {
			if ((*it) == nullptr) { continue; }
			if ((*it)->Detectible() == false) { continue; }
			if ((*it)->TaskStateCnt_Kill() > 0) { continue; }
			if ((*it)->GroupKey() != gKey) { continue; }
			if ((*it)->groupName != gn_) { continue; }
			if ((*it)->name != n_) { continue; }
			//�^�X�N���̏���������
			if (chk_(*(static_pointer_cast<T>(*it))) != true) { continue; }
			w->push_back(static_pointer_cast<T>(*it));
		}
		return w;
	}


	virtual bool Initialize() = 0;// (HWND wnd_) = 0;
	virtual void UpDate() = 0;

	//3DPG1�Ή��ɂ��ǉ�
	//2D�`����̃f�t�H���g�p�����[�^�ݒ�
	virtual void Set2DRenderState(DWORD l_) = 0;
	//3D�`����̃f�t�H���g�p�����[�^�ݒ�
	virtual void Set3DRenderState(DWORD l_) = 0;
};
