#pragma once
#include <time.h>
#include "MiniGame.h"
#include "WindowsGameRuntime.h"

class JPMiniGame :
	public MiniGame
{
public:
	JPMiniGame(WindowsGameRuntime r);
	~JPMiniGame(void);
	void Initialize();
	void Click(float x, float y);
	bool IsComplete() const;
	void Render() const;

private:
	void calcBlocks();
	void mixBlocks();
	const Rect & blockRect(const int num) const;
	int blockNum(const float x, const float y) const;
	void swapBlocks(const int left, const int right);
	const Rect & normalizeTexCoords(const Rect & origin) const;

	WindowsGameRuntime runtime;
	const int blocksNum;
	const float blockWidth;
	const float blockHeight;
	Rect * blocksCoords;
	int * blocks;

	int selectedBlock; // -1 for none
};

