#include "draw_train.hpp"
#include "draw_scene.hpp"

GLBI_Convex_2D_Shape circle{3};
GLBI_Convex_2D_Shape cowCatcher{3};

void addCircleSide(std::vector<float>& points, float r1, float z1, float r2, float z2) {
    float angle;
    for (auto i = 0; i < 360; i++) {
        angle = M_PI * i / 180;

        points.push_back(cos(angle) * r1);
        points.push_back(sin(angle) * r1);
        points.push_back(z1);

        points.push_back(cos(angle) * r2);
        points.push_back(sin(angle) * r2);
        points.push_back(z2);
    }
    
}


void initTrain(){

    // Circle
    std::vector<float> circlePoints;

    addCircleSide(circlePoints, 1., 0., 1., sr);
    addCircleSide(circlePoints, 1., sr, 0.9, sr);
    addCircleSide(circlePoints, 0.9, sr, 0.9, 0.);
    addCircleSide(circlePoints, 0.9, 0., 1., 0.);

    circle.initShape(circlePoints);
    circle.changeNature(GL_TRIANGLE_STRIP);

    // CowCatcher
    std::vector<float> points = {
        5., 11.5, 0.5,
        3., 10., 0.5,
        4., 10., 3.5,
        6., 10., 3.5,
        7., 10., 0.5,
    };

    cowCatcher.initShape(points);
    cowCatcher.changeNature(GL_TRIANGLE_FAN);

}

void drawClosedCylinder() {
    myEngine.mvMatrixStack.pushMatrix();
        myEngine.mvMatrixStack.pushMatrix();
            myEngine.mvMatrixStack.addTranslation(Vector3D(0., 1., 0.));
            myEngine.mvMatrixStack.addRotation(M_PI_2, Vector3D(1., 0., 0.));
            myEngine.updateMvMatrix();
            disc.drawShape();
        myEngine.mvMatrixStack.popMatrix(); 
        myEngine.updateMvMatrix();
        cylinder->draw();
    myEngine.mvMatrixStack.popMatrix();
}

void drawBottomStructure() {
    myEngine.mvMatrixStack.pushMatrix();
        myEngine.mvMatrixStack.addHomothety(Vector3D(4 - 2 * sr, 10., 2.));
        myEngine.mvMatrixStack.addTranslation(Vector3D(0.5, 0.5, 0.5));
        myEngine.updateMvMatrix();
        cube->draw();
    myEngine.mvMatrixStack.popMatrix();
}

void drawBackWagon() {
    myEngine.mvMatrixStack.pushMatrix();
        myEngine.mvMatrixStack.addHomothety(Vector3D(4 - 2 * sr, 5., 5.));
        myEngine.mvMatrixStack.addTranslation(Vector3D(0.5, 0.5, 0.5));
        myEngine.updateMvMatrix();
        cube->draw();
    myEngine.mvMatrixStack.popMatrix();
}

void drawFrontWagon(){
    myEngine.mvMatrixStack.pushMatrix();
        myEngine.mvMatrixStack.addHomothety(Vector3D(2 - sr, 5., 2 - sr));
        myEngine.mvMatrixStack.addTranslation(Vector3D(1., 0., 1.));
        drawClosedCylinder();
    myEngine.mvMatrixStack.popMatrix();
}

void drawChimney() {
    myEngine.mvMatrixStack.pushMatrix();
        myEngine.mvMatrixStack.addHomothety(Vector3D(0.5, 0.5, 1.5));
        myEngine.mvMatrixStack.addTranslation(Vector3D(0., 1., 0.));
        myEngine.mvMatrixStack.addRotation(M_PI_2, Vector3D(1., 0., 0.));
        drawClosedCylinder();
    myEngine.mvMatrixStack.popMatrix();
}

void drawTrainRoof() {
    myEngine.mvMatrixStack.pushMatrix();
        myEngine.mvMatrixStack.addHomothety(Vector3D(5., 6., 1.));
        myEngine.mvMatrixStack.addTranslation(Vector3D(1., 0., 0.));
        myEngine.mvMatrixStack.addRotation(M_PI_2, Vector3D(0., 0., 1.));
        myEngine.updateMvMatrix();
        roof.drawShape();
    myEngine.mvMatrixStack.popMatrix();
}

void drawStructure() {
    myEngine.mvMatrixStack.pushMatrix();
        myEngine.setFlatColor(0., 51. / 255., 102. / 255.);
        myEngine.mvMatrixStack.pushMatrix();
            myEngine.mvMatrixStack.addTranslation(Vector3D(3. + sr, 0., 3.5));
            drawBackWagon();
        myEngine.mvMatrixStack.popMatrix();
        myEngine.mvMatrixStack.pushMatrix();
            myEngine.mvMatrixStack.addTranslation(Vector3D(3 + sr, 5., 3.5));
            drawFrontWagon();
        myEngine.mvMatrixStack.popMatrix();
    myEngine.mvMatrixStack.popMatrix();

    myEngine.mvMatrixStack.pushMatrix();
        myEngine.setFlatColor(0., 0., 0.);
        myEngine.mvMatrixStack.pushMatrix();
            myEngine.mvMatrixStack.addTranslation(Vector3D(3. + sr, 0., 1.5));
            drawBottomStructure();
        myEngine.mvMatrixStack.popMatrix();
        myEngine.mvMatrixStack.pushMatrix();
            myEngine.mvMatrixStack.addTranslation(Vector3D(5., 8., 7 - 2 * sr));
            drawChimney();
        myEngine.mvMatrixStack.popMatrix();
        myEngine.mvMatrixStack.pushMatrix();
            myEngine.mvMatrixStack.addTranslation(Vector3D(2.5, -0.5, 8.5));
            drawTrainRoof();
        myEngine.mvMatrixStack.popMatrix();
    myEngine.mvMatrixStack.popMatrix();
}

void drawWheel() {

    myEngine.mvMatrixStack.pushMatrix();
        myEngine.setFlatColor(0., 0., 0.);
        myEngine.mvMatrixStack.pushMatrix();
            myEngine.mvMatrixStack.addHomothety(Vector3D(0.1, 0.1, 0.1));
            myEngine.mvMatrixStack.addRotation(M_PI_2, Vector3D(1., 0., 0.));
            drawClosedCylinder();
        myEngine.mvMatrixStack.popMatrix();

        auto nb_bars = 15;
        for (auto i = 0; i < nb_bars; i++) {
            myEngine.mvMatrixStack.pushMatrix();
                myEngine.mvMatrixStack.addRotation(i * (2 * M_PI / nb_bars), Vector3D(0., 0., 1.));
                myEngine.mvMatrixStack.addHomothety(Vector3D(0.1, 1., sr));
                myEngine.mvMatrixStack.addTranslation(Vector3D(0.5, 0.5, 0.5));
                myEngine.updateMvMatrix();
                cube->draw();
            myEngine.mvMatrixStack.popMatrix();
        }

        myEngine.updateMvMatrix();
        circle.drawShape();
    myEngine.mvMatrixStack.popMatrix();


}

void drawWheels() {
    myEngine.mvMatrixStack.pushMatrix();
        myEngine.mvMatrixStack.addTranslation(Vector3D(7 - sr, 2., 2.5));
        myEngine.mvMatrixStack.addRotation(M_PI_2, Vector3D(0., 1., 0.));
        myEngine.mvMatrixStack.addHomothety(Vector3D(2.5, 2.5, 1.));
        drawWheel();
    myEngine.mvMatrixStack.popMatrix();
    myEngine.mvMatrixStack.pushMatrix();
        myEngine.mvMatrixStack.addTranslation(Vector3D(3. + sr, 2., 2.5));
        myEngine.mvMatrixStack.addRotation(-M_PI_2, Vector3D(0., 1., 0.));
        myEngine.mvMatrixStack.addHomothety(Vector3D(2.5, 2.5, 1.));
        drawWheel();
    myEngine.mvMatrixStack.popMatrix();
    myEngine.mvMatrixStack.pushMatrix();
        myEngine.mvMatrixStack.addTranslation(Vector3D(7 - sr, 8.5, 1.5));
        myEngine.mvMatrixStack.addRotation(M_PI_2, Vector3D(0., 1., 0.));
        myEngine.mvMatrixStack.addHomothety(Vector3D(1.5, 1.5, 1.));
        drawWheel();
    myEngine.mvMatrixStack.popMatrix();
    myEngine.mvMatrixStack.pushMatrix();
        myEngine.mvMatrixStack.addTranslation(Vector3D(3 + sr, 8.5, 1.5));
        myEngine.mvMatrixStack.addRotation(-M_PI_2, Vector3D(0., 1., 0.));
        myEngine.mvMatrixStack.addHomothety(Vector3D(1.5, 1.5, 1.));
        drawWheel();
    myEngine.mvMatrixStack.popMatrix();
}

void drawCowCatcher() {
    myEngine.mvMatrixStack.pushMatrix();
        myEngine.setFlatColor(0., 0., 0.);
        myEngine.updateMvMatrix();
        cowCatcher.drawShape();
    myEngine.mvMatrixStack.popMatrix();
}

void drawLight() {
    myEngine.mvMatrixStack.pushMatrix();
        myEngine.mvMatrixStack.addTranslation(Vector3D(5, 10., 5.5 - sr));
        myEngine.mvMatrixStack.pushMatrix();
            myEngine.setFlatColor(1., 1., 128. / 255.);
            myEngine.mvMatrixStack.addHomothety(Vector3D(1, 0.5, 1));
            myEngine.updateMvMatrix();
            light->draw();
        myEngine.mvMatrixStack.popMatrix();
        myEngine.mvMatrixStack.pushMatrix();
            myEngine.setFlatColor(0., 0., 0.);
            myEngine.mvMatrixStack.addRotation(-M_PI_2, Vector3D(1., 0., 0.));
            myEngine.updateMvMatrix();
            circle.drawShape();
        myEngine.mvMatrixStack.popMatrix();
    myEngine.mvMatrixStack.popMatrix();
}

void drawTrain(int pos_x, int pos_y){
    
    myEngine.mvMatrixStack.pushMatrix();
        myEngine.mvMatrixStack.addTranslation(Vector3D(10. * pos_x, 10. * pos_y, sr + 2 * rr));
        drawStructure();
        drawWheels();
        drawCowCatcher();
        drawLight();
    myEngine.mvMatrixStack.popMatrix();

}