#include "Effect2D_Std.h"
#include "../../../../Engine/myPG/myPG.h"

//��������������������������������������������������������������������
// �W���G�t�F�N�g
//��������������������������������������������������������������������


namespace Effect2D
{
	//-------------------------------------------------------------------
	//�u�������v�^�X�N�������ɂP�񂾂��s������
	bool  EffectStd::Initialize(string imgFilePath_, ML::Size2 imgSize_)
	{
		//�X�[�p�[�N���X������
		__super::Initialize(defGroupName, defName, true);
		//���\�[�X�N���X����or���\�[�X���L
		this->res = Resource::Create();

		//���f�[�^������
		auto itr = this->res->images.find(imgFilePath_);

		//�摜���܂�����Ă��Ȃ��ꍇ
		if (itr == this->res->images.end()) {
			//�摜�ǂݍ���
			auto newimg = ML::Image::Create(imgFilePath_, imgSize_);
			if (newimg) {
				this->res->images.insert(std::make_pair(imgFilePath_, newimg));
				this->img = newimg;
			}
			//�ǂݍ��ݎ��s
			else {
				return false;
			}
		}
		//�摜������Ă���ꍇ
		else {
			this->img = itr->second;
		}

		//��]�̒��S��������
		this->setImgRotateCenter(this->img->CenterPos());

		//�ǉ��̏���������
		return this->InitializeCustom();
	}
	//-------------------------------------------------------------------
	//�ǉ��̏����������i�X�[�p�[�N���X�Ȃ̂œ��ɂȂ��j
	bool  EffectStd::InitializeCustom() {
		return true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  EffectStd::Finalize()
	{
		//���f�[�^���^�X�N���


		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//�������p���^�X�N�̐���

		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  EffectStd::UpDate()
	{
		this->pos += moveVec;
		
		//���[�v
		this->loopRotate.Addto(this->transform.rotateDeg);
		this->loopScaleX.Addto(this->transform.scaleX);
		this->loopScaleY.Addto(this->transform.scaleY);

		this->loopAddAlpha.Addto(this->color.a);
		this->CountLife();
	}
	//-------------------------------------------------------------------
	//�u�R�c�`��v�P�t���[�����ɍs�������A���C���[0
	void EffectStd::Render3D_L0()
	{

	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs�������A3D�`���ɍs����
	void  EffectStd::Render2D_AF()
	{
		this->img->DrawTransform(this->pos, this->transform, true, this->color.a);
	}
	//-------------------------------------------------------------------
	//�֐���`
	//-------------------------------------------------------------------
	//�R���X�g���N�^
	EffectStd::Loop::Loop()
		:adds(0.0f)
		, lpmax(9999.0f)
		, lpmin(-9999.0f)
	{}
	//���[�v�ݒ�
	void EffectStd::Loop::Set(float adds_, float lpmax_, float lpmin_) {
		adds = adds_;
		lpmax = lpmax_;
		lpmin = lpmin_;
	}
	//�������ԃJ�E���g
	void EffectStd::CountLife() {
		--this->life;
		if (this->life <= 0) {
			this->Kill();
		}
	}
	//���[�v
	void EffectStd::Loop::Addto(float& addto_) {
		if (this->adds > 0.0f) {
			addto_ = min(addto_ + this->adds, this->lpmax);
		}
		else if (this->adds < 0.0f) {
			addto_ = max(addto_ + this->adds, this->lpmin);
		}
	}

	//������������������������������������������������������������������������������������
	//�ȉ��͊�{�I�ɕύX�s�v�ȃ��\�b�h
	//������������������������������������������������������������������������������������
	//�G�t�F�N�g��������
	EffectStd::SP EffectStd::Create(string imgFilePath_, ML::Size2 size_, bool flagGameEnginePushBack_) {
		EffectStd::SP ob = EffectStd::SP(new EffectStd());
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
	//-------------------------------------------------------------------
	bool  EffectStd::B_Initialize(string imgFilePath_, ML::Size2 size_)
	{
		return  this->Initialize(imgFilePath_, size_);
	}
	//-------------------------------------------------------------------
	EffectStd::~EffectStd() { this->B_Finalize(); }
	bool  EffectStd::B_Finalize()
	{
		auto  rtv = this->Finalize();
		return  rtv;
	}
	//-------------------------------------------------------------------
	EffectStd::EffectStd()
		:img(nullptr)
		, pos()
		, moveVec()
		, life(100)
		, transform()
		, color()
		, loopRotate()
		, loopScaleX()
		, loopScaleY()
		, loopAddAlpha()
	{	}

}