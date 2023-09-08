#pragma once
#pragma warning(disable:4996)
#pragma once
//--------------------------------------------------------------------------------------------
// ���C���Q�[���p�N���X
//--------------------------------------------------------------------------------------------
#include "../../Engine/myGame/myGame.h"


class GameBase :public BTask
{
	//�ύX�s����������������������������������������
public:
	typedef shared_ptr<GameBase>		SP;
	typedef weak_ptr<GameBase>		WP;
public:
	//�ύX������������������������������������������
	enum class StateOfGame{BeforeGame, InGame, AfterGame};
	StateOfGame stateOfGame;

	GameBase() :
		stateOfGame(StateOfGame::BeforeGame)
	{}

	void UpDate() override;

	virtual void UpDateBeforeGame() {};
	virtual void UpDateGame() {};
	virtual void UpDateAfterGame() {};
};
