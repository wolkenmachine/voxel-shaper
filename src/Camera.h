#ifndef CAMERA_H
#define CAMERA_H

#include "oscReceiver.h"
#include "ofMain.h"

class Camera
{
    public:
        Camera();
        virtual ~Camera();
        void createCamera();
        void update(ofVec3f acc);
        void draw();


        float camrotX, camrotY, camrotZ;
        float targetX, targetY, targetZ;
        float easing;
        float zoom;


    protected:
    private:
};

#endif // CAMERA_H
