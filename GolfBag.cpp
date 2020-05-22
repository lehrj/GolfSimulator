#include "GolfBag.h"
#include <iostream>

GolfBag::GolfBag()
{
    BuildBag();

}
GolfBag::~GolfBag()
{
}

void GolfBag::BuildBag()
{
    m_bag.clear();
    m_bag.resize(m_bagSize);
    // Club data modeled off of Ping G400 driver, G400 woods, and G irons
    int i = 0;
    m_bag[i].clubName = "Driver";
    m_bag[i].clubAngle = 10.0;
    m_bag[i].clubBalancePoint = 0.75;
    m_bag[i].clubCoR = 0.78; 
    m_bag[i].clubLength = 1.1;
    m_bag[i].clubMass = 0.4;
    m_bag[i].clubMassMoI = 0.08;
    m_bag[i].clubFirstMoment = m_bag[i].clubMass * m_bag[i].clubLength * m_bag[i].clubBalancePoint; // First moment of the rod representing the club about the wrist axis (where the club rod connects to the arm rod) in kg m 
    ++i;

    m_bag[i].clubName = "3 Wood";
    m_bag[i].clubAngle = 14.5;
    m_bag[i].clubBalancePoint = 0.75;
    m_bag[i].clubCoR = 0.78;
    m_bag[i].clubLength = 1.0922;
    m_bag[i].clubMass = 0.4;
    m_bag[i].clubMassMoI = 0.08;
    m_bag[i].clubFirstMoment = m_bag[i].clubMass * m_bag[i].clubLength * m_bag[i].clubBalancePoint;
    ++i;

    m_bag[i].clubName = "5 Wood";
    m_bag[i].clubAngle = 17.5;
    m_bag[i].clubBalancePoint = 0.75;
    m_bag[i].clubCoR = 0.78;
    m_bag[i].clubLength = 1.0668;
    m_bag[i].clubMass = 0.4;
    m_bag[i].clubMassMoI = 0.08;
    m_bag[i].clubFirstMoment = m_bag[i].clubMass * m_bag[i].clubLength * m_bag[i].clubBalancePoint;
    ++i;

    m_bag[i].clubName = "Hybrid";
    m_bag[i].clubAngle = 17.5;
    m_bag[i].clubBalancePoint = 0.75;
    m_bag[i].clubCoR = 0.78;
    m_bag[i].clubLength = 1.0668;
    m_bag[i].clubMass = 0.4;
    m_bag[i].clubMassMoI = 0.08;
    m_bag[i].clubFirstMoment = m_bag[i].clubMass * m_bag[i].clubLength * m_bag[i].clubBalancePoint;
    ++i;

    m_bag[i].clubName = "4 Iron";
    m_bag[i].clubAngle = 21.0;
    m_bag[i].clubBalancePoint = 0.75;
    m_bag[i].clubCoR = 0.78;
    m_bag[i].clubLength = 0.987425;
    m_bag[i].clubMass = 0.4;
    m_bag[i].clubMassMoI = 0.08;
    m_bag[i].clubFirstMoment = m_bag[i].clubMass * m_bag[i].clubLength * m_bag[i].clubBalancePoint;
    ++i;

    m_bag[i].clubName = "5 Iron";
    m_bag[i].clubAngle = 24.0;
    m_bag[i].clubBalancePoint = 0.75;
    m_bag[i].clubCoR = 0.78;
    m_bag[i].clubLength = 0.97155;
    m_bag[i].clubMass = 0.4;
    m_bag[i].clubMassMoI = 0.08;
    m_bag[i].clubFirstMoment = m_bag[i].clubMass * m_bag[i].clubLength * m_bag[i].clubBalancePoint;
    ++i;

    m_bag[i].clubName = "6 Iron";
    m_bag[i].clubAngle = 27.0;
    m_bag[i].clubBalancePoint = 0.75;
    m_bag[i].clubCoR = 0.78;
    m_bag[i].clubLength = 0.955675;
    m_bag[i].clubMass = 0.4;
    m_bag[i].clubMassMoI = 0.08;
    m_bag[i].clubFirstMoment = m_bag[i].clubMass * m_bag[i].clubLength * m_bag[i].clubBalancePoint;
    ++i;

    m_bag[i].clubName = "7 Iron";
    m_bag[i].clubAngle = 30.5;
    m_bag[i].clubBalancePoint = 0.75;
    m_bag[i].clubCoR = 0.78;
    m_bag[i].clubLength = 0.9398;
    m_bag[i].clubMass = 0.4;
    m_bag[i].clubMassMoI = 0.08;
    m_bag[i].clubFirstMoment = m_bag[i].clubMass * m_bag[i].clubLength * m_bag[i].clubBalancePoint;
    ++i;

    m_bag[i].clubName = "8 Iron";
    m_bag[i].clubAngle = 35.0;
    m_bag[i].clubBalancePoint = 0.75;
    m_bag[i].clubCoR = 0.78;
    m_bag[i].clubLength = 0.9271;
    m_bag[i].clubMass = 0.4;
    m_bag[i].clubMassMoI = 0.08;
    m_bag[i].clubFirstMoment = m_bag[i].clubMass * m_bag[i].clubLength * m_bag[i].clubBalancePoint;
    ++i;

    m_bag[i].clubName = "9 Iron";
    m_bag[i].clubAngle = 38.5;
    m_bag[i].clubBalancePoint = 0.75;
    m_bag[i].clubCoR = 0.78;
    m_bag[i].clubLength = 0.9144;
    m_bag[i].clubMass = 0.4;
    m_bag[i].clubMassMoI = 0.08;
    m_bag[i].clubFirstMoment = m_bag[i].clubMass * m_bag[i].clubLength * m_bag[i].clubBalancePoint;
    ++i;

    m_bag[i].clubName = "PW";
    m_bag[i].clubAngle = 45.0;
    m_bag[i].clubBalancePoint = 0.75;
    m_bag[i].clubCoR = 0.78;
    m_bag[i].clubLength = 0.9017;
    m_bag[i].clubMass = 0.4;
    m_bag[i].clubMassMoI = 0.08;
    m_bag[i].clubFirstMoment = m_bag[i].clubMass * m_bag[i].clubLength * m_bag[i].clubBalancePoint;
    ++i;

    m_bag[i].clubName = "UW";
    m_bag[i].clubAngle = 50.0;
    m_bag[i].clubBalancePoint = 0.75;
    m_bag[i].clubCoR = 0.78;
    m_bag[i].clubLength = 0.9017;
    m_bag[i].clubMass = 0.4;
    m_bag[i].clubMassMoI = 0.08;
    m_bag[i].clubFirstMoment = m_bag[i].clubMass * m_bag[i].clubLength * m_bag[i].clubBalancePoint;
    ++i;

    m_bag[i].clubName = "SW";
    m_bag[i].clubAngle = 54.0;
    m_bag[i].clubBalancePoint = 0.75;
    m_bag[i].clubCoR = 0.78;
    m_bag[i].clubLength = 0.89535;
    m_bag[i].clubMass = 0.4;
    m_bag[i].clubMassMoI = 0.08;
    m_bag[i].clubFirstMoment = m_bag[i].clubMass * m_bag[i].clubLength * m_bag[i].clubBalancePoint;
    ++i;
    /*
    m_bag[i].clubName = "LW";
    m_bag[i].clubAngle = 58.0;
    m_bag[i].clubBalancePoint = 0.75;
    m_bag[i].clubCoR = 0.78;
    m_bag[i].clubLength = 0.889;
    m_bag[i].clubMass = 0.4;
    m_bag[i].clubMassMoI = 0.08;
    m_bag[i].clubFirstMoment = m_bag[i].clubMass * m_bag[i].clubLength * m_bag[i].clubBalancePoint;
    */
    m_bag[i].clubName = "Default";
    m_bag[i].clubAngle = 25.0;
    m_bag[i].clubBalancePoint = 0.75;
    m_bag[i].clubCoR = 0.78;
    m_bag[i].clubLength = 1.1;
    m_bag[i].clubMass = 0.4;
    m_bag[i].clubMassMoI = 0.08;
    m_bag[i].clubFirstMoment = m_bag[i].clubMass * m_bag[i].clubLength * m_bag[i].clubBalancePoint;
}

void GolfBag::PrintClubList()
{
    printf("======================================== Club List =========================================\n");
    
    for (int i = 0; i < m_bagSize; ++i)
    {
        std::cout << " " << i << ") " << m_bag[i].clubName << std::endl;
    }
    printf("============================================================================================\n");
}
