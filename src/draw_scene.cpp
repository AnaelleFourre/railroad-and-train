#include "draw_scene.hpp"
#include "draw_tracks.hpp"
#include "draw_station.hpp"
#include "draw_train.hpp"
#include "grid.hpp"

int grid_size {10};

GLBI_Engine myEngine;
GLBI_Set_Of_Points frame(3);
GLBI_Convex_2D_Shape square{3};
GLBI_Convex_2D_Shape disc{3};
GLBI_Convex_2D_Shape roof_left_top{3};
GLBI_Convex_2D_Shape roof_right_top{3};
GLBI_Convex_2D_Shape roofside{3};
IndexedMesh* cube;
IndexedMesh* cylinder;
IndexedMesh* light;

void initScene(const Grid& grid) {
	// Square base
	std::vector<float> squareBase{0.0,0.0,0.0,	
		10.0,0.0,0.0,
		10.0,10.0,0.0,
			0.0,10.0,0.0};
	square.initShape(squareBase);
	square.changeNature(GL_TRIANGLE_FAN);

	// Frame
	std::vector<float> framePoints {10.0, 0.0, 0.0,
			0.0, 0.0, 0.0,
			0.0, 10.0, 0.0,
			0.0, 0.0, 0.0,
			0.0, 0.0, 10.0,
			0.0, 0.0, 0.0
	};
	std::vector<float> frameColors {1.0, 0.0, 0.0,
			1.0, 0.0, 0.0,
			0.0, 1.0, 0.0,
			0.0, 1.0, 0.0,
			0.0, 0.0, 1.0,
			0.0, 0.0, 1.0
	};
	frame.initSet(framePoints, frameColors);
	frame.changeNature(GL_LINES);

	// Disc
	std::vector<float> discPoints;
	float angle;
	for (auto i = 0; i < 360; i++) {
		angle = M_PI * i / 180;
		discPoints.push_back(cos(angle));
		discPoints.push_back(sin(angle));
		discPoints.push_back(0.);
	}
	disc.initShape(discPoints);
	disc.changeNature(GL_TRIANGLE_FAN);

	// Roof left top
	std::vector<float> roof_top_l_points = {
		0., 0., 0.,
		1., 0., 0., 
		1., 0.5, 1., 
		0., 0.5, 1.
	};
	roof_left_top.initShape(roof_top_l_points);
	roof_left_top.changeNature(GL_TRIANGLE_FAN);

	// Roof right top
	std::vector<float> roof_top_r_points = {
		0., 1., 0.,
		1., 1., 0., 
		1., 0.5, 1., 
		0., 0.5, 1.
	};
	roof_right_top.initShape(roof_top_r_points);
	roof_right_top.changeNature(GL_TRIANGLE_FAN);

	// Roof side
	std::vector<float> roofside_points = {
		0., 1., 0., 
		0., 0.5, 1., 
		0., 0., 0., 
	};
	roofside.initShape(roofside_points);
	roofside.changeNature(GL_TRIANGLE_FAN);

	
	// Cube
	cube = basicCube();
	cube->createVAO();

	
	// Cylinder
	cylinder = basicCylinder(1., 1.);
	cylinder->createVAO();

	// Light
	light = basicSphere(1.);
	light->createVAO();


	initTracks();
	initStation();
	initTrain(grid.path[0][0], grid.path[0][1]);
}

void drawRoof() {
	myEngine.mvMatrixStack.pushMatrix();
		myEngine.updateMvMatrix();
		myEngine.setNormalForConvex2DShape(Vector3D(0., -1., 0.5));
		roof_left_top.drawShape();
	myEngine.mvMatrixStack.popMatrix();
	myEngine.mvMatrixStack.pushMatrix();
		myEngine.updateMvMatrix();
		myEngine.setNormalForConvex2DShape(Vector3D(0., 1., 0.5));
		roof_right_top.drawShape();
	myEngine.mvMatrixStack.popMatrix();
	myEngine.mvMatrixStack.pushMatrix();
		myEngine.updateMvMatrix();
		myEngine.setNormalForConvex2DShape(Vector3D(-1., 0., 0.));
		roofside.drawShape();
	myEngine.mvMatrixStack.popMatrix();
	myEngine.mvMatrixStack.pushMatrix();
		myEngine.mvMatrixStack.addTranslation(Vector3D(1., 0., 0.));
		myEngine.updateMvMatrix();
		myEngine.setNormalForConvex2DShape(Vector3D(1., 0., 0.));
		roofside.drawShape();
	myEngine.mvMatrixStack.popMatrix();
}

void drawGrid() {

	myEngine.setAttenuationFactor(Vector3D(10., 0., 10.));
	myEngine.setNormalForConvex2DShape(Vector3D(0., 0., 1.));
	for (auto i = 0; i < grid_size; i++) {
		for (auto j = 0; j < grid_size; j++) {
			myEngine.setFlatColor(10., 10., 10.);
			myEngine.mvMatrixStack.pushMatrix();
				myEngine.mvMatrixStack.addTranslation(Vector3D(10. * (i - grid_size / 2), 10. * (j - grid_size / 2), 0.));
				myEngine.updateMvMatrix();
				square.drawShape();
			myEngine.mvMatrixStack.popMatrix();
		}
	}
	myEngine.setAttenuationFactor(Vector3D(1., 0., 1.));
	
}

void drawFrame() {
	frame.drawSet();
}


void drawScene(const Grid& grid) {
	grid_size = grid.size_grid;
	glPointSize(10.0);
	
	drawFrame();
	
	myEngine.switchToPhongShading();
	drawGrid();
	drawTrain(grid.path[0][0], grid.path[0][1]);
	drawStation(grid.origin[0], grid.origin[1]);
	drawTracksFromPath(grid);
	myEngine.switchToFlatShading();


}



