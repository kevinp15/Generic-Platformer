#define OLC_PGE_APPLICATION
#include "LoopingEntity.hpp"
#include "olcPixelGameEngine.h"
#include "BouncyBall.hpp"
#include "EntityVars.hpp"
#include "Player.hpp"

#include <cstdlib>

static uint32_t NextID = 0;
uint32_t getNextID()
{
    while(kpg::KnownIDs.count(NextID) > 0)
    {
        NextID++;
    }
    kpg::KnownIDs.insert(NextID);
    return NextID;
}

class PortalDemo : public olc::PixelGameEngine
{
private:
#define ballcount 50
    std::unordered_map<std::string, olc::Renderable*> imgs;
public:
    PortalDemo()
    {
        sAppName = "Gravity";
    }

public:
    bool OnUserCreate() override
    {
        std::string playerimage = "player.png";
        olc::Renderable * player = new olc::Renderable();
        player->Load(playerimage);
        std::string ballimage = "ball.png";
        olc::Renderable * ball = new olc::Renderable();
        ball->Load(ballimage);
        
        imgs[playerimage] = player;
        imgs[ballimage] = ball;

        kpg::LoopingEntity * p = new kpg::Player();
        p->Spawn();
        std::pair<std::string, kpg::LoopingEntity*> newplayer(playerimage, p);
        kpg::Entities[getNextID()] = newplayer;
        for(int i = 0; i < ballcount; i++)
        {
            kpg::LoopingEntity * b = new kpg::BouncyBall(rand() % (kpg::nWindowWidth - 32), rand() % (kpg::nWindowWidth - 32), rand() % 1000, rand() % 1000);
            b->Spawn();
            std::pair<std::string, kpg::LoopingEntity*> newball(ballimage, b);
            kpg::Entities[getNextID()] = newball;
        }

        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override
    {
        Clear(olc::BLACK);

        if(GetKey(olc::E).bPressed)
            return false;

        if(GetKey(olc::SPACE).bPressed)
            kpg::start = true;
        
        if(GetKey(olc::LEFT).bHeld)
            kpg::bMovingLeft = true;
        else
            kpg::bMovingLeft = false;
        if(GetKey(olc::RIGHT).bHeld)
            kpg::bMovingRight = true;
        else
            kpg::bMovingRight = false;
        if(GetKey(olc::UP).bPressed)
            kpg::bJumped = true;
        
        for(const auto & [key, val] : kpg::Entities)
        {
            if(val.second->Exists())
            {
                DrawDecal({ val.second->GetXPos(), val.second->GetYPos() }, imgs[val.first]->Decal());
                FillRectDecal({ val.second->GetXPos(), val.second->GetYPos() }, { 32.0f, 8.0f }, olc::BLACK);
                DrawStringDecal({ val.second->GetXPos(), val.second->GetYPos() }, std::to_string(key));
            }
        }

        return true;
    }

    bool OnUserDestroy() override 
    {
        return true;
    }
};

int main(int argc, char const *argv[])
{
    srand(time(nullptr));
	PortalDemo demo;
	if (demo.Construct(kpg::nWindowWidth, kpg::nWindowHeight, 1, 1, false, false))
		demo.Start();

	return 0;
}
