#include "draw_station.hpp"
#include "draw_scene.hpp"
#include "glbasimac/glbi_texture.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include <tools/stb_image.h>

GLBI_Convex_2D_Shape side_roof{3};
GLBI_Texture brick_texture;

void initStation()
{
    // Side roof
    std::vector<float> sr_points = {
        0., 0., 0.,
        4., 0., 0.,
        2., 1., 0.5,
    };
    side_roof.initShape(sr_points);
    side_roof.changeNature(GL_TRIANGLE_FAN);
    
    
    int width, height, channels;
    auto image = stbi_load("../assets/textures/brick.jpg", &width, &height, &channels, 0);
    if (image == NULL) {
        std::cerr << "Unable to load ../assets/textures/brick.jpg" << std::endl;
    }
    
    brick_texture.createTexture();

    brick_texture.attachTexture();
    brick_texture.loadImage(width, height, channels, image);
    brick_texture.setParameters(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    brick_texture.detachTexture();
}

void drawCenterRoof() {
    myEngine.mvMatrixStack.pushMatrix();
        myEngine.mvMatrixStack.addHomothety(Vector3D(6., 5., 1.));
        drawRoof();
    myEngine.mvMatrixStack.popMatrix();
}

void drawSideRoof() {
    myEngine.mvMatrixStack.pushMatrix();
        myEngine.updateMvMatrix();
        myEngine.setNormalForConvex2DShape(Vector3D(0., -1., 2.));
        side_roof.drawShape();
    myEngine.mvMatrixStack.popMatrix();
    myEngine.mvMatrixStack.pushMatrix();
        myEngine.mvMatrixStack.addTranslation(Vector3D(4., 0., 0.));
        myEngine.mvMatrixStack.addHomothety(Vector3D(2., 0.5, 1.));
        myEngine.mvMatrixStack.addRotation(M_PI_2, Vector3D(0., 0., 1.));
        myEngine.updateMvMatrix();
        myEngine.setNormalForConvex2DShape(Vector3D(1., 0., 4.));
        side_roof.drawShape();
    myEngine.mvMatrixStack.popMatrix();
    myEngine.mvMatrixStack.pushMatrix();
        myEngine.mvMatrixStack.addTranslation(Vector3D(4., 2., 0.));
        myEngine.mvMatrixStack.addRotation(M_PI, Vector3D(0., 0., 1.));
        myEngine.updateMvMatrix();
        myEngine.setNormalForConvex2DShape(Vector3D(0., 1., 2.));
        side_roof.drawShape();
    myEngine.mvMatrixStack.popMatrix();
    myEngine.mvMatrixStack.pushMatrix();
        myEngine.mvMatrixStack.addTranslation(Vector3D(0., 2., 0.));
        myEngine.mvMatrixStack.addHomothety(Vector3D(2., 0.5, 1.));
        myEngine.mvMatrixStack.addRotation(-M_PI_2, Vector3D(0., 0., 1.));
        myEngine.updateMvMatrix();
        myEngine.setNormalForConvex2DShape(Vector3D(-1., 0., 4.));
        side_roof.drawShape();
    myEngine.mvMatrixStack.popMatrix();
}

void drawBrickBuilding() {
    myEngine.mvMatrixStack.pushMatrix();
        myEngine.mvMatrixStack.addRotation(M_PI_2, Vector3D(1., 0., 0.));
        myEngine.mvMatrixStack.addRotation(M_PI_2, Vector3D(0., 1., 0.));
        myEngine.updateMvMatrix();
        myEngine.activateTexturing(true);            
        brick_texture.attachTexture();
        cube->draw();
        brick_texture.detachTexture();
        myEngine.activateTexturing(false);
    myEngine.mvMatrixStack.popMatrix();
}

void drawSideBuilding() {
    myEngine.mvMatrixStack.pushMatrix();
        myEngine.setFlatColor(153. / 255, 51. / 255., 0.);
        myEngine.mvMatrixStack.pushMatrix();
            myEngine.mvMatrixStack.addHomothety(Vector3D(4., 2., 3.));
            myEngine.mvMatrixStack.addTranslation(Vector3D(0.5, 0.5, 0.5));
            myEngine.updateMvMatrix();
            drawBrickBuilding();
        myEngine.mvMatrixStack.popMatrix();
        myEngine.setFlatColor(51. / 255, 26. / 255., 0.);
        myEngine.mvMatrixStack.addTranslation(Vector3D{0., 0., 3.});
        myEngine.updateMvMatrix();
        drawSideRoof();
    myEngine.mvMatrixStack.popMatrix();
}

void drawCenterBuilding() {
    myEngine.mvMatrixStack.pushMatrix();
        myEngine.setFlatColor(153. / 255, 51. / 255., 0.);
        myEngine.mvMatrixStack.addHomothety(Vector3D(6., 4., 4.));
        myEngine.mvMatrixStack.addTranslation(Vector3D(0.5, 0.5, 0.5));
        myEngine.updateMvMatrix();
        drawBrickBuilding();
    myEngine.mvMatrixStack.popMatrix();
    myEngine.mvMatrixStack.pushMatrix();
        myEngine.setFlatColor(51. / 255, 26. / 255., 0.);
        myEngine.mvMatrixStack.addTranslation(Vector3D{0, -0.5, 4.});
        drawCenterRoof();
    myEngine.mvMatrixStack.popMatrix();
}

void drawStation(int pos_x, int pos_y)
{

    myEngine.mvMatrixStack.pushMatrix();
        myEngine.mvMatrixStack.addTranslation(Vector3D(10. * (pos_x - 1) , 10. * (pos_y - 1), 0.));
        myEngine.mvMatrixStack.addHomothety(Vector3D(2., 2., 2.));
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
