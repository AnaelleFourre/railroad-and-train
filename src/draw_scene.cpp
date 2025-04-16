#include "draw_scene.hpp"
#include "draw_tracks.hpp"
#include "grid.hpp"

/// Camera parameters
float angle_theta {45.0};      // Angle between x axis and viewpoint
float angle_phy {30.0};      // Angle between z axis and viewpoint
float dist_zoom {30.0};      // Distance between origin and viewpoint

int grid_size {10};

GLBI_Engine myEngine;
GLBI_Set_Of_Points frame(3);
GLBI_Convex_2D_Shape grid{3};

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

	initTracks();

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

void drawFrame() {
	frame.drawSet();
}


void drawScene(const Grid& grid) {
	grid_size = grid.size_grid;
	glPointSize(10.0);
	
	drawFrame();
	drawGrid();

	drawTracksFromPath(grid);

	// drawCurvedTrack();
	// drawStraightTrack();
}



