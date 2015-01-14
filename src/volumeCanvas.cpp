#include "volumeCanvas.h"
#include "volumeSpace.h"

volumeCanvas::volumeCanvas()
{
    //ctor
}

volumeCanvas::~volumeCanvas()
{
    //dtor
}

void volumeCanvas::paintBrush(int x, int y, int z, volumeSpace brush)
{
    x=x+((volumeResX/2)-(brush.volumeResX/2));
    y=y+((volumeResY/2)-(brush.volumeResY/2));
    z=z+((volumeResZ/2)-(brush.volumeResZ/2));
    ofVec3f brushcenter(brush.volumeResX/2,brush.volumeResY/2,brush.volumeResZ/2);

    for(int bx=0; bx<brush.volumeResX; bx++)
        {
             for(int by=0; by<brush.volumeResY; by++)
             {
                     for(int bz=0; bz<brush.volumeResZ; bz++)
                     {
                        if (brush.getVoxel(bx,by,bz))
                        {

                            ofVec3f brushvoxel(bx,by,bz);
                            brushvoxel=brushvoxel-brushcenter;
                            brushvoxel.rotate(brush.rotation.x,brush.rotation.y,brush.rotation.z);
                            brushvoxel=brushvoxel+brushcenter;

                            int rbx=int(brushvoxel.x);
                            int rby=int(brushvoxel.y);
                            int rbz=int(brushvoxel.z);

                            if (boundryCheck(x+rbx,y+rby,z+rbz))
                            {
                                setVoxel(x+rbx,y+rby,z+rbz, true);
                            }

                        }

                     }
             }
        }
}



