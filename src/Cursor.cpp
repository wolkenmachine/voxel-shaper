#include "Cursor.h"

spaceCursor::spaceCursor()
{

}

spaceCursor::~spaceCursor()
{
    //dtor
}

void spaceCursor::create()
{
    cursor = ofVec3f(0, 0, 0);          //set cursor values
    cursorOffset = ofVec3f(0, 0, 0);
    cursorRotation = ofVec3f(0, 0, 0);
}

void spaceCursor::setBrush(volumeSpace space)
{
brush = space;
}

void spaceCursor::draw()
{
brush.drawPointcloud(cursor.x,cursor.y,cursor.z,ofColor( 255, 0, 0 ));
}

void spaceCursor::applyBrush(volumeCanvas canvas)
{
canvas.paintBrush(cursor.x,cursor.y,cursor.z,brush);
}

void spaceCursor::centerOffset()
{
cursorOffset.set(cursor.x,cursor.y,cursor.z);
}

void spaceCursor::move(ofVec3f move)
{
cursorTarget = cursorOffset + move;

}

void spaceCursor::update(ofVec3f move)
{
cursorRotationTarget=move;
cursorRotation += (cursorRotationTarget - cursorRotation)*0.1;
brush.rotation=cursorRotation*360;

cursor += (cursorTarget - cursor)*0.9;

//cout << cursor.x << "," << cursor.y << "," << cursor.z << "\n";
}
