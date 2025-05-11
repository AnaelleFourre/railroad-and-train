#include "draw_train.hpp"
#include "draw_scene.hpp"

GLBI_Convex_2D_Shape wheelSide{3};
GLBI_Convex_2D_Shape cowCatcher{3};

void computeWheelSide(std::vector<float>& points, float r1, float r2) {
    float angle;
    for (auto i = 0; i <= 360; i++) {
        angle = M_PI * (i % 360) / 180;

        points.push_back(cos(angle) * r1);
        points.push_back(sin(angle) * r1);
        points.push_back(0.);

        points.push_back(cos(angle) * r2);
        points.push_back(sin(angle) * r2);
        points.push_back(0.);
    }
    
}


void initTrain(float pos_x, float pos_y) {

    // Wheel side
    std::vector<float> wheelSidePoints;
    computeWheelSide(wheelSidePoints, 0.9, 1.);

    wheelSide.initShape(wheelSidePoints);
    wheelSide.changeNature(GL_TRIANGLE_STRIP);

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
    
    
	myEngine.switchToPhongShading();

    myEngine.setLightPosition(Vector4D(10. * pos_x + 5., 10. * pos_y + 11., 5.5 + 2 * rr, -1.), 0);
	myEngine.setLightIntensity(Vector3D(200., 200., 100.), 0);

    myEngine.addALight(Vector4D(0., 0., 0., 0.), (Vector3D(0.)));
    myEngine.setLightPosition(Vector4D(0., 1., 0., 0.), 1);
	myEngine.setLightIntensity(Vector3D(0.), 1);

    myEngine.switchToFlatShading();

}

void drawClosedCylinder() {
    myEngine.mvMatrixStack.pushMatrix();
        myEngine.mvMatrixStack.pushMatrix();
            myEngine.mvMatrixStack.addTranslation(Vector3D(0., 1., 0.));
            myEngine.mvMatrixStack.addRotation(M_PI_2, Vector3D(1., 0., 0.));
            myEngine.updateMvMatrix();
            if (realist_light) myEngine.setNormalForConvex2DShape(Vector3D(0., 0., 1.));
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
        drawRoof();
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
        myEngine.setFlatColor(10. / 255., 10. / 255., 10. / 255.);
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


void drawThickCircle(float r, float h, float w) {
    myEngine.mvMatrixStack.pushMatrix();
        myEngine.mvMatrixStack.addRotation(M_PI_2, Vector3D(1., 0., 0.));
        myEngine.mvMatrixStack.addHomothety(Vector3D(r - h, w, r - h));
        myEngine.updateMvMatrix();
        cylinder->draw();
    myEngine.mvMatrixStack.popMatrix();
    myEngine.mvMatrixStack.pushMatrix(); 
        myEngine.mvMatrixStack.addRotation(M_PI_2, Vector3D(1., 0., 0.));
        myEngine.mvMatrixStack.addHomothety(Vector3D(r, w, r));
        myEngine.updateMvMatrix();
        cylinder->draw();
    myEngine.mvMatrixStack.popMatrix();

    myEngine.mvMatrixStack.pushMatrix();
        myEngine.mvMatrixStack.addHomothety(Vector3D(r, r, 1.));
        myEngine.mvMatrixStack.pushMatrix();
            if (realist_light) myEngine.setNormalForConvex2DShape(Vector3D(0., 0., -1.));
            myEngine.updateMvMatrix();
            wheelSide.drawShape();
        myEngine.mvMatrixStack.popMatrix();
        myEngine.mvMatrixStack.pushMatrix();
            myEngine.mvMatrixStack.addTranslation(Vector3D(0., 0., w));
            myEngine.updateMvMatrix();
            if (realist_light) myEngine.setNormalForConvex2DShape(Vector3D(0., 0., 1.));
            wheelSide.drawShape();
        myEngine.mvMatrixStack.popMatrix();
    myEngine.mvMatrixStack.popMatrix();
}

void drawWheel() {

    myEngine.mvMatrixStack.pushMatrix();
        myEngine.setFlatColor(0., 10. / 255., 10. / 255.);
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

        drawThickCircle(1., 0.1, sr);
       
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
        myEngine.setFlatColor(0., 10. / 255., 10. / 255.);
        myEngine.updateMvMatrix();
        cowCatcher.drawShape();
    myEngine.mvMatrixStack.popMatrix();
}

void drawLight() {
    myEngine.mvMatrixStack.pushMatrix();
        myEngine.mvMatrixStack.addTranslation(Vector3D(5, 10., 5.5 - sr));
        myEngine.mvMatrixStack.pushMatrix();
            // Switching to flat shading to allow the light to be drawn
            if (realist_light) myEngine.switchToFlatShading();
            myEngine.setFlatColor(1., 1., 128. / 255.);
            myEngine.mvMatrixStack.addHomothety(Vector3D(1, 0.5, 1));
            myEngine.updateMvMatrix();
            light->draw();
            if (realist_light) myEngine.switchToPhongShading();
        myEngine.mvMatrixStack.popMatrix();
        myEngine.mvMatrixStack.pushMatrix();
            myEngine.setFlatColor(0., 10. / 255., 10. / 255.);
            myEngine.mvMatrixStack.addTranslation(Vector3D(0., -0.25, 0.));
            myEngine.mvMatrixStack.addRotation(-M_PI_2, Vector3D(1., 0., 0.));
            drawThickCircle(1., 0.1, 0.5);
        myEngine.mvMatrixStack.popMatrix();
    myEngine.mvMatrixStack.popMatrix();
}

void drawTrain(float pos_x, float pos_y, float angle) {
    
    myEngine.mvMatrixStack.pushMatrix();
        myEngine.mvMatrixStack.addTranslation(Vector3D(pos_x, pos_y, sr + 2 * rr));
        myEngine.mvMatrixStack.addTranslation(Vector3D(5., 5., 0.));
        myEngine.mvMatrixStack.addRotation(angle, Vector3D(0., 0., 1.));
        myEngine.mvMatrixStack.addTranslation(Vector3D(-5., -5., 0.));
        drawStructure();
        drawWheels();
        drawCowCatcher();
        drawLight();
    myEngine.mvMatrixStack.popMatrix();

}