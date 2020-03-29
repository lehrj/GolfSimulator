#include "GolfSwing.h"
#include <math.h>
#include "Vector4d.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

GolfSwing::GolfSwing()
{
    //this->SetDefaultSwingValues(9.8);
}

GolfSwing::~GolfSwing()
{
}

Vector4d GolfSwing::CalculateLaunchVector(void)
{
    double Vc = 0.0;
    double time = 0.0;
    double dt = 0.0025; // Time delta between frames in seconds
    double    a, at; // stores previous time steps results for alpha and its first derivative
    double    b, bt; // stores previous time steps results for beta and its first derivative

    double    phi; // stores value of theta + beta
    double    Vc2; // square of club head velocity 
    double  ak1, ak2, ak3, ak4; // stores intermediate results of Runge-Kutta integration scheme
    double  bk1, bk2, bk3, bk4; // stores intermediate results of Runge-Kutta integration scheme
    double velocityCapture = 0;
    double launchAngle = 0.0;
    bool isVcFound = false;

    std::cout << " Initial values \n";
    std::cout << " Time                                                               = " << time << std::endl;
    std::cout << " DEGREES(theta) Angel between arm rod and vertical axis             = " << Utility::ToDegrees(m_theta) << std::endl;
    std::cout << " DEGREES(alpha) Arm Rod angle sweep from initial backswing position = " << Utility::ToDegrees(m_alpha) << std::endl;
    std::cout << " DEGREES(beta)  Wrist cock angle                                    = " << Utility::ToDegrees(m_beta) << std::endl;
    //std::cout << " DEGREES(phi) = theta + beta;                                       = " << DEGREES(phi) << std::endl;
    std::cout << " DEGREES(phi) = theta + beta;                                       = " << Utility::ToDegrees(m_theta + m_beta) << std::endl;
    std::cout << " Vc, club head velecity                                             = " << Vc << std::endl;
    std::cout << "End initial values \n";
    std::cout << "====================================================================\n";

    for (int i = 0; i < 200; i++)
    {
        time += dt;
        if (time >= 0.1)
        {
            m_Qbeta = 0;
        }
        // save results of previous time step
        a = m_alpha;
        b = m_beta;
        at = m_alpha_dot;
        bt = m_beta_dot;
        // integrate alpha'' and beta''

        // The K1 Step:
        m_alpha_dotdot = ComputeAlphaDotDot();
        m_beta_dotdot = ComputeBetaDotDot();
        ak1 = m_alpha_dotdot * dt;
        bk1 = m_beta_dotdot * dt;
        m_alpha_dot = at + ak1 / 2;
        m_beta_dot = bt + bk1 / 2;
        // The K2 Step:
        m_alpha_dotdot = ComputeAlphaDotDot();
        m_beta_dotdot = ComputeBetaDotDot();
        ak2 = m_alpha_dotdot * dt;
        bk2 = m_beta_dotdot * dt;
        m_alpha_dot = at + ak2 / 2;
        m_beta_dot = bt + bk2 / 2;
        // The K3 Step:
        m_alpha_dotdot = ComputeAlphaDotDot();
        m_beta_dotdot = ComputeBetaDotDot();
        ak3 = m_alpha_dotdot * dt;
        bk3 = m_beta_dotdot * dt;
        m_alpha_dot = at + ak3;
        m_beta_dot = bt + bk3;
        // The K3 Step:
        m_alpha_dotdot = ComputeAlphaDotDot();
        m_beta_dotdot = ComputeBetaDotDot();
        ak4 = m_alpha_dotdot * dt;
        bk4 = m_beta_dotdot * dt;
        m_alpha_dot = at + (ak1 + 2 * ak2 + 2 * ak3 + ak4) / 6;
        m_beta_dot = bt + (bk1 + 2 * bk2 + 2 * bk3 + bk4) / 6;
        m_alpha = a + m_alpha_dot * dt;
        m_beta = b + m_beta_dot * dt;
        m_theta = m_gamma - m_alpha;

        Vc2 = (m_armLength * m_armLength + m_clubLength * m_clubLength + 2 * m_armLength * m_clubLength * cos(m_beta))
            * (m_alpha_dot * m_alpha_dot) + m_clubLength * m_clubLength * m_beta_dot * m_beta_dot
            - 2 * (m_clubLength * m_clubLength + m_armLength * m_clubLength * cos(m_beta)) * m_alpha_dot * m_beta_dot;  // Jorgensen equation

        Vc = sqrt(Vc2);

        phi = m_theta + m_beta;
        if (Utility::ToDegrees(phi) < m_ballPlacementAngle)
        {
            if (isVcFound == false)
            {
                std::cout << " Swing Impact Results" << std::endl;
                std::cout << " Time                                                               = " << time << std::endl;
                std::cout << " DEGREES(theta) Angel between arm rod and vertical axis             = " << Utility::ToDegrees(m_theta) << std::endl;
                std::cout << " DEGREES(alpha) Arm Rod angle sweep from initial backswing position = " << Utility::ToDegrees(m_alpha) << std::endl;
                std::cout << " DEGREES(beta)  Wrist cock angle                                    = " << Utility::ToDegrees(m_beta) << std::endl;
                std::cout << " DEGREES(phi) = theta + beta;                                       = " << Utility::ToDegrees(phi) << std::endl;
                std::cout << " Vc, club head velecity                                             = " << Vc << std::endl;
                std::cout << "====================================================================\n";

                velocityCapture = Vc;
                isVcFound = true;
                launchAngle = m_clubAngle - Utility::ToDegrees(phi);
            }
        }
    }
    std::cout << "Capture velocity = " << velocityCapture << "\nLaunch angle = " << launchAngle << std::endl;

    Vector4d launchVector{ velocityCapture, launchAngle, m_clubMass, m_clubCoR };
    return launchVector;
}

double GolfSwing::ComputeAlphaDotDot(void)
{
    double A = (m_armMassMoI + m_clubMassMoI + m_clubMass * m_armLength * m_armLength + 2 * m_armLength * m_clubFirstMoment * cos(m_beta));
    double B = -(m_clubMassMoI + m_armLength * m_clubFirstMoment * cos(m_beta));
    double F = m_Qalpha - (m_beta_dot * m_beta_dot - 2 * m_alpha_dot * m_beta_dot) * m_armLength * m_clubFirstMoment * sin(m_beta) + m_clubFirstMoment 
        * (m_gravity * sin(m_theta + m_beta) - m_shoulderHorizAccel * cos(m_theta + m_beta)) + (m_armFirstMoment + m_clubMass * m_armLength) 
        * (m_gravity * sin(m_theta) - m_shoulderHorizAccel * cos(m_theta));
    double D = m_clubMassMoI;
    double G = m_Qbeta - m_alpha_dot * m_alpha_dot * m_armLength * m_clubFirstMoment * sin(m_beta) - m_clubFirstMoment 
        * (m_gravity * sin(m_theta + m_beta) - m_shoulderHorizAccel * cos(m_theta + m_beta));
    return (F - (B * G / D)) / (A - (B * B / D));
}

double GolfSwing::ComputeBetaDotDot(void)
{
    double C = -(m_clubMassMoI + m_armLength * m_clubFirstMoment * cos(m_beta));
    double D = m_clubMassMoI;
    double G = m_Qbeta - m_alpha_dot * m_alpha_dot * m_armLength * m_clubFirstMoment * sin(m_beta) - m_clubFirstMoment 
        * (m_gravity * sin(m_theta + m_beta) - m_shoulderHorizAccel * cos(m_theta + m_beta));
    return (G - C * m_alpha_dotdot) / D;
}

void GolfSwing::InputSwingValues()
{
    bool isInputValid = false;
    while (isInputValid == false)
    {
        std::cout << "Input arm length in meters between " << m_minArmLength << " and " << m_maxArmLength << ": ";
        double armLength;
        std::cin >> armLength;
        if (armLength >= m_minArmLength && armLength <= m_maxArmLength)
        {
            isInputValid = true;
            SetArmLength(armLength);
        }
        else
        {
            std::cout << "Input Error, please try again \n";
        }
    }

    isInputValid = false;
    while (isInputValid == false)
    {
        std::cout << "Input Back Swing Percentage in % between " << m_minBackSwingPercentage << " and " << m_maxBackSwingPercentage << ": ";
        double backSwingPercentage;
        std::cin >> backSwingPercentage;
        if (backSwingPercentage >= m_minBackSwingPercentage && backSwingPercentage <= m_maxBackSwingPercentage)
        {
            isInputValid = true;
            SetBackSwingPercentage(backSwingPercentage);
        }
        else
        {
            std::cout << "Input Error, please try again \n";
        }
    }

    isInputValid = false;
    while (isInputValid == false)
    {
        std::cout << "Input Ball Placement Angle in degrees between " << m_minBallPlacementAngle << " and " << m_maxBallPlacementAngle << ": ";
        double ballPlacement;
        std::cin >> ballPlacement;
        if (ballPlacement >= m_minBallPlacementAngle && ballPlacement <= m_maxBallPlacementAngle)
        {
            isInputValid = true;
            SetBallPlacementAngle(ballPlacement);
        }
        else
        {
            std::cout << "Input Error, please try again \n";
        }
    }

    isInputValid = false;
    while (isInputValid == false)
    {
        std::cout << "Input Club Angle in degress between " << m_minClubAngle << " and " << m_maxClubAngle << ": ";
        double clubAngle;
        std::cin >> clubAngle;
        if (clubAngle >= m_minClubAngle && clubAngle <= m_maxClubAngle)
        {
            isInputValid = true;
            SetClubAngle(clubAngle);
        }
        else
        {
            std::cout << "Input Error, please try again \n";
        }
    }

    isInputValid = false;
    while (isInputValid == false)
    {
        std::cout << "Input Club Coefficient of Restitution between (note: values above 0.830 are not legal in USGA play" << m_minClubCoR << " and " << m_maxClubCoR << ": ";
        double clubCoR;
        std::cin >> clubCoR;
        if (clubCoR >= m_minClubCoR && clubCoR <= m_maxClubCoR)
        {
            isInputValid = true;
            SetClubCoR(clubCoR);
        }
        else
        {
            std::cout << "Input Error, please try again \n";
        }
    }

    isInputValid = false;
    while (isInputValid == false)
    {
        std::cout << "Input Club Length in meters between " << m_minClubLength << " and " << m_maxClubLength << ": ";
        double clubLength;
        std::cin >> clubLength;
        if (clubLength >= m_minClubLength && clubLength <= m_maxClubLength)
        {
            isInputValid = true;
            SetClubLength(clubLength);
        }
        else
        {
            std::cout << "Input Error, please try again \n";
        }
    }

    isInputValid = false;
    while (isInputValid == false)
    {
        std::cout << "Input Club Mass in kg between " << m_minClubMass << " and " << m_maxClubMass << ": ";
        double clubMass;
        std::cin >> clubMass;
        if (clubMass >= m_minClubMass && clubMass <= m_maxClubMass)
        {
            isInputValid = true;
            SetClubMass(clubMass);
        }
        else
        {
            std::cout << "Input Error, please try again \n";
        }
    }

    isInputValid = false;
    while (isInputValid == false)
    {
        std::cout << "Input Horizonatal Acceleration prior to application of gravity in m/s^2 between " << m_minShoulderAccel << " and " << m_maxShoulderAccel << ": ";
        double shoulderAccel;
        std::cin >> shoulderAccel;
        if (shoulderAccel >= m_minShoulderAccel && shoulderAccel <= m_maxShoulderAccel)
        {
            isInputValid = true;
            SetShoulderAccel(shoulderAccel);
        }
        else
        {
            std::cout << "Input Error, please try again \n";
        }
    }

    isInputValid = false;
    while (isInputValid == false)
    {
        std::cout << "Input Shoulder Torque in newton meters between " << m_minQalpha << " and " << m_maxQalpha << ": ";
        double shoulderTorque;
        std::cin >> shoulderTorque;
        if (shoulderTorque >= m_minQalpha && shoulderTorque <= m_maxQalpha)
        {
            isInputValid = true;
            SetQalpha(shoulderTorque);
        }
        else
        {
            std::cout << "Input Error, please try again \n";
        }
    }

    isInputValid = false;
    while (isInputValid == false)
    {
        std::cout << "Input Wrist Cock Angle in degrees between " << m_minBeta << " and " << m_maxBeta << ": ";
        double wristCockAngle;
        std::cin >> wristCockAngle;
        if (wristCockAngle >= m_minBeta && wristCockAngle <= m_maxBeta)
        {
            isInputValid = true;
            SetBeta(Utility::ToRadians(wristCockAngle));
        }
        else
        {
            std::cout << "Input Error, please try again \n";
        }
    }

    isInputValid = false;
    while (isInputValid == false)
    {
        std::cout << "Input Wrist Torque as a positve value in newton meters between " << m_minQbeta << " and " << m_maxQbeta << ": ";
        double wristTorque;
        std::cin >> wristTorque;
        if (wristTorque >= m_minQbeta && wristTorque <= m_maxQbeta)
        {
            isInputValid = true;
            m_Qbeta = -wristTorque;
        }
        else
        {
            std::cout << "Input Error, please try again \n";
        }
    }
}

void GolfSwing::ReadInSwingValues()
{
    double armLength;
    double backSwingPercentage;
    double ballPlacement;
    double clubAngle;
    double clubCoR; //  coefficient of restitution of club face
    double clubLength;
    double clubMass;   
    double shoulderAcceleration; //= 0.1 * g; // Horizontal acceleration of the shoulder in  m/s^2
    double shoulderTorque; //= 100; // Torque applied at the shoulder to the arm rod in N m
    double wristCockAngle; //= RADIANS(120.0); // Wrist cock angle in radians
    double wristTorque; //= -10; // Torque applied at the wrist joint to the club rod in N m
    
    std::cout << "Warning: If there are any errors in SwingInputData.txt all values will be purged and default values used!\n";
    std::ifstream inFile("SwingInputData.txt");
    inFile >> armLength >> backSwingPercentage >> ballPlacement >> clubAngle >> clubCoR >> clubLength >> clubMass >> shoulderAcceleration >> shoulderTorque >> wristCockAngle >> wristTorque;
    inFile.close();

    std::vector<std::string> errorList;
    bool isInputValid = true;
    if (armLength < m_minArmLength || armLength > m_maxArmLength)
    {
        isInputValid = false;
        std::string errorString = "1 armLength";
        errorList.push_back(errorString);
    }

    if (backSwingPercentage < m_minBackSwingPercentage || backSwingPercentage > m_maxBackSwingPercentage)
    {
        isInputValid = false;
        std::string errorString = "2 backSwingPercentage";
        errorList.push_back(errorString);
    }

    if (ballPlacement < m_minBallPlacementAngle || ballPlacement > m_maxBallPlacementAngle)
    {
        isInputValid = false;
        std::string errorString = "3 ballPlacement ";
        errorList.push_back(errorString);
    }

    if (clubAngle < m_minClubAngle || clubAngle > m_maxClubAngle)
    {
        isInputValid = false;
        std::string errorString = "4 clubAngle ";
        errorList.push_back(errorString);
    }

    if (clubCoR < m_minClubCoR || clubCoR > m_maxClubCoR)
    {
        isInputValid = false;
        std::string errorString = "5 clubCoR";
        errorList.push_back(errorString);
    }

    if (clubLength < m_minClubLength || clubLength > m_maxClubLength)
    {
        isInputValid = false;
        std::string errorString = "6 clubLength ";
        errorList.push_back(errorString);
    }

    if (clubMass < m_minClubMass || clubMass > m_maxClubMass)
    {
        isInputValid = false;
        std::string errorString = "7 clubMass ";
        errorList.push_back(errorString);
    }

    if (shoulderAcceleration < m_minShoulderAccel || shoulderAcceleration > m_maxShoulderAccel)
    {
        isInputValid = false;
        std::string errorString = "8 shoulderAcceleration ";
        errorList.push_back(errorString);
    }

    if (shoulderTorque < m_minQalpha || shoulderTorque > m_maxQalpha)
    {
        isInputValid = false;
        std::string errorString = "9 shoulderTorque ";
        errorList.push_back(errorString);
    }

    if (wristCockAngle < m_minBeta || wristCockAngle > m_maxBeta)
    {
        isInputValid = false;
        std::string errorString = "10 wristCockAngle ";
        errorList.push_back(errorString);
    }

    if (wristTorque < m_minQbeta || wristTorque > m_maxQbeta)
    {
        isInputValid = false;
        std::string errorString = "11 wristTorque ";
        errorList.push_back(errorString);
    }

    if (isInputValid == false)
    {
        std::cout << "Error in SwingInputData.txt, data invalid\n";
        std::cerr << "Error in SwingInputData.txt, data invalid\n";
        for (unsigned int i = 0; i < errorList.size(); i++)
        {
            std::cout << "Error in line : " << errorList[i] << "\n";
            std::cerr << "Error in line : " << errorList[i] << "\n";
        }
        std::cout << "Reverting to default Swing data \n";
        SetDefaultSwingValues(m_defaultGravity);
    }
    else
    {
        SetArmLength(armLength);
        SetBackSwingPercentage(backSwingPercentage);
        SetBallPlacementAngle(ballPlacement);
        SetClubAngle(clubAngle);
        SetClubCoR(clubCoR);
        SetClubLength(clubLength);
        SetClubMass(clubMass);
        SetShoulderAccel(shoulderAcceleration);
        SetQalpha(shoulderTorque);
        SetBeta(Utility::ToRadians(wristCockAngle));
        SetQbeta(wristTorque);
    }
}

void GolfSwing::SetArmLength(double aLength)
{
    m_armLength = aLength;
}

void GolfSwing::SetBackSwingPercentage(double aPercentage)
{
    m_backSwingPercentage = aPercentage;
}

void GolfSwing::SetBallPlacementAngle(double aAngle)
{
    m_ballPlacementAngle = aAngle;
}

void GolfSwing::SetBeta(double aBeta)
{
    m_beta = aBeta;
}

void GolfSwing::SetClubAngle(double aAngle)
{
    m_clubAngle = aAngle;
}

void GolfSwing::SetClubCoR(double aCoR)
{
    m_clubCoR = aCoR;
}

void GolfSwing::SetClubLength(double aLength)
{
    m_clubLength = aLength;
}

void GolfSwing::SetClubMass(double aMass)
{
    m_clubMass = aMass;
}

void GolfSwing::SetDefaultSwingValues(double aGravity)
{
    if (aGravity < 0.0) // ensuring that we make gravity pointed down since golf doesn't work if its positive
    {
        m_gravity = -aGravity; // Gravity's acceleration inverted  because the downswing is working in gravity's direction m/s^2 
    }
    else
    {
        m_gravity = aGravity;
    }

    // Input Variables
    m_alpha = 0.0; // Angle swept by arm rod from initial backswing position in radians
    m_alpha_dot = 0.0;
    m_alpha_dotdot = 0.0;
    m_armBalancePoint = 0.5;
    m_armLength = 0.62;
    m_armMass = 7.3;
    m_armMassMoI = 1.15; // Mass moment of inertia of the rod representing the arm in kg m^2
    m_backSwingPercentage = 100.0;
    m_ballPlacementAngle = 5.0;
    m_beta = Utility::ToRadians(120.0); // Wrist cock angle in radians
    m_beta_dot = 0.0;
    m_beta_dotdot = 0.0;
    m_clubAngle = 25.0;
    m_clubBalancePoint = 0.75;
    m_clubCoR = 0.78; // club face coefficiant of restitution, aka club spring face, current USGA rules limit this to .830 in tournemnt play
    m_clubLength = 1.1; // length of club in m
    m_clubMass = 0.4;
    m_clubMassMoI = 0.08; // Mass moment of inertia of the rod representing the club in kg m^2
    m_Qalpha = 100; // Torque applied at the shoulder to the arm rod in N m
    m_Qbeta = -10; // Torque applied at the wrist joint to the club rod in N m

    // dependant variables 
    m_armFirstMoment = (m_armMass * m_armLength * m_armBalancePoint); // First moment of the arm rod about the shoulder axis kg m
    m_clubFirstMoment = (m_clubMass * m_clubLength * m_clubBalancePoint); // First moment of the rod representing the club about the wrist axis (where the club rod connects to the arm rod) in kg m
    m_shoulderHorizAccel = 0.1 * m_gravity; // Horizontal acceleration of the shoulder in  m/s^2
    m_gamma = Utility::ToRadians(135.0);
    m_theta = m_gamma - m_alpha;  // Angle between arm rod and vertical axis in radians  
}

void GolfSwing::SetShoulderAccel(double aShouldAcell)
{
    m_shoulderHorizAccel = aShouldAcell * m_gravity;
}

void GolfSwing::SetQalpha(double aQalpha)
{
    m_Qalpha = aQalpha;
}

void GolfSwing::SetQbeta(double aQbeta)
{
    m_Qbeta = aQbeta;
}

void GolfSwing::UpdateGolfSwingValues()
{
    m_clubFirstMoment = m_clubMass * m_clubLength * m_clubBalancePoint; // First moment of the rod representing the club about the wrist axis (where the club rod connects to the arm rod) in kg m
    m_armFirstMoment = m_armMass * m_armLength * m_armBalancePoint; // First moment of the arm rod about the shoulder axis kg m
    double swingFactor = m_backSwingPercentage * 0.01;
    m_beta = m_beta * swingFactor;
    m_gamma = m_gamma * swingFactor;
    m_theta = m_gamma - m_alpha;  // Angle between arm rod and vertical axis in radians

}
