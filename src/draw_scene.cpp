#include "draw_scene.hpp"
#include "draw_tracks.hpp"
#include "draw_station.hpp"
#include "draw_train.hpp"
#include "grid.hpp"

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

bool realist_light {false};

float train_pos_x {0.};
float train_pos_y {0.};
float train_angle {0.};
float move_angle {0.};
float next_angle {0.};
int current_path_index {0};
float train_speed {0.1};
float angle_speed {0.};

Grid grid;


float dot(std::vector<float> a, std::vector<float> b) {
	return a[0] * b[0] + a[1] * b[1];
}

float norm(std::vector<int> a) {
	return sqrt(a[0] * a[0] + a[1] * a[1]);
}

std::vector<float> normalize(std::vector<int> a) {
	auto n = norm(a);
	return std::vector<float>{a[0] / n, a[1] / n};
}

float getAngle(std::vector<int> a, std::vector<int> b) {
	return acos(dot(normalize(std::vector<int>{b[0] - a[0], b[1] - a[1]}), std::vector<float>{0., 1.}));
}


void initScene(const Grid& g) {
	grid = g;
	std::cout << "Init scene" << std::endl;
	train_pos_x = 10. * grid.path[0][0] + 5.;
	train_pos_y = 10. * grid.path[0][1] + 5.;
	auto first_angle = getAngle(grid.path[grid.path.size() - 1], grid.path[0]);
	auto second_angle = getAngle(grid.path[0], grid.path[1]);
	if (first_angle == second_angle) {
		train_angle = first_angle;
		move_angle = first_angle;
	} else {
		train_angle = (first_angle + second_angle) / 2.;
		angle_speed = (second_angle - first_angle) / 100.;
		move_angle = second_angle;
	}
	std::cout << "Angle: " << train_angle << std::endl;

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

	myEngine.switchToPhongShading();
	myEngine.addALight(Vector4D(0., 0., 0., 0.), (Vector3D(0.)));
    myEngine.setLightPosition(Vector4D(1.0, 0.0, 1.0, 0.), 2);
	myEngine.setLightIntensity(Vector3D(0.5, 0.5, 0.5), 2);
	myEngine.switchToFlatShading();
}

void drawRoof() {
	myEngine.setFlatColor(0., 10. / 255., 10. / 255.);
	myEngine.mvMatrixStack.pushMatrix();
		myEngine.updateMvMatrix();
		if (realist_light) myEngine.setNormalForConvex2DShape(Vector3D(0., -1., 0.5));
		roof_left_top.drawShape();
	myEngine.mvMatrixStack.popMatrix();
	myEngine.mvMatrixStack.pushMatrix();
		myEngine.updateMvMatrix();
		if (realist_light) myEngine.setNormalForConvex2DShape(Vector3D(0., 1., 0.5));
		roof_right_top.drawShape();
	myEngine.mvMatrixStack.popMatrix();
	myEngine.mvMatrixStack.pushMatrix();
		myEngine.updateMvMatrix();
		if (realist_light) myEngine.setNormalForConvex2DShape(Vector3D(-1., 0., 0.));
		roofside.drawShape();
	myEngine.mvMatrixStack.popMatrix();
	myEngine.mvMatrixStack.pushMatrix();
		myEngine.mvMatrixStack.addTranslation(Vector3D(1., 0., 0.));
		myEngine.updateMvMatrix();
		if (realist_light) myEngine.setNormalForConvex2DShape(Vector3D(1., 0., 0.));
		roofside.drawShape();
	myEngine.mvMatrixStack.popMatrix();
}

void drawGrid() {

	if (realist_light) myEngine.setNormalForConvex2DShape(Vector3D(0., 0., 1.));
	for (auto i = 0; i < grid.size_grid; i++) {
		for (auto j = 0; j < grid.size_grid; j++) {
			myEngine.setFlatColor(0., 10. / 255., 0.);
			myEngine.mvMatrixStack.pushMatrix();
				myEngine.mvMatrixStack.addTranslation(Vector3D(10. * (i - grid.size_grid / 2), 10. * (j - grid.size_grid / 2), 0.));
				myEngine.updateMvMatrix();
				square.drawShape();
			myEngine.mvMatrixStack.popMatrix();
		}
	}
}

void drawFrame() {
	frame.drawSet();
}

void moveTrainAlongPath() {
	// Determine the new angle according to the path
	auto n = grid.path.size();
	auto train_front_x = train_pos_x;
	auto train_front_y = train_pos_y;

	auto cur_track_x = 10 * grid.path[current_path_index][0];
	auto cur_track_y = 10 * grid.path[current_path_index][1];

	if (angle_speed != 0. && abs(train_front_x - (cur_track_x + 5.)) < 0.1 && abs(train_front_y - (cur_track_y + 5.)) < 0.1) {
		move_angle = next_angle;		
	}


	if (train_front_x < cur_track_x || train_front_x > cur_track_x + 10. || train_front_y < cur_track_y || train_front_y > cur_track_y + 10.) {
		if (angle_speed != 0.) {
			train_angle = next_angle;
			angle_speed = 0.;
		}
		
		current_path_index = (current_path_index + 1) % n;

		auto second_angle = getAngle(grid.path[current_path_index], grid.path[(current_path_index + 1) % n]);
		if (train_angle != second_angle) {
			angle_speed = (second_angle - train_angle) / 100.;
			next_angle = second_angle;
		}
		
		
		
	}

	// Move the train forward
	auto move_x = train_speed * sin(move_angle);
	auto move_y = train_speed * cos(move_angle);


	train_pos_x -= move_x;
	train_pos_y += move_y;
	train_angle += angle_speed;
	
}


void drawScene() {
	glPointSize(10.0);
	
	drawFrame();
	
	if (realist_light) myEngine.switchToPhongShading();

	drawGrid();
	// angle_speed = 0.1;

	drawTrain(train_pos_x - 5., train_pos_y - 5., train_angle);
	
	drawStation(grid.origin[0], grid.origin[1]);
	drawTracksFromPath(grid);

	if (realist_light) myEngine.switchToFlatShading();
}



