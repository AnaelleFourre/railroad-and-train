#include "draw_scene.hpp"

/// Camera parameters
float angle_theta {45.0};      // Angle between x axis and viewpoint
float angle_phy {30.0};      // Angle between z axis and viewpoint
float dist_zoom {30.0};      // Distance between origin and viewpoint

int grid_size {10};

GLBI_Engine myEngine;
GLBI_Set_Of_Points frame(3);
GLBI_Convex_2D_Shape grid{3};
GLBI_Convex_2D_Shape disc{3};
GLBI_Convex_2D_Shape curved_rail{3};
IndexedMesh* cube;
IndexedMesh* cylinder;


void initScene() {
	// Square base
	std::vector<float> squareBase{0.0,0.0,0.0,	
		10.0,0.0,0.0,
		10.0,10.0,0.0,
			0.0,10.0,0.0};
	grid.initShape(squareBase);
	grid.changeNature(GL_TRIANGLE_FAN);

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


	// Cube
	cube = basicCube();
	cube->createVAO();

	// Cylinder
	cylinder = basicCylinder(1., 1.);
	cylinder->createVAO();

}

void drawGrid() {
	for (auto i = 0; i < grid_size; i++) {
		for (auto j = 0; j < grid_size; j++) {
			myEngine.setFlatColor(0.2 * ((i + j) % 2 + 1), 0., 0.);
			myEngine.mvMatrixStack.pushMatrix();
				myEngine.mvMatrixStack.addTranslation(Vector3D(10. * (i - grid_size / 2), 10. * (j - grid_size / 2), 0.));
				myEngine.updateMvMatrix();
				grid.drawShape();
			myEngine.mvMatrixStack.popMatrix();
		}
	}
}

void drawRail() {
	myEngine.setFlatColor(140. / 255, 140. / 255., 140. / 255.);
	myEngine.mvMatrixStack.pushMatrix();
		myEngine.mvMatrixStack.addHomothety(Vector3D(sr, 10., sr));
		myEngine.mvMatrixStack.addTranslation(Vector3D(0.5, 0.5, 0.5));
		myEngine.updateMvMatrix();
		cube->draw();
	myEngine.mvMatrixStack.popMatrix();
}

void drawBalast() {
	myEngine.setFlatColor(153. / 255, 102. / 255., 51. / 255.);
	myEngine.mvMatrixStack.pushMatrix();
		myEngine.mvMatrixStack.addHomothety(Vector3D(6., rr, rr));
		myEngine.mvMatrixStack.addTranslation(Vector3D(0., 1., 1.));
		myEngine.mvMatrixStack.pushMatrix();
			myEngine.mvMatrixStack.addRotation(M_PI / 2, Vector3D(0., 1., 0.));
			myEngine.updateMvMatrix();
			disc.drawShape(); 
			myEngine.mvMatrixStack.pushMatrix();
				myEngine.mvMatrixStack.addTranslation(Vector3D(0., 0., 1.));
				myEngine.updateMvMatrix();
				disc.drawShape();
			myEngine.mvMatrixStack.popMatrix();
		myEngine.mvMatrixStack.popMatrix(); 
		myEngine.mvMatrixStack.pushMatrix();
			myEngine.mvMatrixStack.addRotation(3 * M_PI / 2, Vector3D(0., 0., 1.));
			myEngine.updateMvMatrix();
			cylinder->draw();
		myEngine.mvMatrixStack.popMatrix();
	myEngine.mvMatrixStack.popMatrix();
}

void drawStraightTrack() {
	myEngine.mvMatrixStack.pushMatrix();
		myEngine.mvMatrixStack.addTranslation(Vector3D(POS_X_BALAST1, 0., 0.));
		for (auto i = 0; i < 5; i++) {
			myEngine.mvMatrixStack.pushMatrix();
				myEngine.mvMatrixStack.addTranslation(Vector3D(0., sx + i * 2 * sx + i * 2 * rr, 0.));
				drawBalast();
			myEngine.mvMatrixStack.popMatrix();
		}
	myEngine.mvMatrixStack.popMatrix();
	myEngine.mvMatrixStack.pushMatrix();
		myEngine.mvMatrixStack.addTranslation(Vector3D(POS_X_RAIL1, 0., 2 * rr));
		drawRail();
	myEngine.mvMatrixStack.popMatrix();
	myEngine.mvMatrixStack.pushMatrix();
		myEngine.mvMatrixStack.addTranslation(Vector3D(POS_X_RAIL2, 0., 2 * rr));
		drawRail();
	myEngine.mvMatrixStack.popMatrix();
}

void drawFrame() {
	frame.drawSet();
}


void drawScene() {
	glPointSize(10.0);
	
	drawFrame();
	drawGrid();

	drawStraightTrack();
}



