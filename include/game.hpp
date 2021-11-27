#ifndef __B2D_H
#define __B2D_H

#ifdef __cplusplus

#include <limits>
#include <algorithm>
#include <stdlib.h>

#include "box2d/box2d.h"
#include "rdl.h"

#include "hand.hpp"
#include "rope.hpp"
#include "blade.hpp"
#include "enemy.hpp"
#include "box.hpp"

extern "C" {
    #include <libdragon.h>
    #include "geometry.h"
}

class Enemy;

namespace constants {
    constexpr float to16_16 = 65536.f;
    constexpr float gameAreaWidth = 8.f;
    constexpr float gameAreaHeight = 6.f;
    constexpr int startIncreasingSpeedAtLevel = 20;
    constexpr float swawnSafeRadius = 0.45f; // Must be larget than enemy size
    constexpr float scale = 80.0f;
    constexpr float allowedDistance = 5.0f;
    constexpr int gracePeriodMs = 300;
}

enum CollisionCategory: uint16
{
    enemy = 0x0001,
    environment = 0x0002,
    hand = 0x0004,
    blade = 0x0008,
};

class Game : public b2ContactListener, Box
{
    private:
        void BeginContact(b2Contact* contact);

        RdpDisplayList* rdl;

        // Simulation params
        int32 velocityIterations = 6;
        int32 positionIterations = 2;
        float timeStep = 1.0f / 60.0f;

        b2Vec3 cameraPos = {0., 0., 1.};

        // TODO: move to stack - how to keep the original pointer
        // created when the array is static?
        static const int box_count = 20;
        Enemy* enemies[box_count];

        // Controller data
        int controllers = 0;
        struct controller_data keys;
        struct controller_data keysDown;
        bool holdingLeft = false;
        bool holdingRight = false;

        // Gameplay
        int highScore = 0;
        int score = 0;
        int lives = 3;
        bool isDead = true;
        bool isReset = true;
        int level = 0;

        // Animation and grace period
        int64_t startedShowingDamageL = std::numeric_limits<int64_t>::min();
        int64_t startedShowingDamageR = std::numeric_limits<int64_t>::min();

#ifndef NDEBUG
        int64_t lastUpdate = std::numeric_limits<int64_t>::min();
#endif

    public:
        Game(RdpDisplayList* rdlParam);
        void updateBG();
        void update();
        void updateUI(display_context_t disp);
        void reset();
        void addScore(int points);
        void gameOver();
};

#else

typedef struct Game Game;

#endif

#ifdef __cplusplus
    #define EXPORT_C extern "C"
#else
    #define EXPORT_C
#endif

EXPORT_C Game* new_Game(RdpDisplayList*);
// TODO: implement cleanup routines
EXPORT_C void delete_Game(Game*);

EXPORT_C void update_BG(Game*);
EXPORT_C void update_Game(Game*);
EXPORT_C void update_UI(Game*, display_context_t disp);

#endif /* __B2D_H */

