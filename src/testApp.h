#pragma once
#include "ofMain.h"
#include "volumeCanvas.h"
#include "Camera.h"
#include "Cursor.h"

#include "threadedObject.h"

class testApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

        Camera cam;
        oscReceiver oscreceiver;
        spaceCursor spacecursor;
		volumeCanvas volumespace;
		volumeSpace volumebrush;

		//int plus;
		int times; //for timing frames


        ofVec3f mouseOffset;

        //int mouseXoffset, mouseYoffset; //mouse offset
		//int cursorX,cursorY,cursorZ; //cursor(brush) position
		//int cursorXoffset, cursorYoffset, cursorZoffset;

		bool isdrawing;

        threadedObject renderer;


};
