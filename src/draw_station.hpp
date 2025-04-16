#pragma once

#include "glbasimac/glbi_engine.hpp"
#include "glbasimac/glbi_set_of_points.hpp"
#include "glbasimac/glbi_convex_2D_shape.hpp"
#include "tools/basic_mesh.hpp"

#include "grid.hpp"

using namespace glbasimac;


/* OpenGL Engine */
extern GLBI_Engine myEngine;
extern IndexedMesh* cube;

void initStation();

void drawStation(int pos_x, int pos_y);