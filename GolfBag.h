#pragma once
#include <string>
#include <vector>

struct GolfClub
{
    std::string clubName;
    double clubAngle;         // in degrees
    double clubBalancePoint;
    double clubCoR; // club face coefficiant of restitution, aka club spring face, current USGA rules limit this to .830 in tournemnt play
    double clubFirstMoment; // First moment of the rod representing the club about the wrist axis (where the club rod connects to the arm rod) in kg m
    double clubLength; // length of club in m
    double clubMass;
    double clubMassMoI; // Mass moment of inertia of the rod representing the club in kg m^2
};

class GolfBag
{
public:
    GolfBag();
    ~GolfBag();

    void BuildBag();
    GolfClub GetClub(const int aClubIndex) { return m_bag[aClubIndex]; };
    int GetClubCount() { return m_bagSize; };
    void PrintClubList();

private:
    const int m_bagSize = 14;
    std::vector<GolfClub> m_bag;

};
