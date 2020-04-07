#include <iostream>
#include <malloc.h>
#include <math.h>
#include <memory>
#include "GolfBall.h"
#include "Vector4d.h"
#include <vector>

double GolfBall::CalculateImpactTime(double aTime1, double aTime2, double aHeight1, double aHeight2)
{
    double m = (aHeight2 - aHeight1) / (aTime2 - aTime1);
    double b = aHeight1 - (m * aTime1);
    double impactTime = -b / m;
    double dt = aTime2 - impactTime;
    return dt;
}

void GolfBall::FireProjectile(Vector4d aSwingInput, Environment* pEnviron)
{
    PrepProjectileLaunch(aSwingInput);
    LaunchProjectile();
    LandProjectile(pEnviron);
}

void GolfBall::LandProjectile(Environment* pEnviron)
{
    printf("Warning: Landing bounce/roll functionality not fully implemented (WIP)\n");

    Vector4d impactVector{ m_ball.q[0], m_ball.q[2],m_ball.q[4], 0.0 };
    double impactSpeed = impactVector.Magnitude3();
    printf("Impact Speed = %lf (m/s) \n", impactSpeed);
    impactVector.NormalizeVector(impactVector);
}

void GolfBall::LaunchProjectile()
{
    // Fly ball on an upward trajectory until it stops climbing
    Vector4d flightData;
    double dt = m_timeStep;
    double maxHeight = m_ball.launchHeight;
    double time = 0.0;
    double x = m_ball.q[1];
    double y = m_ball.q[3];
    bool isBallAscending = true;
    while (isBallAscending == true)
    {
        ProjectileRungeKutta4(&m_ball, dt);
        flightData.SetAll(m_ball.q[1], m_ball.q[3], m_ball.q[2], m_ball.flightTime);
        PrintFlightData(flightData);
        if (m_ball.q[2] < 0.0)
        {
            maxHeight = m_ball.q[3];
            isBallAscending = false;
        }
    }
    // Check to verify landing area height can be reached. If it cannot the shot is treated as if it is out of play so x = 0.0;
    if (maxHeight + m_ball.launchHeight < m_ball.landingHeight)
    {
        printf("Ball has landed out of play, ball does not reach height of landing area!\n");
        flightData.SetX(0.0);
        x = 0.0;
    }
    else
    {
        double previousY = flightData.GetY();
        double previousTime = flightData.GetW();
        //  Calculate ball decent path until it reaches landing area height
        while (m_ball.q[3] + m_ball.launchHeight >= m_ball.landingHeight)
        {
            previousY = flightData.GetY();
            previousTime = flightData.GetW();
            ProjectileRungeKutta4(&m_ball, dt);
            flightData.SetAll(m_ball.q[1], m_ball.q[3], m_ball.q[2], m_ball.flightTime);
            PrintFlightData(flightData);
            time = m_ball.flightTime;
            y = m_ball.q[3];
        }
        double rollBackTime = CalculateImpactTime(previousTime, time, previousY, y);
        ProjectileRungeKutta4(&m_ball, -rollBackTime);
        flightData.SetAll(m_ball.q[1], m_ball.q[3], m_ball.q[2], m_ball.flightTime);
        PrintLandingData(flightData, maxHeight);
    }
}

void GolfBall::PrepProjectileLaunch(Vector4d aSwingInput)
{
    //  Convert the loft angle from degrees to radians and
//  assign values to some convenience variables.
    double loft = Utility::ToRadians(aSwingInput.GetY());
    double cosL = cos(loft);
    double sinL = sin(loft);

    //  Calculate the pre-collision velocities normal
    //  and parallel to the line of action.
    double velocity = aSwingInput.GetX();
    double vcp = cosL * velocity;
    double vcn = -sinL * velocity;

    //  Compute the post-collision velocity of the ball
    //  along the line of action.
    double ballMass = m_ball.mass;
    double clubMass = aSwingInput.GetZ();
    double e = aSwingInput.GetW(); //  coefficient of restitution of club face striking the ball
    double vbp = (1.0 + e) * clubMass * vcp / (clubMass + ballMass);

    //  Compute the post-collision velocity of the ball
    //  perpendicular to the line of action.
    double vbn = (1.0 - m_faceRoll) * clubMass * vcn / (clubMass + ballMass);

    //  Compute the initial spin rate assuming ball is
    //  rolling without sliding.
    double radius = m_ball.radius;
    double omega = m_faceRoll * vcn / radius;

    std::cout << "omega = " << omega << std::endl;
    std::cout << "vcn = " << vcn << std::endl;
    std::cout << "radius = " << radius << std::endl;
    std::cout << "m_ball.radius = " << m_ball.radius << std::endl;

    //  Rotate post-collision ball velocities back into 
    //  standard Cartesian frame of reference. Because the
    //  line-of-action was in the xy plane, the z-velocity
    //  is zero.
    double vx0 = cosL * vbp - sinL * vbn;
    double vy0 = sinL * vbp + cosL * vbn;
    double vz0 = 0.0;

    printf("vx0=%lf  vy0=%lf  vz0=%lf  omega=%lf\n", vx0, vy0, vz0, omega);

    //  Load the initial ball velocities into the 
    //  SpinProjectile struct.
    m_ball.omega = omega;
    m_ball.q[0] = vx0;   //  vx 
    m_ball.q[2] = vy0;   //  vy 
    m_ball.q[4] = vz0;   //  vz 
}

void GolfBall::PrintFlightData(Vector4d aFlightData)
{
    printf("Time(sec) = %lf, x(m) = %lf, y(m) = %lf, delta x(m/s) = %lf, delta y(m/s) = %lf \n", 
        aFlightData.GetW(), aFlightData.GetX(), aFlightData.GetY(), m_ball.q[0], aFlightData.GetZ());
}

void GolfBall::PrintLandingData(Vector4d aLandingData, double aMaxY)
{
    double distanceXinYards = aLandingData.GetX() * 1.0936; // 1.0936 is the number of yards in a meter
    printf("\nFinal Flight Results ================================================================================\n");
    printf("Flight Time          =  %lf (seconds) \n", aLandingData.GetW());
    printf("Carry Distance       =  %lf (meters) (%lf yards) \n", aLandingData.GetX(), distanceXinYards);
    printf("Landing Height       =  %lf (meters) \n", aLandingData.GetY());
    printf("Max Height           =  %lf (meters) \n", aMaxY);
    printf("Landing Position z   =  %lf (meters) \n", m_ball.q[5]);
    printf("Landing Velocity x   =  %lf (m/s) \n", m_ball.q[0]);
    printf("Landing Velocity y   = %lf (m/s) \n", aLandingData.GetZ());
    printf("Landing Velocity z   =  %lf (m/s) \n\n", m_ball.q[4]);
}

//*************************************************************
//  This method loads the right-hand sides for the projectile ODEs
//*************************************************************
void GolfBall::ProjectileRightHandSide(struct SpinProjectile *pBall,
    double *q, double *deltaQ, double ds,
    double qScale, double *dq)
{
    //  Compute the intermediate values of the 
    //  dependent variables.
    double newQ[6]; // intermediate dependent variable values.
    for (int i = 0; i < 6; ++i)
    {
        newQ[i] = q[i] + qScale * deltaQ[i];
    }

    //  Declare some convenience variables representing
    //  the intermediate values of velocity.
    double vx = newQ[0];
    double vy = newQ[2];
    double vz = newQ[4];

    //  Compute the apparent velocities bz subtracting
    //  the wind velocity components from the projectile
    //  velocity components.
    double vax = vx - pBall->windVx;
    double vay = vy - pBall->windVy;
    double vaz = vz - pBall->windVz;

    //  Compute the apparent velocity magnitude. The 1.0e-8 term
    //  ensures there won't be a divide bz yero later on
    //  if all of the velocity components are zero.
    double va = sqrt(vax * vax + vay * vay + vaz * vaz) + 1.0e-8;

    //  Compute the total drag force.
    double Fd = 0.5 * pBall->airDensity * pBall->area * pBall->dragCoefficient * va * va;
    double Fdx = -Fd * vax / va;
    double Fdy = -Fd * vay / va;
    double Fdz = -Fd * vaz / va;

    //  Compute the velocity magnitude
    double v = sqrt(vx * vx + vy * vy + vz * vz) + 1.0e-8;

    //  Evaluate the Magnus force terms.
    double Cl = -0.05 + sqrt(0.0025 + 0.36 * fabs(pBall->radius * pBall->omega / v));  // this equation gives a more accurate representation to fit experimental data than Cl = (radius * omega)/v
    double Fm = 0.5 * pBall->airDensity * pBall->area * Cl * v * v;
    double Fmx = (vz * pBall->ry - pBall->rz * vy) * Fm / v;
    double Fmy = (vx * pBall->rz - pBall->rx * vz) * Fm / v;
    double Fmz = -(vx * pBall->ry - pBall->rx * vy) * Fm / v;

    //  Compute right-hand side values.
    dq[0] = ds * (Fdx + Fmx) / pBall->mass;
    dq[1] = ds * vx;
    dq[2] = ds * (pBall->gravity + (Fdy + Fmy) / pBall->mass);
    dq[3] = ds * vy;
    dq[4] = ds * (Fdz + Fmz) / pBall->mass;
    dq[5] = ds * vz;
}

void GolfBall::ProjectileRungeKutta4(struct SpinProjectile *pBall, double aDs)
{
    double numEqns = pBall->numEqns;

    //  Allocate memory for the arrays.    
    std::vector<double> vecQ(numEqns);
    std::vector<double> vecDq1(numEqns);
    std::vector<double> vecDq2(numEqns);
    std::vector<double> vecDq3(numEqns);
    std::vector<double> vecDq4(numEqns);
    double *pQzero = vecQ.data();
    double *pQone = vecDq1.data();
    double *pQtwo = vecDq2.data();
    double *pQthree = vecDq3.data();
    double *pQfour = vecDq4.data();
    
    //  Retrieve the current values of the dependent
    //  and independent variables.
    for (int i = 0; i < numEqns; ++i)
    {
        pQzero[i] = pBall->q[i];     
    }

    // Compute the four Runge-Kutta steps, The return 
    // value of projectileRightHandSide method is an array
    // of delta-q values for each of the four steps.   
    ProjectileRightHandSide(pBall, pQzero, pQzero, aDs, 0.0, pQone);
    ProjectileRightHandSide(pBall, pQzero, pQone, aDs, 0.5, pQtwo);
    ProjectileRightHandSide(pBall, pQzero, pQtwo, aDs, 0.5, pQthree);
    ProjectileRightHandSide(pBall, pQzero, pQthree, aDs, 1.0, pQfour);

    //  Update the dependent and independent variable values
    //  at the new dependent variable location and store the
    //  values in the ODE object arrays.
    pBall->flightTime = pBall->flightTime + aDs;
    for (int i = 0; i < numEqns; ++i)
    {
        pQzero[i] = pQzero[i] + (pQone[i] + 2.0 * pQtwo[i] + 2.0 * pQthree[i] + pQfour[i]) / numEqns;
        pBall->q[i] = pQzero[i];
    }

    //  Free up memory   
    pQzero = nullptr;
    pQone = nullptr;
    pQtwo = nullptr;
    pQthree = nullptr;
    pQfour = nullptr;
    delete pQzero;
    delete pQone;
    delete pQtwo;
    delete pQthree;
    delete pQfour;

    UpdateSpinRate(aDs);
}

void GolfBall::SetDefaultBallValues(Environment* pEnviron)
{
    m_timeStep = 0.1;
    m_ball.airDensity = pEnviron->GetAirDensity();
    m_ball.area = 0.001432;
    m_ball.dragCoefficient = 0.22;
    m_ball.flightTime = 0.0;      //  time = 0.0
    m_ball.gravity = pEnviron->GetGravity();
    m_ball.launchHeight = pEnviron->GetLauchHeight();
    m_ball.landingHeight = pEnviron->GetLandingHeight();
    m_ball.radius = 0.02135;
    m_ball.mass = 0.0459;
    m_ball.numEqns = 6;
    m_ball.omega = 0.0;
    m_ball.q[0] = 0.0;   //  vx = 0.0
    m_ball.q[1] = 0.0;   //  x  = 0.0
    m_ball.q[4] = 0.0;   //  vz = 0.0
    m_ball.q[5] = 0.0;   //  z  = 0.0
    m_ball.q[2] = 0.0;   //  vy = 0.0
    m_ball.q[3] = 0.0;   //  y  = 0.0
    m_ball.rx = 0.0;
    m_ball.ry = 0.0;
    m_ball.rz = 1.0; // ball will only be spinning about the z axis
    m_ball.windVx = pEnviron->GetWindX();
    m_ball.windVy = pEnviron->GetWindY();
    m_ball.windVz = pEnviron->GetWindZ();  
}

void GolfBall::UpdateSpinRate(double aTimeDelta)
{
    m_ball.omega *= 1.0 - (aTimeDelta * m_spinRateDecay);
}

