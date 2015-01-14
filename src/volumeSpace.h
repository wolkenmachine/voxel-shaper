#ifndef VOLUMESPACE_H
#define VOLUMESPACE_H
#include "ofMain.h"
//#include <bitset>

class volumeSpace
{
    public:
        volumeSpace();
        virtual ~volumeSpace();
        void createSpace(int volsizex, int volsizey, int volsizez, bool val, int div); //space size and fill value
        void createSpaceFromfile(char * filname);
        void saveSpaceTofile(char * filename);

        void setVoxel(int x, int y, int z, bool val); //sets voxel to value
        bool getVoxel(int x, int y, int z); //gets voxel value
        int getVoxelNeighbours(int x, int y, int z); // gets amount of Neigbouring voxels that are TRUE (including self, so 27 if all)
        bool boundryCheck(int x, int y, int z); //returns whether a position is within boundry

        void updatePointcloud(int shader); //generate a pointcloud mesh from volume array
        void rebuildPointcloud(int shader);
        void drawPointcloud();
        void drawPointcloud(int x, int y, int z);
        void drawPointcloud(int x, int y, int z, ofColor col); //draw the pointcloud mesh


        bool getChunk(int x, int y, int z); //gets chunk value
        void setChunk(int x, int y, int z, bool val); //sets chunk value
        bool getChunkAtPosition(int x, int y, int z);
        void setChunkAtPosition(int x, int y, int z, bool val);

        int getMeshAtChunk(int x, int y, int z);

        int raycastcoloring(int x, int y, int z, int shader); //find the shading for a certain voxel
        bool ray(int x0, int y0, int z0, float x1, float y1, float z1, int length); // cast a ray from position 0 in direction 1 with a certain lenght, if lenght=0 the ray will be 'infinite'


        bool * volume;  //volume array
//        std::bitset<512*512*512> volume;
        bool * volumeChunks;  //volumechunks array

        int volumeResX,volumeResY,volumeResZ; //volume size values
        int volumeChunkResX,volumeChunkResY,volumeChunkResZ; //volume size values
        int chunkDevider;

        //ofMesh pointmesh; //mesh for pointcloud
        ofMesh * pointmeshArray; //meshes for pointcloud

        ofVec3f position;
        ofVec3f rotation;

        ofVec3f normal;




    protected:
    private:

};

#endif // VOLUMESPACE_H
