// Fill out your copyright notice in the Description page of Project Settings.

#include "RPGSaveGame.h"


URPGSaveGame::URPGSaveGame()
{
	PlayerName = TEXT("Default");
	LevelName = TEXT("Default");

	UserIndex = 0;

	EquipmentItem.Init(-1, 10);
}

