
#include "box2d/box2d.h"
#include "box2d/b2_rope.h"

#include "rope.hpp"
#include "game.hpp"

extern "C" {
    #include <libdragon.h>
    #include "geometry.h"
}

// TODO: move things into a PlayerContraption classs
Rope::Rope(int count, b2Vec2 pos1, b2Vec2 pos2) {
    ropeDef.count = count;

    b2Vec2 vertices[ropeDef.count];
    float masses[ropeDef.count];

    b2Vec2 diff = (pos2 - pos1);
    diff *= 1. / (float)ropeDef.count;

    for (int i = 0; i < ropeDef.count; i++)
    {
        b2Vec2 add = diff;
        add *= (float)i;
        if (i == 0 || i == (ropeDef.count - 1)) {
            masses[i] = 0.0f;
        } else if (i == (ropeDef.count/2)) {
            masses[i] = 5.0f;
        } else {
            masses[i] = 0.5f;
        }
        vertices[i] = add;
    }

    ropeDef.gravity = b2Vec2(0.0f, 10.0f);
    ropeDef.vertices = vertices;
    ropeDef.masses = masses;
    ropeDef.position = pos1;

    ropeTuning.bendStiffness= 0.0001f;

    ropeDef.tuning = ropeTuning;

    rope.Create(ropeDef);
}
b2Vec2 Rope::update(b2Vec2 pos1, b2Vec2 pos2) {
    rope.Step(timeStep, velocityIterations, pos1, pos2);
    return rope.m_ps[ropeDef.count / 2];
}

void Rope::draw(RdpDisplayList* rdl) {
    for (int i = 0; i < ropeDef.count - 1; i++)
    {
        b2Vec2 vertex1 = rope.m_ps[i];
        b2Vec2 vertex3 = rope.m_ps[i+1];

        b2Vec2 offset = (vertex3 - vertex1).Skew();
        offset.Normalize();
        offset *= 0.02;

        b2Vec2 vertex2 = rope.m_ps[i] + offset;

        render_tri_strip(rdl,
            vertex1.x * 80. * constants::to16_16, (vertex1.y) * 40. * constants::to16_16,
            vertex2.x * 80. * constants::to16_16, (vertex2.y) * 40. * constants::to16_16,
            vertex3.x * 80. * constants::to16_16, (vertex3.y) * 40. * constants::to16_16
        );

        vertex1 = rope.m_ps[i+1] + offset;
        render_tri_strip_next(rdl, vertex1.x * 80. * constants::to16_16,   (vertex1.y) * 40. * constants::to16_16);
    }
}

void Rope::reset() {
    rope.Reset(ropeDef.position);
}