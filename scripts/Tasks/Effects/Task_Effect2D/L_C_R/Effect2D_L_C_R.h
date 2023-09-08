#pragma warning(disable:4996)
#pragma once
//��������������������������������������������������������������������
//�����������E�̏��ňړ�����G�t�F�N�g
//��������������������������������������������������������������������
#include "../Std/Effect2D_Std.h"

namespace  Effect2D
{
	class EffectL_C_R :public EffectStd
	{
		//�����E���s�֘A����������������������������������������������������
	public:
		~EffectL_C_R();
		typedef shared_ptr<EffectL_C_R>		SP;
		typedef weak_ptr<EffectStd>			WP;
		static EffectL_C_R::SP Create(string imgFilePath_, ML::Size2 size_, bool flagGameEnginePushBack_);
	protected:
		EffectL_C_R();
		bool InitializeCustom() override;	//����������
		void UpDate() override;	//�X�V����


		//�ǉ������o�E���\�b�h����������������������������������������������
	protected:
		float moveSpeed;		//�ړ����x
		float moveSpeedMin;		//�ړ����x�ŏ�
		int timeStayCenter;	//�����ɑ؍݂��鎞��
		int timeCnt;		//���Ԍv��
		bool passedCenter;		//�����ʉߍς݂�
		
		bool IsOutOfScreen();			//��ʉE���֍s���A�����Ȃ��Ȃ�����
		bool IsPassedCenterOfScreen();	//��ʒ�����ʉ߂�����
		
		//�ݒ�~�X������邽�߂ɕK�v�̂Ȃ��֐����B��
		using EffectStd::setLife;
		using EffectStd::setMoveVec;
		using EffectStd::setPos;
	public:
		//�Z�b�^
		void setMoveSpeed(float moveSpeed_);
		void setTimeStayCenter(int timeStayCenter_);
	};
}