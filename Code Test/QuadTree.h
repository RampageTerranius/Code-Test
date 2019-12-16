#pragma once



class QuadTree
{
public:
	QuadTree(int splits);

private:
	int totalSplits;

};

QuadTree::QuadTree(int splits)
{
	totalSplits = splits;
}