#ifndef VOLUMECANVAS_H
#define VOLUMECANVAS_H

#include <volumeSpace.h>


class volumeCanvas : public volumeSpace
{
    public:
        volumeCanvas();
        virtual ~volumeCanvas();

        void paintBrush(int x, int y, int z, volumeSpace brush);


    protected:
    private:
};

#endif // VOLUMECANVAS_H
