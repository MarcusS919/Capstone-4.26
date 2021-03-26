// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class CAPSTONE_API PathFindingNode
{
public:
	PathFindingNode();
	PathFindingNode(int arrayX_, int arrayY_, FVector worldPosition, bool isWall);
	~PathFindingNode();

	int arrayX;
	int arrayY;

	bool IsObstacle;
	PathFindingNode* ParentNode;
	FVector Position;

	int GetGCost();
	void SetGCost(int value);
	int GetHCost();
	void SetHCost(int value);
	int GetFCost();

private:
	int GCost;
	int HCost;
};
