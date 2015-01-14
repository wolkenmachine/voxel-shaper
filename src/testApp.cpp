#include "testApp.h"
#include "volumeSpace.h"
#include "Camera.h"
#include "Cursor.h"
#include "ofxOsc.h"
#include "oscReceiver.h"


#include "time.h"


//--------------------------------------------------------------
void testApp::setup(){

    //ofSetFullscreen(true);
    ofSetVerticalSync(true);
    glEnable(GL_DEPTH_TEST);    // this uses depth information for occlusion rather than always drawing things on top of each other
	glPointSize(2); // make the points bigger

    ofBackground(128);

    cam.createCamera();     //create a new camera
    oscreceiver.connect();  //connect to osc port
    spacecursor.create();


    volumespace.createSpace(512,512,512,false,24);    //create empty space
    //volumespace.createSpace(110,110,110,false,5);
    volumebrush.createSpace(20,20,20,true,3);
    //volumebrush.createSpaceFromfile("brushvolume.vol");
    spacecursor.setBrush(volumebrush);

    mouseOffset = ofVec3f(0, 0, 0);     //set mouse values

    times=0; //set frame counter to 0
    isdrawing=false; //set isdrawing to false
    ofBackground(125,140,125); // set background color

    //start the mesh building thread
    renderer.import(volumespace);
    renderer.start();
}

//--------------------------------------------------------------
void testApp::update(){

    oscreceiver.update();
    cam.update(oscreceiver.accelerometer);
    spacecursor.update(oscreceiver.cuberotation);
    //volumebrush.rotation.z+=0.5;

    //update the pointcloud whilst drawing, times is for updating more sparsely (update every x frames)
    times++;
    if (isdrawing)
    {
        spacecursor.applyBrush(volumespace);
        if (times%5==1)
        {
            //volumespace.updatePointcloud(0);
        }

    }


}

//--------------------------------------------------------------
void testApp::draw(){

        ofFill();

        ofSetColor(189,216,186);
        ofCircle(32,32,23);
        ofSetColor(70, 90, 70);
        ofCircle(32,32,24);


        cam.draw();
        volumespace.drawPointcloud();
        spacecursor.draw();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
/*
int size=volumespace.chunkDevider*volumespace.chunkDevider*volumespace.chunkDevider;
for (int i; i<size; i++)
{
    cout << volumespace.volumeChunks[i] << ",";
}
*/
//volumespace.saveSpaceTofile("brushvolume.vol");

//volumespace.getChunkAtPosition(20,50,100);
    cout << "keypressed:" << key << "\n";
    if (key == 113)
    {
        cam.zoom+=10;
    }

    if (key == 97)
    {
        cam.zoom-=10;
    }

    if (key == 119)
    {

        //volumespace.rebuildPointcloud(1);
        //spacecursor.brush.rebuildPointcloud(1);
        renderer.rebuild=true;
    }

    	if (key == 'o'){

		//Open the Open File Dialog
		ofFileDialogResult openFileResult= ofSystemLoadDialog("Select a jpg or png");

		//Check if the user opened a file
		if (openFileResult.bSuccess){

			cout << "User selected a file";

			//We have a file, check it and process it
			std::string filestring = openFileResult.getPath();
            char * filename = new char[filestring.size()+1] ;
            strcpy(filename, filestring.c_str());
			volumebrush.createSpaceFromfile(filename);
            spacecursor.setBrush(volumebrush);

		}else {
			cout << "User hit cancel";
		}
	}

	if (key == 's'){
            volumespace.saveSpaceTofile("savedwork.vol");
	}


}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

    if ((button==0 )|| (button==2))
    {
        ofVec3f mouse = ofVec3f(mouseX, 0, mouseY);
        ofVec3f mouseMove = mouseOffset - mouse;
        mouseMove = mouseMove*.5;
        mouseMove.rotate(-cam.camrotY*180,0,-cam.camrotX*180); //make movement paralel to cameraview
        spacecursor.move(mouseMove);
    }

    if (button==1)
    {
        ofVec3f mouse = ofVec3f(mouseX, 0, mouseY);
        ofVec3f mouseMove = mouseOffset - mouse;
        mouseMove = mouseMove*.001;
        cam.targetX=mouseMove.x;
        cam.targetY=mouseMove.z;
    }

    if (button==0)
    {
        isdrawing=true;
        renderer.isdrawing=true;
    }
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

    mouseOffset.set(mouseX,0,mouseY);
    spacecursor.centerOffset();
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    isdrawing=false;
    renderer.isdrawing=false;
    //volumespace.updatePointcloud(0);

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){

}
