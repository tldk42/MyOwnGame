#pragma once

#define WIN32_LEAN_AND_MEAN

#include "Game.h"

class Spacewar : public Game
{
public:
	Spacewar();

	virtual ~Spacewar();

	void Initialize(HWND hwnd) override;

#pragma region Game 순수 가상
	void Update() override;
	void AI() override;
	void Collisions() override;
	void Render() override;
#pragma endregion

	void ReleaseAll() override;
	void ResetAll() override;

private:
};
