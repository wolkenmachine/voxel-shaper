#ifndef OSCRECEIVER_H
#define OSCRECEIVER_H
#include "ofxOsc.h"
#include "ofMain.h"

#define PORT 12345


class oscReceiver
{
    public:
        oscReceiver();
        virtual ~oscReceiver();

        void connect();
        void update();

        ofxOscReceiver receiver;

        ofVec3f accelerometer;
        ofVec3f cuberotation;

    protected:
    private:
};

#endif // OSCRECEIVER_H
