#include "Camera.h"

Camera::Camera()
{
    //ctor
}

Camera::~Camera()
{
    //dtor
}

void Camera::createCamera()
{
	easing = 0.1;
	targetX=0;
	targetY=0;
	targetZ=0;
	zoom=-400;
}

void Camera::draw()
{
    ofTranslate(ofGetWidth()/2,ofGetHeight()/2,zoom);
    ofRotateX(-camrotY*180+90);
    ofRotateZ(camrotX*180+180);
}

void Camera::update(ofVec3f acc)
{

			targetX = acc.x;
			targetY = acc.y;
			targetZ = acc.z;

			targetX = targetX/9.81;
			targetY = targetY/9.81;
			targetZ = targetZ/9.81;

            camrotX += (targetX - camrotX) * easing;
            camrotY += (targetY - camrotY) * easing;
            camrotZ += (targetZ - camrotZ) * easing;

}





