#include <iostream>
#include <stdio.h>
#include <olcC++/olcConsoleGameEngineSDL.h>
#include "GameObj.h"
#include <string>
#include <vector>

#define SCREENHIGHT  100
#define SCREENWIDTH  160


class GameEng : public olcConsoleGameEngine {
public:
    GameEng() {}

    vector<GameObject*> obj_objectList;


    void createNewObj(int id, int h, int w, float x, float y, string type) {
        obj_objectList.push_back(new GameObject(id, h, w, x, y, type));
        obj_objectList[obj_objectList.size() - 1]->updateStats();
        obj_objectList[obj_objectList.size() - 1]->stu_status.f_curHP = obj_objectList[obj_objectList.size() - 1]->st_stats->f_hp;
        obj_objectList[obj_objectList.size() - 1]->stu_status.f_curMP = obj_objectList[obj_objectList.size() - 1]->st_stats->f_mp;
    }

protected:

    virtual bool OnUserCreate() {
        createNewObj(0, 5, 5, 10, 10, "hero");
        createNewObj(0, 5, 5, rand() % m_nScreenWidth, rand() % m_nScreenHeight, "enemy");


        return true;
    }

    virtual bool OnUserUpdate(float fElapsedTime) {

        Fill(0, 0, m_nScreenWidth, m_nScreenHeight, L' ');
        for (GameObject* obj : obj_objectList) {

            if (obj->s_type == "hero") {
                if (GetKey(VK_UP).bHeld) {
                    obj->f_objY -= obj->st_stats->f_speed * fElapsedTime;
                }

                if (GetKey(VK_DOWN).bHeld) {
                    obj->f_objY += obj->st_stats->f_speed * fElapsedTime;
                }

                if (GetKey(VK_LEFT).bHeld) {
                    obj->f_objX -= obj->st_stats->f_speed * fElapsedTime;
                }

                if (GetKey(VK_RIGHT).bHeld) {
                    obj->f_objX += obj->st_stats->f_speed * fElapsedTime;
                }

                if (GetKey(VK_SPACE).bReleased) {

                    createNewObj(2, 1, 1, obj->f_objX + obj->i_width, obj->f_objY + (obj->i_hight / 2) + 0.5f, "blast");

                }
            }
            else if (obj->s_type == "blast") {
                obj->f_objX += obj->st_stats->f_speed * fElapsedTime;
                if (obj->f_objX > m_nScreenWidth) {
                    obj->b_needDelete = true;
                } else {
                    for (vector<GameObject*>::size_type i = 0; i < obj_objectList.size(); i++) {
                        if (obj_objectList[i]->s_type == "enemy") {
                            if ((obj->f_objX > obj_objectList[i]->f_objX &&
                                obj->f_objX < obj_objectList[i]->f_objX + obj_objectList[i]->i_width) &&
                               (obj->f_objY > obj_objectList[i]->f_objY &&
                                obj->f_objY < obj_objectList[i]->f_objY + obj_objectList[i]->i_hight)){
                                    obj->b_needDelete = true;
                                    obj_objectList[i]->stu_status.f_curHP -= HERO->st_stats->f_str * STRANGE_DMG_SCALE;
                                    if (obj_objectList[i]->stu_status.f_curHP < 0)
                                        obj_objectList[i]->b_needDelete = true;
                            }
                        }
                    }
                }
            }
            else if (obj->s_type == "enemy") {
                if (rand() % 2) {
                    if (rand() % 2){
                        obj->f_objX += obj->st_stats->f_speed * fElapsedTime;
                    } else {
                        obj->f_objX -= obj->st_stats->f_speed * fElapsedTime;
                    }

                } else {
                    if (rand() % 2) {
                        obj->f_objY += obj->st_stats->f_speed * fElapsedTime;
                    } else {
                        obj->f_objY -= obj->st_stats->f_speed * fElapsedTime;
                    }
                }
            }

            DrawSprite ((int)obj->f_objX, (int)obj->f_objY, obj->spr_sprite);
        }


        for (vector<GameObject*>::size_type i = 0; i < obj_objectList.size(); i++) {
            if (obj_objectList[i]->b_needDelete) {
                if (obj_objectList[i]->s_type == "enemy"){
                    HERO->st_stats->i_exp += EXP_BOUNTY;
                    HERO->updateStats();
                }
                obj_objectList.erase(obj_objectList.begin() + i);
            }
        }

        return true;
    }

private:




};


int main(){
    GameObject initObjs;
    initObjs.init();

    GameEng game;
    game.ConstructConsole(SCREENWIDTH, SCREENHIGHT, 8, 8);
    game.Start();

    return 0;
}