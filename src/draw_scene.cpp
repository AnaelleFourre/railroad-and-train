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
GLBI_Convex_2D_Shape roof{3};
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

	// Roof
	std::vector<float> cr_points = {
		1., 0., 0., // 1, 7
		1., 0.5, 1., // 2, 6
		1., 1., 0., // 3
		0., 1., 0., // 4, 10
		0., 0.5, 1., // 5, 9
		1., 0.5, 1., // 2, 6
		1., 0., 0., // 1, 7
		0., 0., 0., // 8
		0., 0.5, 1., // 5, 9
		0., 1., 0., // 4, 10
	};
	roof.initShape(cr_points);
	roof.changeNature(GL_TRIANGLE_STRIP);

	
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

	
	myEngine.switchToPhongShading();
	myEngine.setLightPosition(Vector4D(10. * grid.path[0][0] + 5., 10. * grid.path[0][1] + 11., 5.5 + 2 * rr, 1.));
	myEngine.setLightIntensity(Vector3D(100., 100., 50.));
    myEngine.switchToFlatShading();

}

void drawGrid() {

	for (auto i = 0; i < grid_size; i++) {
		for (auto j = 0; j < grid_size; j++) {
			myEngine.setFlatColor(0.2 * ((i + j) % 2 + 1), 0., 0.);
			myEngine.mvMatrixStack.pushMatrix();
				myEngine.mvMatrixStack.addTranslation(Vector3D(10. * (i - grid_size / 2), 10. * (j - grid_size / 2), 0.));
				myEngine.updateMvMatrix();
				square.drawShape();
			myEngine.mvMatrixStack.popMatrix();
		}
	}
}

void drawFrame() {
	frame.drawSet();
}


void drawScene(const Grid& grid) {
	grid_size = grid.size_grid;
	glPointSize(10.0);

	
	drawFrame();
	drawGrid();
	
	myEngine.switchToPhongShading();
	drawTracksFromPath(grid);
	myEngine.switchToFlatShading();
	drawStation(grid.origin[0], grid.origin[1]);
	drawTrain(grid.path[0][0], grid.path[0][1]);


}



