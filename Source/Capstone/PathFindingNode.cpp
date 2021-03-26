// Fill out your copyright notice in the Description page of Project Settings.


#include "PathFindingNode.h"

PathFindingNode::PathFindingNode():arrayX(0), arrayY(0),IsObstacle(false),ParentNode(nullptr),GCost(0),HCost(0) {
	
}


PathFindingNode::PathFindingNode(int arrayX_, int arrayY_, FVector worldPosition, bool isWall) {
	arrayX = arrayX_;
	arrayY = arrayY_;
	Position = worldPosition;
	IsObstacle = isWall;
}


PathFindingNode::~PathFindingNode(){

}

int PathFindingNode::GetGCost()
{
	return GCost;
}

void PathFindingNode::SetGCost(int value)
{
	GCost = value;
}

int PathFindingNode::GetHCost()
{
	return HCost;
}

void PathFindingNode::SetHCost(int value)
{
	HCost = value;
}

int PathFindingNode::GetFCost()
{
	return GCost + HCost;
}
