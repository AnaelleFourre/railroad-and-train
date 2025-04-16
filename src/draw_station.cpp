#include "draw_station.hpp"

GLBI_Convex_2D_Shape side_roof{3};
GLBI_Convex_2D_Shape center_roof{3};


void initStation()
{
    // Side roof
    std::vector<float> sr_points = {
        1, 1., 0.5,
        0., 0., 0.,
        0., 2., 0.,
        4., 2., 0.,
        4., 0., 0.,
        0., 0., 0.
    };
    side_roof.initShape(sr_points);
    side_roof.changeNature(GL_TRIANGLE_FAN);

    // Center roof
    std::vector<float> cr_points = {
        6., 0., 0., // 1, 7
        6., 2.5, 1., // 2, 6
        6., 5., 0., // 3
        0., 5., 0., // 4, 10
        0., 2.5, 1., // 5, 9
        6., 2.5, 1., // 2, 6
        6., 0., 0., // 1, 7
        0., 0., 0., // 8
        0., 2.5, 1., // 5, 9
        0., 5., 0., // 4, 10
    };
    center_roof.initShape(cr_points);
    center_roof.changeNature(GL_TRIANGLE_STRIP);
}

void drawSideBuilding() {
    myEngine.mvMatrixStack.pushMatrix();
        myEngine.setFlatColor(153. / 255, 51. / 255., 0.);
        myEngine.mvMatrixStack.pushMatrix();
            myEngine.mvMatrixStack.addHomothety(Vector3D(4., 2., 3.));
            myEngine.mvMatrixStack.addTranslation(Vector3D(0.5, 0.5, 0.5));
            myEngine.updateMvMatrix();
            cube->draw();
        myEngine.mvMatrixStack.popMatrix();
        myEngine.setFlatColor(51. / 255, 26. / 255., 0.);
        myEngine.mvMatrixStack.addTranslation(Vector3D{0., 0., 3.});
        myEngine.updateMvMatrix();
        side_roof.drawShape();
    myEngine.mvMatrixStack.popMatrix();
}

void drawCenterBuilding() {
    myEngine.mvMatrixStack.pushMatrix();
        myEngine.setFlatColor(153. / 255, 51. / 255., 0.);
        myEngine.mvMatrixStack.addHomothety(Vector3D(6., 4., 4.));
        myEngine.mvMatrixStack.addTranslation(Vector3D(0.5, 0.5, 0.5));
        myEngine.updateMvMatrix();
        cube->draw();
    myEngine.mvMatrixStack.popMatrix();
    myEngine.mvMatrixStack.pushMatrix();
        myEngine.setFlatColor(51. / 255, 26. / 255., 0.);
        myEngine.mvMatrixStack.addTranslation(Vector3D{0, -0.5, 4.});
        myEngine.updateMvMatrix();
        center_roof.drawShape();
    myEngine.mvMatrixStack.popMatrix();
}

void drawStation(int pos_x, int pos_y)
{
    myEngine.mvMatrixStack.pushMatrix();
        myEngine.mvMatrixStack.addTranslation(Vector3D(10. * pos_x, 10. * pos_y, 0.));
        myEngine.mvMatrixStack.pushMatrix();
            myEngine.mvMatrixStack.addTranslation(Vector3D{3., 1., 0.});
            drawSideBuilding();
        myEngine.mvMatrixStack.popMatrix();
        myEngine.mvMatrixStack.pushMatrix();
            myEngine.mvMatrixStack.addTranslation(Vector3D{2., 3., 0.});
            drawCenterBuilding();
        myEngine.mvMatrixStack.popMatrix();
        myEngine.mvMatrixStack.pushMatrix();
            myEngine.mvMatrixStack.addTranslation(Vector3D{3., 7., 0.});
            drawSideBuilding();
        myEngine.mvMatrixStack.popMatrix();
    myEngine.mvMatrixStack.popMatrix();
}
