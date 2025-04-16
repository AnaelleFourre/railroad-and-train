#include "draw_tracks.hpp"
#include "draw_scene.hpp"

#include "grid.hpp"

GLBI_Convex_2D_Shape curved_rail_int{3};
GLBI_Convex_2D_Shape curved_rail_ext{3};
GLBI_Convex_2D_Shape disc{3};
IndexedMesh* cylinder;


void computeCurve(const Vector3D & a0, const Vector3D & a1, const Vector3D & a2,
    const Vector3D & b0, const Vector3D & b1, const Vector3D & b2, 
    std::vector<float>& points, bool reverse) {

    auto nb_points = 50;
    Vector3D b0_1, b1_1, a0_1, a1_1;
    Vector3D b0_2, a0_2;
    float t;
    for (auto i = 0; i <= nb_points; i++) {
        t = (float) i / nb_points;
        if (reverse) {
            t = (1 - t);
        }

        b0_1 = b0 * (1 - t) + b1 * t;
        b1_1 = b1 * (1 - t) + b2 * t;
        b0_2 = b0_1 * (1 - t) + b1_1 * t;
        points.emplace_back(b0_2[0]);
        points.emplace_back(b0_2[1]);
        points.emplace_back(b0_2[2]);

        a0_1 = a0 * (1 - t) + a1 * t;
        a1_1 = a1 * (1 - t) + a2 * t;
        a0_2 = a0_1 * (1 - t) + a1_1 * t;
        points.emplace_back(a0_2[0]);
        points.emplace_back(a0_2[1]);
        points.emplace_back(a0_2[2]);
    }
}

void initCurvedRail(float size, GLBI_Convex_2D_Shape& shape) {
    std::vector<float> points;

    auto a0 = Vector3D(0., size, 0.);
    auto a1 = Vector3D(size, size, 0.);
    auto a2 = Vector3D(size, 0., 0.);

    auto b0 = Vector3D(0., size + sr , 0);
    auto b1 = Vector3D(size + sr , size + sr , 0);
    auto b2 = Vector3D(size + sr , 0., 0);

    auto c0 = Vector3D(0., size + sr , sr);
    auto c1 = Vector3D(size + sr , size + sr , sr);
    auto c2 = Vector3D(size + sr , 0., sr);

    auto d0 = Vector3D(0., size, sr);
    auto d1 = Vector3D(size, size, sr);
    auto d2 = Vector3D(size, 0., sr);

    computeCurve(a0, a1, a2, b0, b1, b2, points, false);
    computeCurve(b0, b1, b2, c0, c1, c2, points, true);
    computeCurve(c0, c1, c2, d0, d1, d2, points, false);
    computeCurve(d0, d1, d2, a0, a1, a2, points, true);

    shape.initShape(points);
    shape.changeNature(GL_TRIANGLE_STRIP);
}


void initTracks() {
    
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
	// disc.changeNature(GL_TRIANGLE_STRIP_ADJACENCY);

	// Balast
	cylinder = basicCylinder(1., 1.);
	cylinder->createVAO();

    // Curved Rail
	initCurvedRail(POS_X_RAIL1, curved_rail_int);
	initCurvedRail(POS_X_RAIL2 - sr, curved_rail_ext);
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
		myEngine.mvMatrixStack.addTranslation(Vector3D(POS_X_RAIL2 - sr, 0., 2 * rr));
		drawRail();
	myEngine.mvMatrixStack.popMatrix();
}

void drawCurvedTrack() {
	std::vector<float> balasts_rotation = {M_PI / 12, M_PI / 4, 5 * M_PI / 12};

	for (auto i = 0; i < 3; i++) {
		myEngine.mvMatrixStack.pushMatrix();
			myEngine.mvMatrixStack.addRotation(balasts_rotation[i], Vector3D(0., 0., 1.));
			myEngine.mvMatrixStack.addTranslation(Vector3D(POS_X_BALAST1, 0., 0.));
			drawBalast();
		myEngine.mvMatrixStack.popMatrix();
	}
	myEngine.mvMatrixStack.pushMatrix();
		myEngine.setFlatColor(140. / 255, 140. / 255., 140. / 255.);
		myEngine.mvMatrixStack.addTranslation(Vector3D(0., 0., 2 * rr));
		myEngine.updateMvMatrix();
		curved_rail_int.drawShape();
		curved_rail_ext.drawShape();
	myEngine.mvMatrixStack.popMatrix();
}

std::vector<int> getDirection(std::vector<int> a, std::vector<int> b) {
    return std::vector<int>{b[0] - a[0], b[1] - a[1]};
}


void rotate(int n) {
    switch (n)
	{
        case 1:
		myEngine.mvMatrixStack.addTranslation(Vector3D(10., 0., 0.));
		break;
		case 2:
		myEngine.mvMatrixStack.addTranslation(Vector3D(10., 10., 0.));
		break;
		case 3:
		myEngine.mvMatrixStack.addTranslation(Vector3D(0., 10., 0.));
		break;
		default:
		break;
	}
    myEngine.mvMatrixStack.addRotation(n * M_PI_2, Vector3D(0., 0., 1.));
}

void drawTracksFromPath(const Grid& grid) {

    int n = grid.path.size();
    auto current_direction = getDirection(grid.path[n - 1], grid.path[0]);
    for (auto i = 0; i < n; i++) {
        myEngine.mvMatrixStack.pushMatrix();
            auto next_direction = getDirection(grid.path[i], grid.path[(i + 1) % n]); 
            myEngine.mvMatrixStack.addTranslation(Vector3D(10. * grid.path[i][0], 10. * grid.path[i][1], 0.));
            if (next_direction == current_direction) {
                if (current_direction[0] != 0)
                    rotate(1);
                drawStraightTrack();
            } else {
                auto angle = std::vector<int>{(next_direction[0] - current_direction[0]), (next_direction[1] - current_direction[1])};
                if (angle[0] == 1 && angle[1] == 1) 
                    rotate(2);
                if (angle[0] == -1 && angle[1] == 1) 
                    rotate(3);
                if (angle[0] == 1 && angle[1] == -1) 
                    rotate(1);
                drawCurvedTrack();
            }
            current_direction = next_direction;

        myEngine.mvMatrixStack.popMatrix();
    }

}