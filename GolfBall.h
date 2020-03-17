#pragma once
#include "Environment.h"
#include "Utility.h"
#include "Vector4d.h"

struct SpinProjectile
{
    double airDensity;
    double area;
    double dragCoefficient;
    double flightTime;
    double gravity;
    double launchHeight;
    double landingHeight;
    double mass;
    int numEqns;  // number of equations to solve
    double omega;  //  angular velocity, m/s
    double q[6];
    double rx;     //  spin axis vector component
    double ry;     //  spin axis vector component
    double rz;     //  spin axis vector component
    double radius; //  sphere radius, m
    double windVx;
    double windVy;
    double windVz;
};

class GolfBall
{
public:
    Vector4d CalculateProjectileData(Vector4d aSwingInput);
    double CalculateImpactTime(double aTime1, double aTime2, double aHeight1, double aHeight2);
    void FireProjectile(Vector4d aSwingInput, Environment* pEnviron);
    void LandProjectile(Environment* pEnviron);
    void LaunchProjectile();
    void PrintFlightData(Vector4d aFlightData);
    void PrintLandingData(Vector4d aLandingData, double aMaxY);
    void PrepProjectileLaunch(Vector4d aSwingInput);
    void ProjectileRightHandSide(struct SpinProjectile *projectile,
        double *q, double *deltaQ, double ds,
        double qScale, double *dq);
    void ProjectileRungeKutta4(struct SpinProjectile *projectile, double aDs);   
    void SetDefaultBallValues(Environment* pEnviron);
private:
    SpinProjectile m_ball;

    double m_timeStep;
};