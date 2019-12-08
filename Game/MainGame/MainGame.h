#pragma once

class MainGame : public GameCore
{
public:
	MainGame(Framework* frameWork);
	~MainGame();

	void LoadContent() override;

	virtual void Update(float deltaTime) override;
};