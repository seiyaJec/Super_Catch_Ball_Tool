#pragma once
#include "../Effect2D.h"

//��������������������������������������������������������������������
// �W���G�t�F�N�g
//��������������������������������������������������������������������

namespace Effect2D
{
	class  EffectStd : public  BTask
	{
	//�����E�����n��������������������������������������������������
	public:
		virtual  ~EffectStd();
		typedef  shared_ptr<EffectStd>		SP;
		typedef  weak_ptr<EffectStd>		WP;
		Resource::SP	res;
		//�G�t�F�N�g��������
		static EffectStd::SP Create(string imgFilePath_, ML::Size2 size_, bool flagGameEnginePushBack_);
	
	protected:

		EffectStd();
		bool  B_Initialize(string imgFilePath_, ML::Size2 imgSize_);
		bool  B_Finalize();
		bool  Initialize(string imgFilePath_, ML::Size2 imgSize_);	//�u�������v�^�X�N�������ɂP�񂾂��s������
		virtual bool  InitializeCustom();							//�ǉ��̏���������
		void  UpDate()			override;//�u���s�v�P�t���[�����ɍs������
		void  Render3D_L0()		override;//�u3D�`��v�P�t���[�����ɍs�������A���C���[0
		void  Render2D_AF()		override;//�u2D�`��v�P�t���[�����ɍs�������A3D�`���ɍs����
		bool  Finalize();		//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	
		
	//���C���̃����o�E���\�b�h������������������������������������������
	
	//�[�[����J�[�[//
	protected:
		ML::Image::SP img;		//���g���g���Ă���摜�ւ̃L�[

		ML::Vec2 pos;				//���W
		ML::Vec2 moveVec;			//�ړ�����
		ML::Transform transform;	//�ό`���
		int life;					//��������
		ML::Color color;			//�F���
		void CountLife();			//�������ԃJ�E���g

		//���[�v
		struct Loop
		{
			float adds;				//�ǉ�����l
			float lpmax;				//�ő�
			float lpmin;				//�ŏ�
			Loop();
			void Set(float adds_, float lpmax_, float lpmin_);	//�ꊇ�ݒ�
			void Addto(float& addTo_);						//���[�v
		};
		Loop	loopRotate;			//�ǉ���������l�i�p�x�j
		Loop	loopScaleX;			//�ǉ���������l�iX�X�P�[���j
		Loop	loopScaleY;			//�ǉ���������l�iY�X�P�[���j
		Loop	loopAddAlpha;		//�ǉ���������l�i�����x

	//�[�[���J�[�[//
	public:
		//�Z�b�^
		//���W
		void setPos(const ML::Vec2& pos_) {
			this->pos = pos_;
		}
		//�ړ��x�N�g��
		void setMoveVec(const ML::Vec2& moveVec_){
			this->moveVec = moveVec_; 
		}
		//��������
		void setLife(int life_) {
			this->life = life_; 
		}
		//��]
		void setImgRotate(float rotDeg_) {
			this->transform.rotateDeg = rotDeg_;
		}
		//��]�̒��S
		void setImgRotateCenter(ML::Vec2 rotCtPos_) {
			this->transform.rotateCenterPos = rotCtPos_;
		}
		//�X�P�[�����O
		void setImgScale(float scX_, float scY_) {
			this->transform.scaleX = scX_;
			this->transform.scaleY = scY_;
		}
		//���]
		void setImgFlip(bool flipX_, bool flipY_) {
			this->transform.flipX = flipX_;
			this->transform.flipY = flipY_;
		}
		//�F���
		void setColor(const ML::Color& color_) {
			this->color = color_; 
		}
		//��]���[�v�ݒ�
		void setRotateLoop(float adds_, float max_ = 360.0f, float min_ = 0.0f) {
			this->loopRotate.Set(adds_, max_, min_);
		}
		//�����x���[�v�ݒ�
		void setAlphaLoop(float adds_, float max_ = 1.0f, float min_ = 1.0f) {
			this->loopAddAlpha.Set(adds_, max_, min_);
		}
		//�X�P�[��X���[�v�ݒ�
		void setScaleXLoop(float adds_, float max_ = 10.0f, float min_ = 0.0f) {
			this->loopScaleX.Set(adds_, max_, min_);
		}
		//�X�P�[��Y���[�v�ݒ�
		void setScaleYLoop(float adds_, float max_ = 10.0f, float min_ = 0.0f) {
			this->loopScaleY.Set(adds_, max_, min_);
		}

	};
}