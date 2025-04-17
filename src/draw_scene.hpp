#pragma once

#include "glbasimac/glbi_engine.hpp"
#include "glbasimac/glbi_set_of_points.hpp"
#include "glbasimac/glbi_convex_2D_shape.hpp"
#include "tools/basic_mesh.hpp"

#include "grid.hpp"

using namespace glbasimac;

/* Camera parameters and functions */
static const float Z_NEAR {0.1f};
static const float Z_FAR {500.f};
extern float angle_theta;      // Angle between x axis and viewpoint
extern float angle_phy  ;      // Angle between z axis and viewpoint
extern float dist_zoom  ;      // Distance between origin and viewpoint

static const float POS_X_RAIL1 {3.};
static const float POS_X_RAIL2 {7.};
static const float POS_X_BALAST1 {2.};
static const float POS_X_BALAST2 {8.};
static const float sr {0.2};
static const float rr {0.4};
static const float sx {(10. - 10 * rr) / 10.};

extern int angle_anim_rot_scale;

extern int angle_anim_rot_arm;

extern int angle;

/* OpenGL Engine */
extern GLBI_Engine myEngine;
extern IndexedMesh* cube;
extern IndexedMesh* cylinder;
extern GLBI_Convex_2D_Shape disc;
extern GLBI_Convex_2D_Shape roof;

void initScene();
void drawScene(const Grid& grid);


