#include "myGame.h"
#include <time.h>
#include <vector>
#include <algorithm>
#include <functional>
#include <fstream>
//#include "PerformanceCounter.h"	//���s���x�𒲂ׂ�

DWORD BTask::uKeyManager = 10000;

int BData::argManager = 0;
BData::BData() :
	myArg(argManager)
{
	++argManager;
}

bool myEngine::B_Initialize()//(HWND wnd_)
{
	//	�^�C�}�[����\��1�~���b�ɐݒ�
	timeBeginPeriod(1);
	//debugMode = false;
	Initialize();//(wnd_);
	return true;
}

void myEngine::B_Step()//(HWND wnd_)
{
	this->renderCnt += this->renderCall;
	this->updateCnt += this->updateCall;

	if (this->updateCnt >= this->renderCall) {
		this->updateCnt -= this->renderCall;

		//	ESC�L�[�������ꂽ	�I���v���t���O���n�m
		if (GetAsyncKeyState(VK_ESCAPE)) { quitRequest = true; }

		//	�^�X�N�̃X�e�b�v����
		this->UpDate();
		//	�o�^�ς݃^�X�N�̃A�b�v�f�[�g�������Ăяo��
		this->B_StepSub_CallTaskUpDate();
		//	�ǉ��o�^���ꂽ�^�X�N�𔽉f������
		this->B_StepSub_Entry();
		//	���ŏ�Ԃ̃^�X�N����菜��
		this->B_StepSub_DeleteTask();
	}

	if (this->renderCnt >= this->updateCall) {
		this->renderCnt -= this->updateCall;

		//�R�c�̕`��v���C�I���e�B���X�V����
		this->B_UpDateSub_CallTaskUpDate3DPriority();

		////�`��J�n
		//this->dgi->Begin(this->dgi->EffectState().param.bgColor);
		{
			//2D�`��i3D�w�ʁj
			this->B_StepSub_CallTaskRender2D_BF();
			//���C���[���ɕ`��
			this->B_StepSub_CallTaskRender3D_4();
			//2D�`��(3D�`���j
			this->B_StepSub_CallTaskRender2D_AF();
			//�f�o�b�O�\��
			this->B_StepSub_DebugOut();
		}
		//�t���b�v
		ScreenFlip();
		//����ʃ��Z�b�g
		ClearDrawScreen();
		////�`��I��
		//this->dgi->End();
	}

	//	�^�X�N�����݂��Ȃ��ꍇ�I���Ƃ���
	//	�I���v�������鎞�@�E�B���h�E�̔j�����s��
	if ((tasks_detect.size() == 0 && tasks_undetect.size() == 0 && add_Tasks.size() == 0)
		|| quitRequest == true) {
		//DestroyWindow(wnd_);
		finishGame = true;
	}
	frameCnt++;
}

//�^�X�N�̍X�V����
void  myEngine::B_StepSub_CallTaskUpDate()
{
	for (auto it = tasks_detect.begin(); it != tasks_detect.end(); ++it) {
		(*it).second->T_Step();
	}
	for_each(tasks_undetect.begin(), tasks_undetect.end(), mem_fn(&BTask::T_Step));

}

//3D�`��̍X�V����
void  myEngine::B_UpDateSub_CallTaskUpDate3DPriority()
{
	for (auto it = tasks_detect.begin(); it != tasks_detect.end(); ++it) {
		(*it).second->T_UpDate3DPriority();
	}
	for_each(tasks_undetect.begin(), tasks_undetect.end(), mem_fn(&BTask::T_UpDate3DPriority));

}
//�^�X�N�̍X�V�O����
void  myEngine::B_StepSub_Entry()
{
	//	�ǉ����ꂽ�^�X�N�̓o�^
	for (auto it = add_Tasks.begin(); it != add_Tasks.end(); ++it) {
		if ((*it)->Detectible()) {
			pair<DWORD, BTask::SP>  d;
			d.first = (*it)->GroupKey();
			d.second = (*it);
			tasks_detect.push_back(d);
		}
		else { tasks_undetect.push_back((*it)); }
		(*it).reset();
	}
	add_Tasks.erase(add_Tasks.begin(), add_Tasks.end());
}

//3D�`��O�ɍs����2D�`��
void  myEngine::B_StepSub_CallTaskRender2D_BF()
{
	//2D�`��i3D�w�ʁj
	if (this->layerEnable2D[0]) {
		//	�^�X�N�̕`�揈��(2D)
		//first�ɂ͉摜�D��x�iPriority�j�Asecond�ɂ̓^�X�N�ւ̃|�C���^�j
		//�����D��x�̒l���������悤�ɁAmultimap���g���Ă���B
		using dmap = std::pair<float, BTask*>;
		multimap<float, BTask*> draw2DMM;

		//���o�\�ȃ^�X�N
		for (auto it = tasks_detect.begin(); it != tasks_detect.end(); ++it) {
			//�T�X�y���h�łȂ�
			//���Arender2D_Active[0]��true�ȃ^�X�N�����`�揈���Ɋ܂߂�
			if ((*it).second->TaskStateCnt_Suspend() == 0) {
				if ((*it).second->render2D_Active[0]) {
					//�l���������ق�����ɕ`�揈�����s�����߂ɕ��������Ă���j
					//.get()��shared_ptr���ۗL���Ă���|�C���^��Ԃ��Ă����
					draw2DMM.insert(dmap(-(*it).second->render2D_Priority[0], (*it).second.get()));
				}
			}
		}
		//���o�s�\�ȃ^�X�N
		for (auto it = tasks_undetect.begin(); it != tasks_undetect.end(); ++it) {
			if ((*it)->TaskStateCnt_Suspend() == 0) {
				if ((*it)->render2D_Active[0]) {
					draw2DMM.insert(dmap(-(*it)->render2D_Priority[0], (*it).get()));
				}
			}
		}
		//Set2DRenderState(0);
		//this->dgi->Begin2D();
		//�摜�D��x���ɕ��בւ����^�X�N��`��
		for (auto it = draw2DMM.begin(); it != draw2DMM.end(); ++it) {
			(*it).second->T_Draw2D(0);
		}
		//this->dgi->End2D();
	}
}

//���C���[0���珇��3D�`��
void  myEngine::B_StepSub_CallTaskRender3D_4()
{

	for (DWORD lc = 0; lc < 4; ++lc) {
		if (this->layerEnable3D[lc]) {
			//	�^�X�N�̕`�揈��(3D)
			using dmap = std::pair<float, BTask::SP>;
			multimap<float, BTask::SP> draw3DMM;
			multimap<float, BTask::SP> draw3DAPMM;
			//�`�揈�����s�����߂Ƀ^�X�N��z��ɒǉ����鏈��
			//���o�\�ȃ^�X�N��`��z��ɒǉ�
			for (auto it = tasks_detect.begin(); it != tasks_detect.end(); ++it) {
				if ((*it).second->TaskStateCnt_Suspend() == 0) {
					if ((*it).second->render3D_Active[lc]) {
						if (!(*it).second->render3D_Alpha) {
							//���߂Ȃ��F��O���牜�ɕ��ׂ�
							draw3DMM.insert(dmap((*it).second->render3D_Priority[lc], (*it).second));
						}
						else {
							//���߂���F�������O�ɕ��ׂ�
							draw3DAPMM.insert(dmap(-(*it).second->render3D_Priority[lc], (*it).second));
						}
					}
				}
			}
			//���o�s�\�ȃ^�X�N��`��z��ɒǉ�
			for (auto it = tasks_undetect.begin(); it != tasks_undetect.end(); ++it) {
				if ((*it)->TaskStateCnt_Suspend() == 0) {
					if ((*it)->render3D_Active[lc]) {
						if (!(*it)->render3D_Alpha) {
							//���߂Ȃ��F��O���牜�ɕ��ׂ�
							draw3DMM.insert(dmap((*it)->render3D_Priority[lc], (*it)));
						}
						else {
							//���߂���F�������O�ɕ��ׂ�
							draw3DAPMM.insert(dmap(-(*it)->render3D_Priority[lc], (*it)));
						}
					}
				}
			}
			//this->dgi->Begin3D();
			Set3DRenderState(lc);
			//���߂Ȃ��I�u�W�F�N�g�̕`��
			for (auto it = draw3DMM.begin(); it != draw3DMM.end(); ++it) { (*it).second->T_Draw3D(0); }
			//���߂���I�u�W�F�N�g�̕`��
			for (auto it = draw3DAPMM.begin(); it != draw3DAPMM.end(); ++it) { (*it).second->T_Draw3D(0); }
			//�`��͈͂����ɖ߂�
			//this->dgi->End3D();
		}
	}
}

//3D�`���ɍs����2D�`��
void  myEngine::B_StepSub_CallTaskRender2D_AF()
{
	if (this->layerEnable2D[1]) {
		//	�^�X�N�̕`�揈��(2D)
		using dmap = std::pair<float, BTask*>;
		multimap<float, BTask*> draw2DMM;
		for (auto it = tasks_detect.begin(); it != tasks_detect.end(); ++it) {
			if ((*it).second->TaskStateCnt_Suspend() == 0) {
				if ((*it).second->render2D_Active[1]) {
					draw2DMM.insert(dmap(-(*it).second->render2D_Priority[1], (*it).second.get()));
				}
			}
		}
		for (auto it = tasks_undetect.begin(); it != tasks_undetect.end(); ++it) {
			if ((*it)->TaskStateCnt_Suspend() == 0) {
				if ((*it)->render2D_Active[1]) {
					draw2DMM.insert(dmap(-(*it)->render2D_Priority[1], (*it).get()));
				}
			}
		}
		Set2DRenderState(1);
		//this->dgi->Begin2D();
		for (auto it = draw2DMM.begin(); it != draw2DMM.end(); ++it) {
			(*it).second->T_Draw2D(1);
		}
		//this->dgi->End2D();
	}
}


//�f�o�b�O���`��
void  myEngine::B_StepSub_DebugOut()
{
	if (debugMode)
	{
		ML::Color c[] = { ML::Color(1,1,0,0),  ML::Color(1,0,0,1),
							ML::Color(1,0,1,1),  ML::Color(1,0,1,0),
							ML::Color(1,1,2,0),  ML::Color(1,1,0,0),
							ML::Color(1,1,0,1),  ML::Color(1,0,0,1), };
		ML::Vec2 dPos(0, 0);
		int display = 30;	//�����ڕ\�������邩
		{

			//�ŐV"display"�̃^�X�N��\��������
			int  s = max(0, int(tasks_detect.size()) - display);
			int id = s;
			for (auto it = (tasks_detect.begin() + s); it != tasks_detect.end(); ++it) {
				//�󋵂��o��
				stringstream ss;
				ss << id << ">" << (*it).second->groupName << ">" << (*it).second->name << ">" << (*it).second->DebugOut();
				this->debugFont->Draw(dPos, ss.str().c_str(), c[0]);
				//���̕�����\���ʒu��ݒ�
				dPos.y += debugFont->get_size();
				//��ʂɎ��܂�Ȃ��Ȃ�����I��
				if (dPos.y >= (int)screenHeight) { break; }
				++id;
			}
		}
		{
			//�ŐV"display"�̃^�X�N��\��������
			int  s = max(0, int(tasks_undetect.size()) - display);
			int id = s;
			for (auto it = (tasks_undetect.begin() + s); it != tasks_undetect.end(); ++it) {
				//�󋵂��o��
				stringstream ss;
				ss << id << "���o�s�^�X�N>" << (*it)->groupName << ">" << (*it)->name << ">" << (*it)->DebugOut();
				this->debugFont->Draw(dPos, ss.str().c_str(), c[0]);
				//���̕�����\���ʒu��ݒ�
				dPos.y += debugFont->get_size();
				//��ʂɎ��܂�Ȃ��Ȃ�����I��
				if (dPos.y >= (int)screenHeight) { break; }
				++id;
			}
		}
	}

}

//kill��Ԃ̃^�X�N�̍폜
void  myEngine::B_StepSub_DeleteTask()
{
	class cNullCheck {
	public:
		// �^�X�N�����ɉ������Ă���Ȃ�true��Ԃ�
		bool operator()(BTask::SP t_) const {
			bool rtv = t_ == nullptr;
			return  rtv;
		}
	};
	class cNullCheckP {
	public:
		// �^�X�N�����ɉ������Ă���Ȃ�true��Ԃ�
		bool operator()(pair<DWORD, BTask::SP> t_) const {
			bool rtv = t_.second == nullptr;
			return  rtv;
		}
	};

	//���o�\�ȃ^�X�N�̍폜
	{
		for (auto it = tasks_detect.begin(); it != tasks_detect.end(); it++) {	//Kill��Ԃ̑S�Ă��폜����
			if ((*it).second->TaskStateCnt_Kill() > 0) {
				(*it).second.reset();
			}
		}
		//�폜����
		auto endIt = remove_if(tasks_detect.begin(), tasks_detect.end(), cNullCheckP());
		tasks_detect.erase(endIt, tasks_detect.end());	//Kill��Ԃ̑S�Ă��폜����
	}

	//���o�s�\�ȃ^�X�N�̍폜
	{
		for (auto it = tasks_undetect.begin(); it != tasks_undetect.end(); it++) {	//Kill��Ԃ̑S�Ă��폜����
			if ((*it)->TaskStateCnt_Kill() > 0) {
				(*it).reset();
			}
		}
		//�폜����
		auto endIt = remove_if(tasks_undetect.begin(), tasks_undetect.end(), cNullCheck());
		tasks_undetect.erase(endIt, tasks_undetect.end());	//Kill��Ԃ̑S�Ă��폜����
	}
}


myEngine::~myEngine()
{
	//�S�Ẵ^�X�N���������
	for (auto it = this->tasks_detect.begin(); it != this->tasks_detect.end(); ++it) {
		if ((*it).second == nullptr) { continue; }
		(*it).second.reset();
	}
	for (auto it = this->tasks_undetect.begin(); it != this->tasks_undetect.end(); ++it) {
		if ((*it) == nullptr) { continue; }
		(*it).reset();
	}
	for (auto it = add_Tasks.begin(); it != add_Tasks.end(); it++) {
		if ((*it) == nullptr) { continue; }
		(*it).reset();
	}
	this->debugFont.reset();
	//	�^�C�}�[����\�̐ݒ�i1�~���b�P�ʁj����������
	timeEndPeriod(1);
}


bool myEngine::KillAll_GN(const  string& gn_, const  string& n_, bool  nt_)
{
	bool rtv = false;
	for (auto it = tasks_detect.begin(); it != tasks_detect.end(); it++) {	//Kill��Ԃ̑S�Ă��폜����
		if ((*it).second == nullptr) { continue; }
		if ((*it).second->TaskStateCnt_Kill() > 0) { continue; }
		if ((*it).second->groupName != gn_) { continue; }
		if ((*it).second->name != n_) { continue; }
		(*it).second->Kill(nt_);
		rtv = true;
	}
	for (auto it = tasks_undetect.begin(); it != tasks_undetect.end(); it++) {	//Kill��Ԃ̑S�Ă��폜����
		if ((*it) == nullptr) { continue; }
		if ((*it)->TaskStateCnt_Kill() > 0) { continue; }
		if ((*it)->groupName != gn_) { continue; }
		if ((*it)->name != n_) { continue; }
		(*it)->Kill(nt_);
		rtv = true;
	}
	//�o�^�҂��^�X�N�ɑ΂��Ă��������s��(2016/03/22)
	for (auto it = add_Tasks.begin(); it != add_Tasks.end(); it++) {
		if ((*it) == nullptr) { continue; }
		if ((*it)->TaskStateCnt_Kill() > 0) { continue; }
		if ((*it)->groupName != gn_) { continue; }
		if ((*it)->name != n_) { continue; }
		(*it)->Kill(nt_);
	}
	return rtv;
}
bool myEngine::KillAll_G(const  string& gn_, bool  nt_)
{
	bool rtv = false;
	for (auto it = tasks_detect.begin(); it != tasks_detect.end(); it++) {	//Kill��Ԃ̑S�Ă��폜����
		if ((*it).second == nullptr) { continue; }
		if ((*it).second->TaskStateCnt_Kill() > 0) { continue; }
		if ((*it).second->groupName != gn_) { continue; }
		(*it).second->Kill(nt_);
		rtv = true;
	}
	for (auto it = tasks_undetect.begin(); it != tasks_undetect.end(); it++) {	//Kill��Ԃ̑S�Ă��폜����
		if ((*it) == nullptr) { continue; }
		if ((*it)->TaskStateCnt_Kill() > 0) { continue; }
		if ((*it)->groupName != gn_) { continue; }
		(*it)->Kill(nt_);
		rtv = true;
	}
	//�o�^�҂��^�X�N�ɑ΂��Ă��������s��(2016/03/22)
	for (auto it = add_Tasks.begin(); it != add_Tasks.end(); it++) {
		if ((*it) == nullptr) { continue; }
		if ((*it)->TaskStateCnt_Kill() > 0) { continue; }
		if ((*it)->groupName != gn_) { continue; }
		(*it)->Kill(nt_);
	}
	return rtv;
}
//	�����̃^�X�N��S�Ē�~����
bool myEngine::StopAll_GN(const  string& gn_, const  string& n_, bool  m_)
{
	bool rtv = false;
	for (auto it = tasks_detect.begin(); it != tasks_detect.end(); it++) {	//Kill��Ԃ̑S�Ă��폜����
		if ((*it).second == nullptr) { continue; }
		if ((*it).second->TaskStateCnt_Kill() > 0) { continue; }
		if ((*it).second->groupName != gn_) { continue; }
		if ((*it).second->name != n_) { continue; }
		(*it).second->Stop(m_);
		rtv = true;
	}
	for (auto it = tasks_undetect.begin(); it != tasks_undetect.end(); it++) {	//Kill��Ԃ̑S�Ă��폜����
		if ((*it) == nullptr) { continue; }
		if ((*it)->TaskStateCnt_Kill() > 0) { continue; }
		if ((*it)->groupName != gn_) { continue; }
		if ((*it)->name != n_) { continue; }
		(*it)->Stop(m_);
		rtv = true;
	}
	//�o�^�҂��^�X�N�ɑ΂��Ă��������s��(2016/03/22)
	for (auto it = add_Tasks.begin(); it != add_Tasks.end(); it++) {
		if ((*it) == nullptr) { continue; }
		if ((*it)->TaskStateCnt_Kill() > 0) { continue; }
		if ((*it)->groupName != gn_) { continue; }
		if ((*it)->name != n_) { continue; }
		(*it)->Stop(m_);
	}
	return rtv;
}
bool myEngine::StopAll_G(const  string& gn_, bool  m_)
{
	bool rtv = false;
	for (auto it = tasks_detect.begin(); it != tasks_detect.end(); it++) {	//Kill��Ԃ̑S�Ă��폜����
		if ((*it).second == nullptr) { continue; }
		if ((*it).second->TaskStateCnt_Kill() > 0) { continue; }
		if ((*it).second->groupName != gn_) { continue; }
		(*it).second->Stop(m_);
		rtv = true;
	}
	for (auto it = tasks_undetect.begin(); it != tasks_undetect.end(); it++) {	//Kill��Ԃ̑S�Ă��폜����
		if ((*it) == nullptr) { continue; }
		if ((*it)->TaskStateCnt_Kill() > 0) { continue; }
		if ((*it)->groupName != gn_) { continue; }
		(*it)->Stop(m_);
		rtv = true;
	}
	//�o�^�҂��^�X�N�ɑ΂��Ă��������s��(2016/03/22)
	for (auto it = add_Tasks.begin(); it != add_Tasks.end(); it++) {
		if ((*it) == nullptr) { continue; }
		if ((*it)->TaskStateCnt_Kill() > 0) { continue; }
		if ((*it)->groupName != gn_) { continue; }
		(*it)->Stop(m_);
	}
	return rtv;
}
//	�����̃^�X�N��S�Ĉꎞ�����ɂ���
bool myEngine::SuspendAll_GN(const  string& gn_, const  string& n_, bool  m_)
{
	bool rtv = false;
	for (auto it = tasks_detect.begin(); it != tasks_detect.end(); it++) {	//Kill��Ԃ̑S�Ă��폜����
		if ((*it).second == nullptr) { continue; }
		if ((*it).second->TaskStateCnt_Kill() > 0) { continue; }
		if ((*it).second->groupName != gn_) { continue; }
		if ((*it).second->name != n_) { continue; }
		(*it).second->Suspend(m_);
		rtv = true;
	}
	for (auto it = tasks_undetect.begin(); it != tasks_undetect.end(); it++) {	//Kill��Ԃ̑S�Ă��폜����
		if ((*it) == nullptr) { continue; }
		if ((*it)->TaskStateCnt_Kill() > 0) { continue; }
		if ((*it)->groupName != gn_) { continue; }
		if ((*it)->name != n_) { continue; }
		(*it)->Suspend(m_);
		rtv = true;
	}
	//�o�^�҂��^�X�N�ɑ΂��Ă��������s��(2016/03/22)
	for (auto it = add_Tasks.begin(); it != add_Tasks.end(); it++) {
		if ((*it) == nullptr) { continue; }
		if ((*it)->TaskStateCnt_Kill() > 0) { continue; }
		if ((*it)->groupName != gn_) { continue; }
		if ((*it)->name != n_) { continue; }
		(*it)->Suspend(m_);
	}
	return rtv;
}
bool myEngine::SuspendAll_G(const  string& gn_, bool  m_)
{
	bool rtv = false;
	for (auto it = tasks_detect.begin(); it != tasks_detect.end(); it++) {	//Kill��Ԃ̑S�Ă��폜����
		if ((*it).second == nullptr) { continue; }
		if ((*it).second->TaskStateCnt_Kill() > 0) { continue; }
		if ((*it).second->groupName != gn_) { continue; }
		(*it).second->Suspend(m_);
		rtv = true;
	}
	for (auto it = tasks_undetect.begin(); it != tasks_undetect.end(); it++) {	//Kill��Ԃ̑S�Ă��폜����
		if ((*it) == nullptr) { continue; }
		if ((*it)->TaskStateCnt_Kill() > 0) { continue; }
		if ((*it)->groupName != gn_) { continue; }
		(*it)->Suspend(m_);
		rtv = true;
	}
	//�o�^�҂��^�X�N�ɑ΂��Ă��������s��(2016/03/22)
	for (auto it = add_Tasks.begin(); it != add_Tasks.end(); it++) {
		if ((*it) == nullptr) { continue; }
		if ((*it)->TaskStateCnt_Kill() > 0) { continue; }
		if ((*it)->groupName != gn_) { continue; }
		(*it)->Suspend(m_);
	}
	return rtv;
}


void myEngine::PushBack(const BTask::SP& t_)
{
	if (t_) {
		add_Tasks.push_back(t_);
	}
}
