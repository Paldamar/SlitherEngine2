#pragma once


class MainGame : public GameCore
{
public:
	MainGame(Framework* frameWork);
	~MainGame();

	void LoadContent() override;
};