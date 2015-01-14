#include "oscReceiver.h"

oscReceiver::oscReceiver()
{
    //ctor
}


oscReceiver::~oscReceiver()
{
    //dtor
}

void oscReceiver::connect()
{
    cout << "listening for osc messages on port " << PORT << "\n";
	receiver.setup(PORT);
	accelerometer.set(0,0,0);
	cuberotation.set(0,0,0);

}

void oscReceiver::update()
{
//update camera rotation via OSC
    while(receiver.hasWaitingMessages()){
		// get the next message
		ofxOscMessage m;
		receiver.getNextMessage(&m);
		// check for accelerometer message
		//cout<<m.getAddress() <<"\n";
		if(m.getAddress() == "/accxyz"){
			float X = m.getArgAsFloat(0);
			float Y = m.getArgAsFloat(1);
			float Z = m.getArgAsFloat(2);

            accelerometer.set(X,Y,Z);
		}

        if(m.getAddress() == "/boxrot"){
			float X = m.getArgAsFloat(0);
			float Y = m.getArgAsFloat(1);
			//float Z = m.getArgAsFloat(2);

            cuberotation.set(X,Y,0);
		}
    }

}
