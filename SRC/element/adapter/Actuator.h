/* ****************************************************************** **
**    OpenSees - Open System for Earthquake Engineering Simulation    **
**          Pacific Earthquake Engineering Research Center            **
**                                                                    **
**                                                                    **
** (C) Copyright 1999, The Regents of the University of California    **
** All Rights Reserved.                                               **
**                                                                    **
** Commercial use of this program without express permission of the   **
** University of California, Berkeley, is strictly prohibited.  See   **
** file 'COPYRIGHT'  in main directory for information on usage and   **
** redistribution,  and for a DISCLAIMER OF ALL WARRANTIES.           **
**                                                                    **
** Developed by:                                                      **
**   Frank McKenna (fmckenna@ce.berkeley.edu)                         **
**   Gregory L. Fenves (fenves@ce.berkeley.edu)                       **
**   Filip C. Filippou (filippou@ce.berkeley.edu)                     **
**                                                                    **
** ****************************************************************** */

#ifndef Actuator_h
#define Actuator_h

// Written: Andreas Schellenberg (andreas.schellenberg@gmail.com)
// Created: 09/07
// Revision: A
//
// Description: This file contains the class definition for Actuator.
// Actuator is a truss element defined by two nodes. It receives the 
// displacement command and reports back the actuator force over a
// persistent tcp/ip channel.
// This Actuator element will work in 1d, 2d or 3d problems.

#include <Element.h>
#include <Matrix.h>

#define RemoteTest_open              1
#define RemoteTest_setup             2
#define RemoteTest_setTrialResponse  3
#define RemoteTest_execute           4
#define RemoteTest_commitState       5
#define RemoteTest_getDaqResponse    6
#define RemoteTest_getDisp           7
#define RemoteTest_getVel            8
#define RemoteTest_getAccel          9
#define RemoteTest_getForce         10
#define RemoteTest_getTime          11
#define RemoteTest_getInitialStiff  12
#define RemoteTest_getTangentStiff  13
#define RemoteTest_getDamp          14
#define RemoteTest_getMass          15
#define RemoteTest_DIE              99

class Channel;


class Actuator : public Element
{
public:
    // constructors
    Actuator(int tag, int dim, int Nd1, int Nd2,
        double EA, int ipPort, int addRayleigh = 0, double rho = 0.0);
    Actuator();
    
    // destructor
    ~Actuator();
    
    // method to get class type
    const char *getClassType() const {return "Actuator";};
    
    // public methods to obtain information about dof & connectivity
    int getNumExternalNodes() const;
    const ID &getExternalNodes();
    Node **getNodePtrs();
    int getNumDOF();
    void setDomain(Domain *theDomain);
    
    // public methods to set the state of the element
    int commitState();
    int revertToLastCommit();
    int revertToStart();
    int update();
    
    // public methods to set and to obtain stiffness,
    // and to obtain mass, damping and residual information
    const Matrix &getTangentStiff();
    const Matrix &getInitialStiff();
    const Matrix &getDamp();
    const Matrix &getMass();
    
    void zeroLoad();
    int addLoad(ElementalLoad *theLoad, double loadFactor);
    int addInertiaLoadToUnbalance(const Vector &accel);
    
    const Vector &getResistingForce();
    const Vector &getResistingForceIncInertia();
    
    // public methods for element output
    int sendSelf(int commitTag, Channel &sChannel);
    int recvSelf(int commitTag, Channel &rChannel, FEM_ObjectBroker &theBroker);
    int displaySelf(Renderer &theViewer, int displayMode, float fact, const char **modes, int numMode);

    void Print(OPS_Stream &s, int flag = 0);
    
    // public methods for element recorder
    Response *setResponse(const char **argv, int argc, OPS_Stream &s);
    int getResponse(int responseID, Information &eleInformation);
    
protected:

private:
    // private attributes - a copy for each object of the class
    int numDIM;                     // actuator in 1d, 2d or 3d domain
    int numDOF;                     // number of dof for actuator
    ID  connectedExternalNodes;     // contains the tags of the end nodes
    
    double EA;          // section stiffness of actuator
    int ipPort;         // ipPort
    int addRayleigh;    // flag to add Rayleigh damping
    double rho;         // rho: mass per unit length
    double L;           // undeformed actuator length
    double tPast;       // past time
    double cosX[3];     // direction cosines for transformation
    
    Matrix *theMatrix;  // pointer to objects matrix (a class wide Matrix)
    Vector *theVector;  // pointer to objects vector (a class wide Vector)
    Vector *theLoad;    // pointer to the load vector
    
    Vector db;  // deformation in basic system
    Vector q;   // forces in basic system
    
    Channel *theChannel;    // channel
    double *rData;          // receive data array
    Vector *recvData;       // receive vector
    double *sData;          // send data array
    Vector *sendData;       // send vector
    
    Vector *ctrlDisp;   // ctrl displacements in basic system
    Vector *ctrlForce;  // ctrl forces in basic system
    Vector *daqDisp;    // daq displacements in basic system
    Vector *daqForce;   // daq forces in basic system
    
    Node *theNodes[2];
    
    // static data - single copy for all objects of the class
    static Matrix ActuatorM2;   // class wide matrix for 2*2
    static Matrix ActuatorM4;   // class wide matrix for 4*4
    static Matrix ActuatorM6;   // class wide matrix for 6*6
    static Matrix ActuatorM12;  // class wide matrix for 12*12
    static Vector ActuatorV2;   // class wide Vector for size 2
    static Vector ActuatorV4;   // class wide Vector for size 4
    static Vector ActuatorV6;   // class wide Vector for size 6
    static Vector ActuatorV12;  // class wide Vector for size 12
    
    int setupConnection();
};

#endif
