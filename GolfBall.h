#pragma once
#include <vector>
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
                /*
                q[0] = vx, velocity
                q[1] = x position
                q[2] = vy, velocity
                q[3] = y position
                q[4] = vz, velocity
                q[5] = z position
                */
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
    double CalculateImpactTime(double aTime1, double aTime2, double aHeight1, double aHeight2);
    void FireProjectile(Vector4d aSwingInput, Environment* pEnviron);
    void LandProjectile(Environment* pEnviron);
    void LaunchProjectile();
    void PrintFlightData();
    void PrintLandingData(Vector4d aLandingData, double aMaxY);
    void PrepProjectileLaunch(Vector4d aSwingInput);
    void ProjectileRightHandSide(struct SpinProjectile *projectile,
        double *q, double *deltaQ, double ds,
        double qScale, double *dq);
    void ProjectileRungeKutta4(struct SpinProjectile *projectile, double aDs);   
    void SetDefaultBallValues(Environment* pEnviron);
    void UpdateSpinRate(double aTimeDelta);

private:
    SpinProjectile m_ball;
    const double m_faceRoll = 0.7142857142857143; // <== 5/7, represents the ball moving up the club face to impart spin
    const double m_spinRateDecay = 0.04; // Rate at which the spinrate slows over time, using value from Trackman launch monitors of 4% per second
    float m_timeStep;
    
    std::vector<double> m_xVals;
    std::vector<double> m_yVals;
};