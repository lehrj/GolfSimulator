#pragma once

class Environment
{
public:
    const double GetAirDensity() { return m_airDensity; };
    const double GetGravity() { return m_gravity; };
    const double GetLandingHeight() { return m_landingHeight; };
    const double GetLauchHeight() { return m_launchHeight; };
    const double GetWindX() { return m_windX; };
    const double GetWindY() { return m_windY; };
    const double GetWindZ() { return m_windZ; };
    
    void InputEnvironmentData();
    void ReadInEnvironmentData();

    void SetAirDensity(double density);
    void SetDefaultEnvironment();
    void SetGravity(double gravity);
    void SetLandingHeight(double landingHeight);
    void SetLauchHeight(double launchHeight);
    void SetWindX(double windX);
    void SetWindY(double windY);
    void SetWindZ(double windZ);

private:
    double m_airDensity;
    double m_gravity;
    double m_landingHeight;
    double m_launchHeight;
    double m_windX;
    double m_windY;
    double m_windZ;

    // variables for bounce and roll functionality not yet implemented
    double m_landingFriction;
    double m_landingHardness;
    double m_landingXslope;
    double m_landingZslope;
   
    // min max consts
    const double m_minAirDensity = 0.0;
    const double m_maxAirDensity = 68.0; // just above the air density of Venus
    const double m_minGravity = 0.1; 
    const double m_maxGravity = 28.0; // approximate value for the mass of the sun
    const double m_minMaxHeight = 450.0; // Launch & Landing min/max heights is just above the largest elevation change (>400 meters) of any real golf course which is the Extreme 19 in Limpopo Province South Africa
    const double m_minMaxWind = 667.0;// highest know wind speed on Neptune
};

