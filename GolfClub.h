#pragma once
#include <string>

struct GolfBag
{
};


class GolfClub
{
public:

private:
    // new
    std::string m_clubName;
    

    double m_clubAngle;         // in degrees
    double m_clubBalancePoint;
    double m_clubCoR; // club face coefficiant of restitution, aka club spring face, current USGA rules limit this to .830 in tournemnt play
    double m_clubFirstMoment; // First moment of the rod representing the club about the wrist axis (where the club rod connects to the arm rod) in kg m
    double m_clubLength; // length of club in m
    double m_clubMass; 
    double m_clubMassMoI; // Mass moment of inertia of the rod representing the club in kg m^2


    const double m_minClubAngle = 0.0;
    const double m_maxClubAngle = 80.0;
    const double m_minClubCoR = 0.1;
    const double m_maxClubCoR = 1.0;
    const double m_minClubLength = 0.1;
    const double m_maxClubLength = 5.0;
    const double m_minClubMass = 0.0;
    const double m_maxClubMass = 10.0;
};
