#include "LoopingEntity.hpp"
#include <chrono>
#include <thread>

class EntityLoopException : public std::exception
{
public:
    EntityLoopException(std::string exception)
    {
        err = exception;
    }
    
private:
    std::string err;
    const char * what () const throw ()
    {
        return err.c_str();
    }
    
};

bool kpg::LoopingEntity::Exists()
{
    return !end;
}

bool kpg::LoopingEntity::Spawn()
{
    std::thread calcthread(&LoopingEntity::CalcThread, this);
    calcthread.detach();
    return true;
}

bool kpg::LoopingEntity::OnCreate()
{
    return true;
}
bool kpg::LoopingEntity::OnUpdate()
{
    return true;
}
bool kpg::LoopingEntity::OnDestroy()
{
    return true;
}

kpg::EntityType kpg::LoopingEntity::GetEntityType()
{
    return etype;
}

float kpg::LoopingEntity::GetHeight()
{
    return Height;
}

float kpg::LoopingEntity::GetWidth()
{
    return Width;
}

float kpg::LoopingEntity::GetXPos()
{
    return XPos;
}

float kpg::LoopingEntity::GetYPos()
{
    return YPos;
}

float kpg::LoopingEntity::GetXVel()
{
    return XVel;
}

float kpg::LoopingEntity::GetYVel()
{
    return YVel;
}

kpg::Coordinates kpg::LoopingEntity::GetNE()
{
    return { XPos + Width, YPos };
}

kpg::Coordinates kpg::LoopingEntity::GetSE()
{
    return { XPos + Width, YPos + Height };
}

kpg::Coordinates kpg::LoopingEntity::GetSW()
{
    return { XPos, YPos + Height };
}

kpg::Coordinates kpg::LoopingEntity::GetNW()
{
    return { XPos, YPos };
}

void kpg::LoopingEntity::CalcThread()
{
    if(!OnCreate()) throw EntityLoopException("OnCreate method error");
    
    auto period = std::chrono::milliseconds(Period);
    auto next = std::chrono::steady_clock::now();
    auto prev = next - period;
    while (!end)
    {
        auto now = std::chrono::steady_clock::now();
        prev = now;
        
        if(!OnUpdate()) throw EntityLoopException("OnUpdate method error");
        
        next += period;
        std::this_thread::sleep_until(next);
    }
    
    if(!OnDestroy()) throw EntityLoopException("OnDestroy method error");
}

void kpg::LoopingEntity::Collide(Collision newcollision)
{
    CollisionMutex.lock();
    Collisions.push(newcollision);
    CollisionMutex.unlock();
}

void kpg::LoopingEntity::Destroy()
{
    end = true;
}
