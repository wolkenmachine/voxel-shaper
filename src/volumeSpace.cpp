#include "volumeSpace.h"
#include <iostream>
#include <fstream>
#include <string>
#include <bitset>

volumeSpace::volumeSpace()
{

}

volumeSpace::~volumeSpace()
{
    //dtor
}

void volumeSpace::createSpace(int volsizex, int volsizey, int volsizez, bool val, int div)
{
    //create a new volume array
    volumeResX=volsizex;
    volumeResY=volsizey;
    volumeResZ=volsizez;

    int size=volumeResX*volumeResY*volumeResZ;
    volume = new bool[size];
    for(int i=0; i<size; i++)
    {volume[i]=val;}


    //create a new chunk array
    chunkDevider=div;
    volumeChunkResX=volumeResX/chunkDevider;
    volumeChunkResY=volumeResY/chunkDevider;
    volumeChunkResZ=volumeResZ/chunkDevider;

    int chunkSize=chunkDevider*chunkDevider*chunkDevider;
    volumeChunks = new bool[chunkSize];
    for(int i=0; i<chunkSize; i++)
    {volumeChunks[i]=val;}

    //update the pointcloud
    pointmeshArray = new ofMesh[chunkSize];
    for(int i=0; i<chunkSize; i++)
    {pointmeshArray[i].clear();}
    updatePointcloud(0);
    cout << "created a volumespace with dimensions: " << volumeResX << "x" << volumeResY << "x" << volumeResZ <<"\n";

    //set position and rotation of the volumespace
    rotation.set(0,0,0);
    position.set(0,0,0);

}

void volumeSpace::createSpaceFromfile(char * filename)
{

    string line;
    ifstream myfile (filename);
    int i=0;
    if (myfile.is_open())
       {
           getline (myfile,line);
           int volsizex = atoi(line.c_str());
           getline (myfile,line);
           int volsizey = atoi(line.c_str());
           getline (myfile,line);
           int volsizez = atoi(line.c_str());
           createSpace(volsizex,volsizey,volsizez,false,1);
        while ( myfile.good() )
        {
            getline (myfile,line);
            if (line=="1") {volume[i]=true;}
            i++;
            //cout << line;
        }
        myfile.close();
        cout << "Opened a volumespace from file: " << filename << "\n";
      }

      else cout << "Unable to open file" << "\n";
      rebuildPointcloud(0);
}

void volumeSpace::saveSpaceTofile(char * filename)
{
  ofstream myfile;
  myfile.open (filename);
  myfile << volumeResX << "\n";
  myfile << volumeResY << "\n";
  myfile << volumeResZ << "\n";
  for(int z=0; z<volumeResZ; z++)
    {
         for(int y=0; y<volumeResY; y++)
         {
             for(int x=0; x<volumeResX; x++)
             {
                 myfile << getVoxel(x,y,z) << "\n";
             }
         }
    }

  myfile.close();
  cout << "Saved a volumespace to file: " << filename << "\n";
}

void volumeSpace::setVoxel(int x, int y, int z, bool val)
{
    volume[((x+volumeResX*y)+z*volumeResX*volumeResY)]=val;
    setChunkAtPosition(x,y,z,true);
}

bool volumeSpace::getVoxel(int x, int y, int z)
{
    return volume[((x+volumeResX*y)+z*volumeResX*volumeResY)];
}

int volumeSpace::getVoxelNeighbours(int x, int y, int z)
{
    int neighbours=0;


    normal.set(0,0,0);

    for(int vx=-1; vx<2; vx++)
    {
         for(int vy=-1; vy<2; vy++)
         {
                 for(int vz=-1; vz<2; vz++)
                 {
                     if((getVoxel(x+vx,y+vy,z+vz)) && boundryCheck(x+vx,y+vy,z+vz))
                     {
                        neighbours++;

                     }
                     else
                     {
                        normal+=ofVec3f(vx,vy,vy);
                     }
                 }
         }
    }
    return neighbours;
}

bool volumeSpace::boundryCheck(int x, int y, int z)
{
    return (x>=0 && y >=0 && z>0 && x<volumeResX && y<volumeResY && z<volumeResZ-1);
}

void volumeSpace::updatePointcloud(int shader)
{

    int centerx=volumeResX/2;
    int centery=volumeResY/2;
    int centerz=volumeResZ/2;
    //pointmesh.clear();
    //pointmesh.setMode(OF_PRIMITIVE_POINTS);

    //loop through chunks
    for(int cz=0; cz<chunkDevider; cz++)
    {
         for(int cy=0; cy<chunkDevider; cy++)
         {
             for(int cx=0; cx<chunkDevider; cx++)
             {
                //if the chunk is true, loop through voxels in chunk
                if(getChunk(cx,cy,cz))
                {
                    int mesh = getMeshAtChunk(cx,cy,cz);
                    pointmeshArray[mesh].clear();
                    pointmeshArray[mesh].setMode(OF_PRIMITIVE_POINTS);
                    for(int z=0; z<volumeChunkResZ; z++)
                    {
                         for(int y=0; y<volumeChunkResY; y++)
                         {
                             for(int x=0; x<volumeChunkResX; x++)
                             {
                                int currentX=volumeChunkResX*cx+x;
                                int currentY=volumeChunkResY*cy+y;
                                int currentZ=volumeChunkResZ*cz+z;
                                if(getVoxel(currentX,currentY,currentZ))
                                {

                                    if(getVoxelNeighbours(currentX,currentY,currentZ)<26)
                                    {
                                        ofVec3f pos(currentX-centerx, currentY-centery, currentZ-centerz);

                                        //pointmeshArray[mesh].addNormal(normal);)
                                        if (shader>0) // raycasting shader
                                        {
                                            float shade = raycastcoloring(currentX,currentY,currentZ,shader);
                                            shade=shade/255;
                                            pointmeshArray[mesh].addColor(ofColor(100*shade,200*shade,200*shade));
                                        }
                                        pointmeshArray[mesh].addVertex(pos);
                                    }
                                }
                             }
                         }
                    }
                    setChunk(cx,cy,cz,false);
                }
             }
         }
    }

}


void volumeSpace::rebuildPointcloud(int shader)
{
    int chunkSize=chunkDevider*chunkDevider*chunkDevider;
    for(int i=0; i<chunkSize; i++)
    {volumeChunks[i]=true;}
    updatePointcloud(shader);
}

void volumeSpace::drawPointcloud()
{

    drawPointcloud(0,0,0);
}

void volumeSpace::drawPointcloud(int x, int y, int z)
{
    drawPointcloud(x,y,z,ofColor( 50, 100, 100));
}

void volumeSpace::drawPointcloud(int x, int y, int z, ofColor col)
{
    ofPushMatrix();
    ofTranslate(x,y,z);
    ofRotateX(rotation.x);
    ofRotateY(rotation.y);
    ofRotateZ(rotation.z);
    ofSetColor(col);
    //ofEnableLighting();
    for(int cz=0; cz<chunkDevider; cz++)
    {
         for(int cy=0; cy<chunkDevider; cy++)
         {
             for(int cx=0; cx<chunkDevider; cx++)
             {

                //if the chunk is true, draw the mesh
                ofSetColor(col);
                if(getChunk(cx,cy,cz))
                {
                        ofSetColor(150,255,150);
                }
                //pointmeshArray[((cx+chunkDevider*cy)+cz*chunkDevider*chunkDevider)].draw();
                int mesh = getMeshAtChunk(cx,cy,cz);
                pointmeshArray[mesh].draw();
             }
         }
    }

    //ofDisableLighting();
    ofNoFill();
    ofSetColor(70,90,70);
    ofTranslate(-0.5,-0.5,-0.5);
    ofScale(volumeResX,volumeResY,volumeResZ);
    ofBox(0, 0, 0, 1);
    ofPopMatrix();
}


bool volumeSpace::getChunk(int x, int y, int z)
{
    return volumeChunks[((x+chunkDevider*y)+z*chunkDevider*chunkDevider)];
}

void volumeSpace::setChunk(int x, int y, int z, bool val)
{
    volumeChunks[((x+chunkDevider*y)+z*chunkDevider*chunkDevider)]=val;
}

void volumeSpace::setChunkAtPosition(int x, int y, int z, bool val)
{
    int chunkX=int(floor(x/volumeChunkResX));
    int chunkY=int(floor(y/volumeChunkResY));
    int chunkZ=int(floor(z/volumeChunkResZ));

    if (!getChunk(chunkX,chunkY,chunkZ))
    {
        setChunk(chunkX,chunkY,chunkZ,val);
        //cout <<chunkX << "," <<chunkY << "," <<chunkZ << "\n";
    }

    //cout << chunkX << ", " << chunkY << ", " << chunkZ << ", " << "\n";
}

bool volumeSpace::getChunkAtPosition(int x, int y, int z)
{
    int chunkX=int(floor(x/volumeChunkResX));
    int chunkY=int(floor(y/volumeChunkResY));
    int chunkZ=int(floor(z/volumeChunkResZ));

    return getChunk(chunkX,chunkY,chunkZ);

}

int volumeSpace::getMeshAtChunk(int x, int y, int z)
{
    return ((x+chunkDevider*y)+z*chunkDevider*chunkDevider);
}

int volumeSpace::raycastcoloring(int x, int y, int z, int shader)
{
    int col;
    int incr;



    //if(ray(x,y,z, 1, 1, 1,0)){col+=20;}// cast a direct lighting ray
    //if(ray(x,y,z, 1, -1, 1,0)){col+=20;}



    if (shader==1) //ambient occulision based on ranom sampling(nice & grainy result but takes ages)
    {
        col=50;
        incr=3;
        for (int i=0; i<100; i++)
        {
            if(ray(x,y,z, ofRandom(-1,1),ofRandom(-1,1),ofRandom(-1,1), 75)){col+=incr;}
        }
    }


    if (shader==2) //ambient occulision based on box sampling (faster though not very nice result)
    {
        col=50;
        incr=20;
        if(ray(x,y,z, 1, 1,-1, 75)){col+=incr;}
        if(ray(x,y,z,-1, 1,-1, 75)){col+=incr;}
        if(ray(x,y,z, 1,-1,-1, 75)){col+=incr;}
        if(ray(x,y,z,-1,-1,-1, 75)){col+=incr;}

        if(ray(x,y,z, 1, 1, 1, 75)){col+=incr;}
        if(ray(x,y,z,-1, 1, 1, 75)){col+=incr;}
        if(ray(x,y,z, 1,-1, 1, 75)){col+=incr;}
        if(ray(x,y,z,-1,-1, 1, 75)){col+=incr;}

        if(ray(x,y,z, 0, 0, 1, 75)){col+=incr;}
        if(ray(x,y,z, 0, 1, 0, 75)){col+=incr;}
        if(ray(x,y,z, 1, 0, 0, 75)){col+=incr;}

        if(ray(x,y,z, 0, 0,-1, 75)){col+=incr;}
        if(ray(x,y,z, 0,-1, 0, 75)){col+=incr;}
        if(ray(x,y,z,-1, 0, 0, 75)){col+=incr;}
    }

    if (shader==3) //ambient occulision based on box sampling (faster though not very nice result)
    {
        col=50;
        incr=15;
        if(ray(x,y,z, 1, 1,-1, 75)){col+=incr;}
        if(ray(x,y,z,-1, 1,-1, 75)){col+=incr;}
        if(ray(x,y,z, 1,-1,-1, 75)){col+=incr;}
        if(ray(x,y,z,-1,-1,-1, 75)){col+=incr;}

        if(ray(x,y,z, 1, 1, 1, 75)){col+=incr;}
        if(ray(x,y,z,-1, 1, 1, 75)){col+=incr;}
        if(ray(x,y,z, 1,-1, 1, 75)){col+=incr;}
        if(ray(x,y,z,-1,-1, 1, 75)){col+=incr;}

        if(ray(x,y,z, 0, 0, 1, 75)){col+=incr;}
        if(ray(x,y,z, 0, 1, 0, 75)){col+=incr;}
        if(ray(x,y,z, 1, 0, 0, 75)){col+=incr;}

        if(ray(x,y,z, 0, 0,-1, 75)){col+=incr;}
        if(ray(x,y,z, 0,-1, 0, 75)){col+=incr;}
        if(ray(x,y,z,-1, 0, 0, 75)){col+=incr;}

        if(ray(x,y,z, 0,-1,-1, 75)){col+=incr;}
        if(ray(x,y,z,-1,-1, 0, 75)){col+=incr;}
        if(ray(x,y,z,-1, 0,-1, 75)){col+=incr;}

        if(ray(x,y,z, 0, 1, 1, 75)){col+=incr;}
        if(ray(x,y,z, 1, 1, 0, 75)){col+=incr;}
        if(ray(x,y,z, 1, 0, 1, 75)){col+=incr;}
    }



    if (col > 255)
    {
        col = 255;
    }
    return col;
}

bool volumeSpace::ray(int x0, int y0, int z0, float x1, float y1, float z1, int length)
{

    ofVec3f begin = ofVec3f(x0,y0,z0);
    //ofVec3f eind = ofVec3f(x1,y1,z1);
    //ofVec3f verschil = eind-begin;
    //ofVec3f verschil = ofVec3f(ofRandom(-1,1),ofRandom(-1,1),ofRandom(-1,1));
    ofVec3f verschil = ofVec3f(x1,y1,z1);
    verschil.normalize();

    float x = x0;
    float y = y0;
    float z = z0;

    //cout << x << ","<< y << ","<< z << "\n";
    //cout << eind.x << ","<< eind.y << ","<< eind.z << "\n";
    //cout << verschil.x << ","<< verschil.y << ","<< verschil.z << "\n";
    //cout << "-> \n";
    int steps = 0;

    //start by incrementing 1 step
    x+=verschil.x;
    y+=verschil.y;
    z+=verschil.z;

    //step through voxels in ray
    while(true)
    {

        //if (getChunkAtPosition(x,y,z)) // if the chunk is true step through voxels in chunk
        //{
            //increment (skip every other voxel, because what the heck, and the results are practicly the same)
            x+=verschil.x;
            y+=verschil.y;
            z+=verschil.z;
            steps++;
        //}
        //else
        //{
            // else jump to next chunk
        //    x+=verschil.x*volumeChunkResX;
       //     y+=verschil.y*volumeChunkResY;
       //     z+=verschil.z*volumeChunkResZ;
       //     steps+=volumeChunkResX;
        //}

        if ((length>0)&& (steps>length)) // limit the depth of the rays (for ambient occulision)
        {
            return true;
        }

        //cout << x << ","<< y << ","<< z << "\n";
        if (x>=volumeResX || y>=volumeResX || z>=volumeResX || x<=0 || y<=0 || z<=0) // if the ray walks out of the volumespace quit
        {
            return true;
        }

        if (getVoxel((int)x,(int)y,(int)z)) // if the ray hits a 'true' voxel quit and return false
        {
            return false;
        }

    }
    //cout << "O";
    return true;

}
