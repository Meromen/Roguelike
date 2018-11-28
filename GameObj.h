
#include <iostream>
#include <stdio.h>
#include <olcC++/olcConsoleGameEngineSDL.h>
#include <string>
#include <vector>




#define HERO obj_objectList[0]
#define STRANGE_HP_SCALE 13
#define ITELECT_MP_SCALE 13
#define AGILITY_SPEED_SCALE 3
#define EXP_NEED_FOR_LEVEL 1000
#define STRANGE_DMG_SCALE 5
#define EXP_BOUNTY 300

using namespace std;


struct Stats {
    int i_lvl = 0;
    int i_exp = 0;
    float f_speed = 0;
    float f_hp = 0;
    float f_mp = 0;
    float f_str = 0;
    float f_agl = 0;
    float f_int = 0;
};

struct Obj {
    int i_id = 0;
    olcSprite* spr_sprite;
    Stats* st_stats;
};

Stats* newStats(float agl, float str, float inte) {
    Stats* temp = new Stats;
    temp->f_agl = agl;
    temp->f_str = str;
    temp->f_int = inte;
    return temp;
};

Obj* newObj(int id, olcSprite* sprite, Stats* stats) {
    Obj* temp = new Obj;
    temp->i_id = id;
    temp->st_stats = stats;
    temp->spr_sprite = sprite;

    return temp;
}


struct Status{
    float f_curHP = 0;
    float f_curMP = 0;
};

class GameObject {
private:
    vector<Obj*> v_enemiesList;
    vector<Obj*> v_heroesList;
    vector<Obj*> v_blastsList;

    olcSprite *spr_UNW_0;
    olcSprite *spr_HERO_0;
    olcSprite *spr_BLAST_0;
    olcSprite *spr_ENEMY_0;

/*
		___________________
		|      SPRITES	   |
		|__________________|
*/

    void spriteCreate(int w, int h, wstring pattern, olcSprite *targetSprite) {
        for (int y = 0; y < h; y++)
            for (int x = 0; x < w; x++) {
                targetSprite->SetGlyph(x, y, pattern[y * w + x]);
                if (pattern[y * w + x] == L'@') {
                    targetSprite->SetColour(x, y, 0x000D);
                }
                else
                if (pattern[y * w + x] == L'#') {
                    targetSprite->SetColour(x, y, 0x0000);
                }
                else
                if (pattern[y * w + x] == L'?') {
                    targetSprite->SetColour(x, y, rand() % 16);
                }
                else {
                    targetSprite->SetColour(x, y, 0x0007);
                }
            }
    };


    void initSprites() {
        wstring ws_sprPattern;
        int spriteWidth = 0;
        int spriteHight = 0;

        /* UNKNOWN Sprite
        example:
                    ???
                    ???
                    ???
        */

        ws_sprPattern = L"?????????";
        spriteHight = 3;
        spriteWidth = 3;
        spr_UNW_0 = new olcSprite(spriteWidth, spriteHight);

        spriteCreate(spriteWidth, spriteHight, ws_sprPattern, spr_UNW_0);

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

        ws_sprPattern  = L"##@##";
        ws_sprPattern += L"-=|=-";
        ws_sprPattern += L"##|##";
        ws_sprPattern += L"#|#|#";
        ws_sprPattern += L"|###|";
        spriteWidth = 5;
        spriteHight = 5;
        spr_HERO_0 = new olcSprite(spriteWidth, spriteWidth);
        spriteCreate(spriteWidth, spriteHight, ws_sprPattern, spr_HERO_0);

        /*_________________________________*/

        /*  BLAST Sprite
            example:
                    ๛
            (0x0E5B)
        */

        spr_BLAST_0 = new olcSprite(1, 1);
        spr_BLAST_0->SetColour(0, 0, 0x00C0);
        spr_BLAST_0->SetGlyph(0, 0, L'๛');

        /*_________________________________*/

        /*  ENEMY Sprite
            example:
                    #@^@#
                    >-+-<
                    ^^|^^
                    |#|#|
                    |#|#|
            (0x0E5B)
        */

        ws_sprPattern  = L"#@^@#";
        ws_sprPattern += L">-+-<";
        ws_sprPattern += L"^^|^^";
        ws_sprPattern += L"|#|#|";
        ws_sprPattern += L"|#|#|";

        spriteWidth = 5;
        spriteHight = 5;
        spr_ENEMY_0 = new olcSprite(spriteWidth, spriteHight);
        spriteCreate(spriteWidth, spriteHight, ws_sprPattern, spr_ENEMY_0);


        //___________________________________________________________________
    };

    void initObjs() {
        // Heroes
        v_heroesList.push_back(newObj  (0, spr_HERO_0,  newStats(   5,   4,   2)));

        // Enemies
        v_enemiesList.push_back(newObj (0, spr_ENEMY_0, newStats(   5,   5,   0)));

        // Blast
        v_blastsList.push_back(newObj  (0, spr_BLAST_0, newStats(  15,   0,   0)));
    };

    olcSprite* getSprite(int id, string type) {
        if        (type == "hero"){
            return v_heroesList[id]->spr_sprite;
        } else if (type == "enemy"){
            return v_enemiesList[id]->spr_sprite;
        } else if (type == "blast"){
            return v_blastsList[id]->spr_sprite;
        }
            return NULL;
    }

    Stats* getStats(int id, string type) {
        if        (type == "hero"){
            return v_heroesList[id]->st_stats;
        } else if (type == "enemy"){
            return v_enemiesList[id]->st_stats;
        } else if (type == "blast"){
            return v_blastsList[id]->st_stats;
        } else
            return NULL;

    }

public:
    int i_id;
    int i_hight;
    int i_width;
    float f_objX;
    float f_objY;
    string s_type;
    olcSprite* spr_sprite;
    bool b_needDelete;
    Stats* st_stats;
    Status stu_status;

    void init() {
        initSprites();
        initObjs();
    }

    GameObject() {};

    GameObject(int id, int h, int w, float x, float y, string type ) {
        i_id = id;
        f_objX = x;
        f_objY = y;
        i_hight = h;
        i_width = w;
        s_type = type;
        spr_sprite = getSprite(i_id, s_type);
        b_needDelete = false;
        st_stats = getStats(i_id, s_type);

    }

    GameObject(int h, int w, olcSprite* sprite) {
        i_id = 0;
        i_hight = h;
        i_width = w;
        f_objX = 50.0f;
        f_objY = 50.0f;
        s_type = "UNKNOWN";
        b_needDelete = false;
        spr_sprite = spr_UNW_0;
    }

    void setStats(float speed, float hp, float mp, float str, float agl, float inte) {
        this->st_stats->f_speed = speed;
        this->st_stats->f_hp = hp;
        this->st_stats->f_mp = mp;
        this->st_stats->f_str = str;
        this->st_stats->f_agl = agl;
        this->st_stats->f_int = inte;
    }

    void setSpeed (float speed) {
        this->st_stats->f_speed = speed;
    }

    void setStr (float str) {
        this->st_stats->f_str = str;
    }

    void setAgl (float agl) {
        this->st_stats->f_agl = agl;
    }

    void setInt (float inti) {
        this->st_stats->f_int = inti;
    }

    void updateStats() {
        while(this->st_stats->i_lvl < this->st_stats->i_exp / EXP_NEED_FOR_LEVEL) {
            this->st_stats->i_lvl++;
            this->st_stats->f_str++;
            this->st_stats->f_agl++;
            this->st_stats->f_int++;
        }
        this->st_stats->f_hp = this->st_stats->f_str * STRANGE_HP_SCALE;
        this->st_stats->f_mp = this->st_stats->f_int * ITELECT_MP_SCALE;
        this->st_stats->f_speed = this->st_stats->f_agl * AGILITY_SPEED_SCALE;
    }

};