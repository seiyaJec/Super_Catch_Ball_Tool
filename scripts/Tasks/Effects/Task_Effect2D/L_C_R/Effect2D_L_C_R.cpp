//��������������������������������������������������������������������
//�����������E�̏��ňړ�����G�t�F�N�g
//��������������������������������������������������������������������
#include  "../../../../Engine/myPG/myPG.h"
#include  "Effect2D_L_C_R.h"

namespace  Effect2D
{
	//�R���X�g���N�^
	EffectL_C_R::EffectL_C_R()
		:EffectStd()
		, moveSpeed(10)
		, moveSpeedMin(1)
		, timeStayCenter(60)
	{}

	//�f�X�g���N�^
	EffectL_C_R::~EffectL_C_R() {}

	//--------------------------------------------------------
	//����������
	bool EffectL_C_R::InitializeCustom() {
		ML::Size2 imgSize = this->img->get_size();
		this->pos.x = imgSize.w / -2;
		this->pos.y = (float)ge->screenHeight / 2.0f;
		this->moveVec.x = (float)moveSpeed;

		return true;
	}
	//--------------------------------------------------------
	//�X�V����
	void EffectL_C_R::UpDate() {
		
		//��ʒ����ɂ��邩
		if (this->IsPassedCenterOfScreen() == false) {
			this->pos += this->moveVec;
		}
		else {
			if (this->timeCnt >= this->timeStayCenter) {
				this->pos += this->moveVec;
			}
			++timeCnt;
		}


		//��ʊO�ɏo����L��
		if (this->IsOutOfScreen() == true) {
			this->Kill();
		}
		
	}

	//--------------------------------------------------
	//���̑����\�b�h
	//--------------------------------------------------
	//��ʉE���֍s�������Ȃ��Ȃ�����
	bool EffectL_C_R::IsOutOfScreen() {
		return this->pos.x - this->img->get_size().w * this->transform.scaleX >= ge->screenWidth;
	}
	//��ʒ�����ʉ߂�����
	bool EffectL_C_R::IsPassedCenterOfScreen() {
		ML::Vec2 screenCenter = ge->screenCenter();
		return
			this->pos.x >= screenCenter.x
			&& this->pos.y >= screenCenter.y;
	}

	//--------------------------------------------------------
	//�Z�b�^
	void EffectL_C_R::setMoveSpeed(float moveSpeed_) {
		if (moveSpeed_ <= 0) {
			string debug("EffectL_C_R::setMoveSpeed>>�s���ȃf�[�^�l�ł��I");
			ge->printToDebugFile(debug);
		}
		this->moveSpeed = moveSpeed_;
		this->moveVec.x = moveSpeed_;
	}
	void EffectL_C_R::setTimeStayCenter(int timeStayCenter_) {
		if (timeStayCenter_ < 0) {
			string debug("EffectL_C_R::setMoveSpeed>>�s���ȃf�[�^�l�ł��I");
			ge->printToDebugFile(debug);
		}
		this->timeStayCenter = timeStayCenter_;
	}


	//--------------------------------------------------------
	//�G�t�F�N�g��������
	EffectL_C_R::SP EffectL_C_R::Create(string imgFilePath_, ML::Size2 size_, bool flagGameEnginePushBack_) {
		EffectL_C_R::SP ob = EffectL_C_R::SP(new EffectL_C_R());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//�Q�[���G���W���ɓo�^

			}
			if (!ob->B_Initialize(imgFilePath_, size_)) {
				ob->Kill();//�C�j�V�����C�Y�Ɏ��s������Kill
			}
			return  ob;
		}
		return nullptr;
	}

}