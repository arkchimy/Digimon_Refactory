#pragma once
#include "stdafx.h"

#define 성장기 0x0001
#define 보스 0x0010

#define 배틀종료 false
#define 배틀시작 true

#define IDLE		 0
#define Walk		 1
#define Action		 2
#define Skill		 3
#define Hit			 4
#define Victory      5
#define Death		 6
#define PowerUP		 7

#define Sprite_Type  8 //   Idle /Walk / Action / Skill / Hit / Victory /Death

typedef struct
{
	wstring ImgFile;
	int Sprite_cnt;
	float Width, Height;
}Sprite_Info; // 

typedef struct Sprites_Info
{	//메모리 절약을 위한 비트사용
public:
	unsigned int idle_size : 5;
	unsigned int Walk_size : 4;

	unsigned int Action_size : 4;
	unsigned int Skill_size : 4;      // 
	unsigned int Hit_size : 4;		  // 피격 모션 Sprite cnt
	unsigned int victory_size : 5;	  // 승리포즈 Sprite cnt
	unsigned int Death_size : 4;      // 죽음 Sprite Cnt


	unsigned int Fire_Timing_Idx : 4; // 몇번째 Fire idx에서 Bullet이 발사되는지
	unsigned int Fire_Type : 2;		  // 총알의 타입
	unsigned int Bullet_Cnt : 3;
	unsigned int Skill_Bullet_Cnt : 3;

	unsigned int Skill_Timing_Idx : 4; // 몇번째 Fire idx에서 Bullet이 발사되는지
	unsigned int Skill_Type : 2;		  // 총알의 타입

	float Fire_interval;   // 공격 텀
	float Skill_percentage; // 스킬 발동확률

	wstring Normal_Sprite_File;
	wstring Skill_Sprite_File;
	wstring Skill_CutScene;
	wstring Next_digimon;

}Sprites_Info;

static Sprites_Info Guilmon
{
					 11, // Idle
					 8,  // Walk
					 14, // Action
					  8,  //Skill
					  2, // Hit Sprite
					  12, // Victory Sprite
					  14, // Death Sprite Cnt

					 8,  // Fire_Timing                                                             
					 0,  // Fire Type
					 1,
					 1,

					 5,  // Skill Timing
					 0,  //Skill_Type


					 0.3f, // Fire_interval
					 10.f,  // Skill_Percent

					 Digimon_Folder + L"길몬_브레스.png",
					 Digimon_Folder + L"길몬_스킬브레스.png",
					 L"Guilmon_Cut.png",
					 Digimon_Folder + L"가르고몬.png",

};
static Sprites_Info Renamon
{
					 11, // Idle
					 8,  // Walk
					 9, // Action
					 9,  //Skill
					 6, // Hit Sprite
					 15, // Victory Sprite
					 12, // Death Sprite Cnt

					 4,  // Fire_Timing                                                             
					 1,  // Fire Type
					 1,
					 5,  // Bullet_Cnt

					 5,  // Skill Timing
					 1,  //Skill_Type


					 0.3f, // Fire_interval
					 10.f,  // Skill_Percent

					 Digimon_Folder + L"레나몬_브레스.png",
					 Digimon_Folder + L"레나몬_스킬브레스.png",
					 L"Renamon_Cut.png",
					 Digimon_Folder + L"가르고몬.png",
};

static vector<D3DXVECTOR4> Renamon_UV
{
	//IDLE
	{0,0,40,37},

	//Walk
	{0,43,45,75},// 시작x ,시작y x간격 y끝부분

	//Action
	{0,80,60,114},

	//Skill
	{0,120,50,167},
	//Hit
	{0,185,40,220},
	//Victory
	{0,220,40,260},
	//Death
	{0,267,50,296},
	//PowerUP
	{0,220,40,260},
};
static Sprites_Info Terriermon
{
					 22, // Idle
					 6,  // Walk
					 9, // Action
					 14,  //Skill
					 8, // Hit Sprite
					 18, // Victory Sprite
					 13, // Death Sprite Cnt

					 6,  // Fire_Timing                                                             
					 1,  // Fire Type
					 1,  // Bullet_Cnt
					 1,  // SkillBullet

					 11,  // Skill Timing
					 1,  //Skill_Type


					 0.3f, // Fire_interval
					 10.f,  // Skill_Percent

					 Digimon_Folder + L"테리어몬_브레스.png",
					 Digimon_Folder + L"테리어몬_스킬브레스.png",
					 L"Terriermon_Cut.png",
					 Digimon_Folder + L"가르고몬.png",
};
static vector<D3DXVECTOR4> Terriermon_UV
{
	//IDLE
	{0,0,40,40},

	//Walk
	{0,40,40,80},// 시작x ,시작y x간격 y끝부분

	//Action
	{0,80,40,120},

	//Skill
	{0,120,50,170},
	//Hit
	{0,170,50,220},
	//Victory
	{0,220,50,270},
	//Death
	{0,270,50,320},
	//PowerUP
	{0,220,50,270},
};
static Sprites_Info Galgomon
{
					 2, // Idle
					 2,  // Walk
					 3, // Action
					 3,  //Skill
					 3, // Hit Sprite
					 5, // Victory Sprite
					 2, // Death Sprite Cnt

					 2,  // Fire_Timing                                                             
					 2,  // Fire Type
					 3,  // Bullet_Cnt
					 1,

					 2,  // Skill Timing
					 1,  //Skill_Type


					 0.3f, // Fire_interval
					 0.f,  // Skill_Percent

					 Digimon_Folder + L"가르고몬_브레스.png",
					 Digimon_Folder + L"테리어몬_스킬브레스.png",
					 L"Terriermon_Cut.png",
					 Digimon_Folder + L"가르고몬.png",
};
static vector<D3DXVECTOR4> Galgomon_UV
{
	//IDLE
	{0,0,50,50},

	//Walk
	{0,50,50,100},// 시작x ,시작y x간격 y끝부분

	//Action
	{0,100,50,150},

	//Skill
	{0,150,50,200},
	//Hit
	{0,150,50,200},
	//Victory
	{0,200,50,250},
	//Death
	{0,200,50,250},
	//PowerUP
	{0,200,50,250},
};