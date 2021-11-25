#ifndef __B2D_H
#define __B2D_H

#ifdef __cplusplus

#include "box2d/box2d.h"
#include "rdl.h"

#include "rope.hpp"
#include "enemy.hpp"

namespace constants {
    constexpr float to16_16 = 65536.f;
    constexpr float gameAreaWidth = 8.f;
    constexpr float gameAreaHeight = 6.f;
}

enum CollisionCategory: uint16
{
    enemy = 0x0001,
    environment = 0x0002,
    hand = 0x0004,
    blade = 0x0008,
};

class Game : public b2ContactListener
{
    private:
        b2BodyDef groundBodyDef;
        b2Body* groundBody;
        b2PolygonShape groundBox;
        b2BodyDef bodyDef;
        b2Body* body;
        b2PolygonShape dynamicBox;
        b2FixtureDef groundFixtureDef;
        int32 velocityIterations = 6;
        int32 positionIterations = 2;
        float timeStep = 1.0f / 60.0f;

        RdpDisplayList* rdl;

        float scale = 1.0f;
        b2Vec3 cameraPos = {0., 0., 1.};

        Rope* rope;

        static const int box_count = 10;
        Enemy* enemies[box_count];

        // Gameplay
        int highScore = 0;
        int score = 0;
        int lives = 3;
        bool isDead = true;
        bool shouldReset = false;

    public:
        Game(RdpDisplayList* rdlParam);
        int update();
        void updateUI(display_context_t disp);
        void reset();
        void addScore(int points);

        void BeginContact(b2Contact* contact);
};

#else

typedef struct Game Game;

#endif

#ifdef __cplusplus
    #define EXPORT_C extern "C"
#else
    #define EXPORT_C
#endif


#include <libdragon.h>
EXPORT_C Game* new_Game(RdpDisplayList*);
EXPORT_C void delete_Game(Game*);
EXPORT_C int update_Game(Game*);
EXPORT_C void update_UI(Game*, display_context_t disp);

#endif /* __B2D_H */
