#include "StdAfx.h"
#include "JPMiniGame.h"
#include <assert.h>

JPMiniGame::JPMiniGame(WindowsGameRuntime r)
	: runtime(r)
	, blocksNum(cColumns * cRows)
	, blockWidth(r.GetWidth() / cColumns)
	, blockHeight(r.GetHeight() / cRows)
{
	blocksCoords = new Rect[blocksNum];
	blocks = new int[blocksNum];
}

JPMiniGame::~JPMiniGame() override
{
	delete [] blocks;
	delete [] blocksCoords;
}

void JPMiniGame::Initialize()
{
	srand(time(NULL));
	selectedBlock = -1;
	calcBlocks();
	mixBlocks();
}

void JPMiniGame::Click(float x, float y)
{
	const int otherBlock = blockNum(x, y);
	// не двигать больше уже поставленные на место
	if (otherBlock == blocks[otherBlock])
	{
		return;
	}

	// выбрать, если ничего не выбрано
	if (-1 == selectedBlock)
	{
		selectedBlock = otherBlock;
		return;
	}
	swapBlocks(selectedBlock, otherBlock);
	selectedBlock = -1;
}

bool JPMiniGame::IsComplete() const
{
	for (int i = 0; i < blocksNum; ++i)
	{
		if (i != blocks[i])
		{
			return false;
		}
	}
	return true;
}

/* virtual */ void JPMiniGame::Render() const
{
	for (int i = 0; i < blocksNum; ++i)
	{
		Rect texRect = normalizeTexCoords(blockRect(blocks[i]));
		Rect scrRectOrigin = blockRect(i);
		int indent;
		if (blocks[i] == i)	// правильно поставленные идут без отступа
		{
			indent = 0;
		}
		else if (blocks[i] == selectedBlock)	// выделенные с небольшим оступом
		{
			indent = 2;
		}
		else						// у всех остальных отступ большой
		{
			indent = 5;
		}

		scrRectOrigin.left += indent;
		scrRectOrigin.top += indent;
		scrRectOrigin.right -= indent;
		scrRectOrigin.bottom -= indent;

		::Render(scrRectOrigin, cTextureId, texRect);
	}
}

void JPMiniGame::calcBlocks()
{
	for (int row = 0; row < cRows; ++row)
	{
		for (int column = 0; column < cColumns; ++column)
		{
			const int currentBlock = column + row * cColumns;
			assert(currentBlock < blocksNum);

			blocksCoords[currentBlock].left = column * blockWidth;
			blocksCoords[currentBlock].top = row * blockHeight;
			blocksCoords[currentBlock].right = (column + 1) * blockWidth;
			blocksCoords[currentBlock].bottom = (row + 1) * blockHeight;

			blocks[currentBlock] = currentBlock;
		}
	}
}

void JPMiniGame::mixBlocks()
{
	for (int i = 0; i < blocksNum; ++i)
	{
		const int r = rand() % blocksNum;
		swapBlocks(i, r);
	}
}

const Rect& JPMiniGame::blockRect(int num) const
{
	return blocksCoords[blocks[num]];
}

int JPMiniGame::blockNum(float x, float y) const
{
	const int row = int(y / blockHeight);
	const int column = int(x / blockWidth);
	return column + row * cColumns;
}

void JPMiniGame::swapBlocks(const int left, const int right)
{
	if (left != right)
	{
		const int t = blocks[left];
		blocks[left] = blocks[right];
		blocks[right] = t;
	}
}

// преобразует из координат в пикселах в координаты [0..1]
// причём (0, 0) - верхний левый угол картинки, а не нижний
const Rect& JPMiniGame::normalizeTexCoords(const Rect & origin) const
{
	static Rect rect;
	rect.left = origin.left / runtime.GetWidth();
	rect.top = (runtime.GetHeight() - origin.top) / runtime.GetHeight();
	rect.right = origin.right / runtime.GetWidth();
	rect.bottom = (runtime.GetHeight() - origin.bottom) / runtime.GetHeight();
	return rect;
}