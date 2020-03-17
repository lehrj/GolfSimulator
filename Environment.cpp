#include "Environment.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>


void Environment::InputEnvironmentData()
{
    bool isInputValid = false;
    while (isInputValid == false)
    {
        double airDensity;
        std::cout << "Input value for Air Density in kg/m^3 between " << m_minAirDensity << " and " << m_maxAirDensity << ": ";
        std::cin >> airDensity;
        if (airDensity >= m_minAirDensity && airDensity <= m_maxAirDensity)
        {
            isInputValid = true;
            SetAirDensity(m_airDensity);
        }
        else
        {
            std::cout << "Input Error, please try again \n";
        }
    }

    isInputValid = false;
    while (isInputValid == false)
    {
        double gravity;
        std::cout << "Input value for Gravity in m/s^2 between " << m_minGravity << " and " << m_maxGravity << ": ";
        std::cin >> gravity;
        if (gravity >= m_minGravity && gravity <= m_maxGravity)
        {
            isInputValid = true;
            SetGravity(gravity);
        }
        else
        {
            std::cout << "Input Error, please try again \n";
        }
    }
    
    isInputValid = false;
    while (isInputValid == false)
    {
        double landingHeight;
        std::cout << "Input value for Landing Height in meters  between " << -m_minMaxHeight << " and " << m_minMaxHeight << ": ";
        std::cin >> landingHeight;
        if (landingHeight >= -m_minMaxHeight && landingHeight <= m_minMaxHeight)
        {
            isInputValid = true;
            m_landingHeight = landingHeight;
        }
        else
        {
            std::cout << "Input Error, please try again \n";
        }
    }

    isInputValid = false;
    while (isInputValid == false)
    {
        double launchHeight;
        std::cout << "Input value for Launch Height in meters between " << -m_minMaxHeight << " and " << m_minMaxHeight << ": ";
        std::cin >> launchHeight;
        if (launchHeight >= -m_minMaxHeight && launchHeight <= m_minMaxHeight)
        {
            isInputValid = true;
            m_launchHeight = launchHeight;
        }
        else
        {
            std::cout << "Input Error, please try again \n";
        }
    }

    isInputValid = false;
    while (isInputValid == false)
    {
        double windX;
        std::cout << "Input value for Wind X in m/s between " << -m_minMaxWind << " and " << m_minMaxWind << ": ";
        std::cin >> windX;
        if (windX >= -m_minMaxWind && windX <= m_minMaxWind)
        {
            isInputValid = true;
            SetWindX(m_windX);
        }
        else
        {
            std::cout << "Input Error, please try again \n";
        }
    }

    isInputValid = false;
    while (isInputValid == false)
    {
        double windY;
        std::cout << "Input value for Wind Y in m/s between " << -m_minMaxWind << " and " << m_minMaxWind << ": ";
        std::cin >> windY;
        if (windY >= -m_minMaxWind && windY <= m_minMaxWind)
        {
            isInputValid = true;
            m_windY = windY;
        }
        else
        {
            std::cout << "Input Error, please try again \n";
        }
    }

    isInputValid = false;
    while (isInputValid == false)
    {
        double windZ;
        std::cout << "Input value for Wind Z in m/s between " << -m_minMaxWind << " and " << m_minMaxWind << ": ";
        std::cin >> windZ;
        if (windZ >= -m_minMaxWind && windZ <= m_minMaxWind)
        {
            isInputValid = true;
            m_windZ = windZ;
        }
        else
        {
            std::cout << "Input Error, please try again \n";
        }
    }
}

void Environment::ReadInEnvironmentData()
{
    double airDensity;
    double gravity;
    double landingHeight;
    double launchHeight;
    double windX;
    double windY;
    double windZ;
    
    std::ifstream inFile("EnvironmentInputData.txt");

    inFile >> airDensity >> gravity >> landingHeight >> launchHeight >> windX >> windY >> windZ;
    inFile.close();

    std::vector<std::string> errorList;

    bool isInputValid = true;
    if (airDensity < m_minAirDensity || airDensity > m_maxAirDensity)
    {
        isInputValid = false;
        std::string errorString = "1 airDensity";
        errorList.push_back(errorString);
    }

    if (gravity < m_minGravity || gravity > m_maxGravity)
    {
        isInputValid = false;
        std::string errorString = "2 gravity";
        errorList.push_back(errorString);
    }

    if (landingHeight < -m_minMaxHeight || landingHeight > m_minMaxHeight)
    {
        isInputValid = false;
        std::string errorString = "3 landingHeight";
        errorList.push_back(errorString);
    }
    if (launchHeight < -m_minMaxHeight || launchHeight > m_minMaxHeight)
    {
        isInputValid = false;
        std::string errorString = "4 launchHeight";
        errorList.push_back(errorString);
    }

    if (windX < m_minMaxWind || windX > m_minMaxWind)
    {
        isInputValid = false;
        std::string errorString = "5 windX";
        errorList.push_back(errorString);
    }

    if (windY < m_minMaxWind || windY > m_minMaxWind)
    {
        isInputValid = false;
        std::string errorString = "6 windY";
        errorList.push_back(errorString);
    }

    if (windZ < m_minMaxWind || windZ > m_minMaxWind)
    {
        isInputValid = false;
        std::string errorString = "7 windZ";
        errorList.push_back(errorString);
    }

    if (isInputValid == false)
    {
        std::cout << "Error in EnvironmentInputData.txt, data invalid\n";
        std::cerr << "Error in EnvironmentInputData.txt, data invalid\n";
        for (unsigned int i = 0; i < errorList.size(); i++)
        {
            std::cout << "Error in line : " << errorList[i] << "\n";
            std::cerr << "Error in line : " << errorList[i] << "\n";
        }

        std::cout << "Reverting to default Environment data \n";
        SetDefaultEnvironment();
    }
    else
    {
        SetAirDensity(airDensity); 
        SetGravity(gravity);
        SetLandingHeight(landingHeight);
        SetLauchHeight(launchHeight);
        SetWindX(windX);
        SetWindY(windY);
        SetWindZ(windZ);
    }
}

void Environment::SetDefaultEnvironment()
{
    m_gravity = -9.8;
    m_airDensity = 1.225;
    m_windX = 0.0;
    m_windY = 0.0;
    m_windZ = 0.0;
    m_launchHeight = 0.0;
    m_landingHeight = 0.0;

    // variables for bounce and roll functionality not yet implemented
    m_landingFriction = 1.0;
    m_landingHardness = 1.0;
    m_landingXslope = 0.0;
    m_landingZslope = 0.0;
}

void Environment::SetAirDensity(double density)
{
    m_airDensity = density;
}

void Environment::SetGravity(double gravity)
{
    m_gravity = gravity;
}

void Environment::SetLandingHeight(double landingHeight)
{
    m_landingHeight = landingHeight;
}

void Environment::SetLauchHeight(double launchHeight)
{
    m_launchHeight = launchHeight;
}

void Environment::SetWindX(double windX)
{
    m_windX = windX;
}

void Environment::SetWindY(double windY)
{
    m_windY = windY;
}

void Environment::SetWindZ(double windZ)
{
    m_windZ = windZ;
}