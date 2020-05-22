#pragma once
#include "Environment.h"
#include "GolfBag.h"
#include "Utility.h"
#include "Vector4d.h"


class GolfSwing 
{
public:
    GolfSwing();
    ~GolfSwing();

    Vector4d CalculateLaunchVector(void);
    double ComputeAlphaDotDot(void);
    double ComputeBetaDotDot(void);
    const double GetBackSwingPercentage() { return m_backSwingPercentage; };   
    void InputSwingValuesBasic();
    void InputSwingValuesVerbose();
    void PrintSwingInputData();
    void PrintSwingMechanics(const double aClubVelocity, const double aTime);
    void ReadInSwingValues();
    void SetArmLength(double aLength);
    void SetBackSwingPercentage(double aPercentage);
    void SetBallPlacementAngle(double aAngle);
    void SetBeta(double aBeta);
    void SetClubAngle(double aAngle);
    void SetClubCoR(double aCoR);
    void SetClubLength(double aLength);
    void SetClubMass(double aMass);
    void SetDefaultSwingValues(double aGravity);
    void SetQalpha(double aQalpha);
    void SetQbeta(double aQbeta);
    void SetShoulderAccel(double aShoulderAccel); 
    void UpdateGolfSwingValues();  

    void SelectClub();
    void UpdateClubData();

private:
    GolfBag* m_pBag;
    GolfClub m_club;

    double m_alpha; // Angle swept by arm rod from initial backswing position in radians
    double m_alpha_dot;
    double m_alpha_dotdot;
    double m_armBalancePoint;
    double m_armFirstMoment; // First moment of the arm rod about the shoulder axis kg m
    double m_armLength; // Length arm rod in m
    double m_armMass;
    double m_armMassMoI; // Mass moment of inertia of the rod representing the arm in kg m^2
    double m_backSwingPercentage;
    double m_ballPlacementAngle;
    double m_beta; // Wrist cock angle in radians 120.0 in radians
    double m_beta_dot;
    double m_beta_dotdot;
    double m_clubAngle;
    double m_clubBalancePoint;
    double m_clubCoR; // club face coefficiant of restitution, aka club spring face, current USGA rules limit this to .830 in tournemnt play
    double m_clubFirstMoment; // First moment of the rod representing the club about the wrist axis (where the club rod connects to the arm rod) in kg m
    double m_clubLength; // length of club in m
    double m_clubMass;
    double m_clubMassMoI; // Mass moment of inertia of the rod representing the club in kg m^2
    double m_gamma; // 135.0 degrees in radians
    double m_gravity; // Gravity's acceleration m/s^2
    double m_Qalpha; // Torque applied at the shoulder to the arm rod in N m
    double m_Qbeta; // Torque applied at the wrist joint to the club rod in N m   
    double m_shoulderHorizAccel; // Horizontal acceleration of the shoulder in  m/s^2
    double m_theta;// = m_gamma - m_alpha, Angle between arm rod and vertical axis in radians

    // min, max, and default consts
    const double m_defaultGravity = 9.8;
    const double m_minArmLength = 0.1;
    const double m_maxArmLength = 5.0;
    const double m_minBackSwingPercentage = 0.1;
    const double m_maxBackSwingPercentage = 110.0; // not 100.0 because sometimes you just gotta swing to hard
    const double m_minBallPlacementAngle = -10.0;
    const double m_maxBallPlacementAngle = 10.0;
    const double m_minBeta = 0.0;
    const double m_maxBeta = 180.0;
    const double m_minClubAngle = 0.0;
    const double m_maxClubAngle = 80.0;
    const double m_minClubCoR = 0.1;
    const double m_maxClubCoR = 1.0;
    const double m_minClubLength = 0.1;
    const double m_maxClubLength = 5.0;
    const double m_minClubMass = 0.0;
    const double m_maxClubMass = 10.0;
    const double m_minShoulderAccel = 0.00001;
    const double m_maxShoulderAccel = 10.0;
    const double m_minQalpha = 1.0;
    const double m_maxQalpha = 100.0;
    const double m_minQbeta = -0.01;
    const double m_maxQbeta = 100;
};