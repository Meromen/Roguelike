#include <iostream>
#include <stdio.h>
#include <olcC++/olcConsoleGameEngineSDL.h>
#include <string>
#include <vector>

#define SCREENHIGHT  50
#define SCREENWIDTH  80

using namespace std;


struct Stats{
	float f_speed = 0;
	float f_hp = 0;
	float f_mp = 0;
	float f_str = 0;
	float f_agl = 0;
	float f_int = 0;
};


class GameObject {
private:
	

public:
	int i_id;
	int i_hight;
	int i_width;
	float f_objX;
	float f_objY;
	string s_type;
	vector<olcSprite*> v_sprites;
	int i_spriteIter;
	bool b_needDelete;
	Stats st_stats;

	GameObject(int id, int h, int w, float x, float y, string type, olcSprite* sprite) {
		i_id = id;
		f_objX = x;
		f_objY = y;
		i_hight = h;
		i_width = w;
		s_type = type;
		v_sprites.push_back(sprite);
		i_spriteIter = 0;
		b_needDelete = false;
	}

	GameObject(int id, int h, int w, olcSprite* sprite) {
		i_id = id;
		i_hight = h;
		i_width = w;
		f_objX = 50.0f;
		f_objY = 50.0f;
		s_type = "UNKNOWN";
		i_spriteIter = 0;
		b_needDelete = false;		
		v_sprites.push_back(sprite);
	}

protected:

	void SetStats(float speed, float hp, float mp, float str, float agl, float inte) {
		this->st_stats.f_speed = speed;
		this->st_stats.f_hp = hp;
		this->st_stats.f_mp = mp;
		this->st_stats.f_str = str;
		this->st_stats.f_agl = agl;
		this->st_stats.f_int = inte;
	}	
};

class GameEng : public olcConsoleGameEngine {
public:
	GameEng() {

	}
	vector<GameObject*> obj_objectList;
	olcSprite *spr_UNW;
	olcSprite *spr_HERO;
	olcSprite *spr_BLAST;

protected:
	virtual bool OnUserCreate() {
		InitSprites();
		obj_objectList.push_back(new GameObject(0, 5, 5, 10, 10, "hero", spr_HERO));	
		

		return true;
	}

	void createNewObj(int id, int h, int w, float x, float y, string type, olcSprite* sprite) {
		obj_objectList.push_back(new GameObject(id, h, w, x, y, type, sprite));
	}

	virtual bool OnUserUpdate(float fElapsedTime) {

		Fill(0, 0, m_nScreenWidth, m_nScreenHeight, L' ');
		for (GameObject* obj : obj_objectList) {

			if (obj->s_type == "hero") {
				if (GetKey(VK_UP).bHeld) {
					obj->f_objY -= 20.0f * fElapsedTime;
				}

				if (GetKey(VK_DOWN).bHeld) {
					obj->f_objY += 20.0f * fElapsedTime;
				}

				if (GetKey(VK_LEFT).bHeld) {
					obj->f_objX -= 20.0f * fElapsedTime;
				}

				if (GetKey(VK_RIGHT).bHeld) {
					obj->f_objX += 20.0f * fElapsedTime;
				}

				if (GetKey(VK_SPACE).bReleased) {
					createNewObj(2, 1, 1, obj->f_objX, obj->f_objY, "blast", spr_BLAST);
					createNewObj(2, 1, 1, obj->f_objX + 5, obj->f_objY + 5 , "blast", spr_BLAST);
				}
			}
			else if (obj->s_type == "blast") {
				obj->f_objX += 40.0f * fElapsedTime;
				if (obj->f_objX > m_nScreenWidth) {
					obj->b_needDelete = true;
				}
			}


			DrawSprite((int)obj->f_objX, (int)obj->f_objY, obj->v_sprites[obj->i_spriteIter]);
		}


		for (vector<GameObject*>::size_type i = 0; i < obj_objectList.size(); i++) {
			if (obj_objectList[i]->b_needDelete) {
				obj_objectList.erase(obj_objectList.begin() + i);
			}
		}

 	return true;
	}

private:
	
	
/*
		___________________
		|      SPRITES	   |
		|__________________|

*/


void InitSprites() {
	wstring ws_sprPattern;

	/* UNKNOWN Sprite
	example:
				???
				???
				???
	*/


	spr_UNW = new olcSprite(3, 3);
	
	
	spr_UNW->SetColour(0, 0, 3);
	spr_UNW->SetColour(1, 0, 3);
	spr_UNW->SetColour(2, 0, 3);
	spr_UNW->SetColour(0, 1, 4);
	spr_UNW->SetColour(1, 1, 5);
	spr_UNW->SetColour(2, 1, 6);
	spr_UNW->SetColour(0, 2, 7);
	spr_UNW->SetColour(1, 2, 8);
	spr_UNW->SetColour(2, 2, 9);
	spr_UNW->SetGlyph(0, 0, L'?');
	spr_UNW->SetGlyph(1, 0, L'?');
	spr_UNW->SetGlyph(2, 0, L'?');
	spr_UNW->SetGlyph(0, 1, L'?');
	spr_UNW->SetGlyph(1, 1, L'?');
	spr_UNW->SetGlyph(2, 1, L'?');
	spr_UNW->SetGlyph(0, 2, L'?');
	spr_UNW->SetGlyph(1, 2, L'?');
	spr_UNW->SetGlyph(2, 2, L'?');

	/*_________________________________*/


	/* HERO Sprite
	example:
				##@##
				-=|=-
				##|##
				#/#\#
				|###|

	(# - empty block)
	*/
	
	ws_sprPattern += L"##@##";
	ws_sprPattern += L"-=|=-";
	ws_sprPattern += L"##|##";
	ws_sprPattern += L"#|#|#";
	ws_sprPattern += L"|###|";
	int heroWidth = 5;
	int heroHight = 5;
	spr_HERO = new olcSprite(heroWidth, heroHight);
	for (int y = 0; y < heroHight; y++)
		for (int x = 0; x < heroWidth; x++) {
			spr_HERO->SetGlyph(x, y, ws_sprPattern[y * heroWidth + x]);
			if (ws_sprPattern[y * heroWidth + x] == L'@') {
				spr_HERO->SetColour(x, y, 0x000D);
			}
			else
				if (ws_sprPattern[y * heroWidth + x] == L'#') {
					spr_HERO->SetColour(x, y, 0x0000);
				}
				else {
					spr_HERO->SetColour(x, y, 0x0007);
				}
		}
	/*_________________________________*/

	/*  BLAST Sprite
		example:

				๛
		(0x0E5B)
	*/

	spr_BLAST = new olcSprite(1, 1);
	spr_BLAST->SetColour(0, 0, 0x00C0);
	spr_BLAST->SetGlyph(0, 0, L'๛');
	//___________________________________________________________________
};

};


int main(){		
	GameEng game;
	game.ConstructConsole(SCREENWIDTH, SCREENHIGHT, 8, 8);	
	game.Start();
	

	

	return 0;
}
