#include <iostream>
#include <stdio.h>
#include <olcC++/olcConsoleGameEngineSDL.h>
#include <string>
#include <vector>

#define SCREENHIGHT  50
#define SCREENWIDTH  80

using namespace std;

class GameObject {
private:


public:
    int i_id;
    int i_hight;
    int i_width;
    float f_objX, f_objY;
    string s_type;
    vector<olcSprite *> v_Sprites;
    int i_spriteIter;
    bool b_needDelete;

    GameObject(int id, int h, int w, float x, float y, string type, vector<olcSprite *> sprites ){
        i_id = id;
        f_objX = x;
        f_objY = y;
        i_hight = h;
        i_width = w;
        s_type = type;
        v_Sprites = sprites;
        i_spriteIter = 0;
        b_needDelete = false;
    }

    GameObject(int id, int h, int w){
        i_id = id;
        i_hight = h;
        i_width = w;
        f_objX = rand() % SCREENWIDTH;
        f_objY = rand() % SCREENHIGHT;
        s_type = "UNKOWN";
        i_spriteIter = 0;
        b_needDelete = false;


        olcSprite *s_ts = new olcSprite(3, 3);

        s_ts->SetColour(0, 0, rand() % 16);
        s_ts->SetColour(1, 0, rand() % 16);
        s_ts->SetColour(2, 0, rand() % 16);
        s_ts->SetColour(0, 1, rand() % 16);
        s_ts->SetColour(1, 1, rand() % 16);
        s_ts->SetColour(2, 1, rand() % 16);
        s_ts->SetColour(0, 2, rand() % 16);
        s_ts->SetColour(1, 2, rand() % 16);
        s_ts->SetColour(2, 2, rand() % 16);
        s_ts->SetGlyph(0, 0, L'?');
        s_ts->SetGlyph(1, 0, L'#');
        s_ts->SetGlyph(2, 0, L'?');
        s_ts->SetGlyph(0, 1, L'?');
        s_ts->SetGlyph(1, 1, L'#');
        s_ts->SetGlyph(2, 1, L'?');
        s_ts->SetGlyph(0, 2, L'?');
        s_ts->SetGlyph(1, 2, L'#');
        s_ts->SetGlyph(2, 2, L'?');
        v_Sprites.push_back(s_ts);
    }

protected:


    void AddSprite(olcSprite *sprite){
        this->v_Sprites.push_back(sprite);
    }

    void ChangeType(string newType){
        this->s_type = newType;
    }

};

class GameEng : public olcConsoleGameEngine{
public:
        GameEng(){

        }

protected:
        virtual bool OnUserCreate(){

            obj_objectList.reserve(20);
            obj_objectList.push_back(new GameObject(0, 3, 3));
            obj_objectList.push_back(new GameObject(0, 3, 3));
            obj_objectList.push_back(new GameObject(0, 3, 3));

            return true;
        }

        virtual bool OnUserUpdate(float fElapsedTime){
            if (obj_objectList.size() < 20) {
                obj_objectList.push_back(new GameObject(0, 3, 3));
            } else {
                obj_objectList.erase(obj_objectList.begin());
                obj_objectList.push_back(new GameObject(0, 3, 3));
            }

            Fill(0 ,0, SCREENWIDTH, ScreenHeight(), L' ');
            for(GameObject* obj : obj_objectList)
                DrawSprite((int)obj->f_objX, (int)obj->f_objY, obj->v_Sprites[obj->i_spriteIter]);
            return true;
        }

private:
    vector<GameObject*> obj_objectList;
};




int main() {
    GameEng game;

    game.ConstructConsole(SCREENWIDTH, SCREENHIGHT, 8, 8);



    game.Start();


    std::cout << "Hello, World!" << std::endl;
    return 0;
}