#include "GameBase.h"
//--------------------------------------------------------------------------------------------
// ���C���Q�[���p�N���X
//--------------------------------------------------------------------------------------------

void GameBase::UpDate() {
	switch (this->stateOfGame) {
	case StateOfGame::BeforeGame:
		UpDateBeforeGame();
		break;
	case StateOfGame::InGame:
		UpDateGame();
		break;
	case StateOfGame::AfterGame:
		UpDateAfterGame();
		break;
	}
}
