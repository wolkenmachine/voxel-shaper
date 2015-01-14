#ifndef _THREADED_OBJECT
#define _THREADED_OBJECT

#include "ofMain.h"
#include "volumeSpace.h"


class threadedObject : public ofThread{

	public:

        volumeSpace calcspace;
        bool isdrawing;
        bool rebuild;

		//--------------------------
		threadedObject(){
		    isdrawing=false;
		    rebuild=false;
		}

		void start(){
            startThread(true, false);   // blocking, verbose
        }

        void stop(){
            stopThread();
        }

		//--------------------------
		void threadedFunction(){

			while( isThreadRunning() != 0 ){


                        if (isdrawing==true)
                        {
                            calcspace.updatePointcloud(2);
                        }
                        else
                        {   if (rebuild==true)
                            {
                                calcspace.rebuildPointcloud(1);
                                rebuild=false;
                            }

                        }

                        //calcspace.rebuildPointcloud(1);
			}
		}

		void import(volumeSpace input)
		{
		    calcspace = input;
		}

};

#endif
