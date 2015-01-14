#ifndef CURSOR_H
#define CURSOR_H

#include "volumeSpace.h"
#include "volumeCanvas.h"



class spaceCursor
{
    public:
        spaceCursor();
        virtual ~spaceCursor();
        void create();

        void draw();
        void centerOffset();
        void move(ofVec3f move);
        void update(ofVec3f move);

        void setBrush(volumeSpace space);
        void applyBrush(volumeCanvas canvas);

        ofVec3f cursor;
        ofVec3f cursorTarget;
        ofVec3f cursorOffset;
        ofVec3f cursorRotation;
        ofVec3f cursorRotationTarget;

        int cursormode;

        volumeSpace brush;

    protected:
    private:
};

#endif // CURSOR_H
