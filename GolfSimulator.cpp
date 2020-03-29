// GolfSimulation 
//
//
#include "Environment.h"
#include "GolfBall.h"
#include "GolfSwing.h"
#include "Vector4d.h"
#include <iostream>

int main()
{
    bool isRunSimulation = true;
    while (isRunSimulation == true)
    {
        Environment* pEnvironment = new Environment();
        pEnvironment->SetDefaultEnvironment();   
        GolfSwing* pSwing = new GolfSwing();
        pSwing->SetDefaultSwingValues(pEnvironment->GetGravity());
        pSwing->UpdateGolfSwingValues();
        GolfBall* pBall = new GolfBall();
        pBall->SetDefaultBallValues(pEnvironment);
        
        std::cout << "Welcome to GolfSimulator!\n";
        
        bool isInputValid = false;
        bool isInputTrue;
        while (isInputValid == false)
        {
            std::cout << "Would you like to input swing values? (1 or 0) : ";
            std::cin >> isInputTrue;
            if (std::cin.fail())
            {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Error, invalid input, please try again \n";
            }
            else
            {
                isInputValid = true;
                if (isInputTrue == true)
                {
                    pSwing->InputSwingValues();
                }
            }
        }

        isInputValid = false;
        while (isInputValid == false)
        {
            std::cout << "Would you like to read swing values from file? (1 or 0) : ";
            std::cin >> isInputTrue;
            if (std::cin.fail())
            {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Error, invalid input, please try again \n";
            }
            else
            {
                isInputValid = true;
                if (isInputTrue == true)
                {
                    pSwing->ReadInSwingValues();
                }
            }
        }

        Vector4d swingVector = pSwing->CalculateLaunchVector();
        std::cout << "swingVector Vector = \n";
        swingVector.PrintVector();
        std::cout << "\n";

        pBall->FireProjectile(swingVector, pEnvironment);

        std::cout << "Would you like to run the simulation again? (1 or 0) : ";
        std::cin >> isRunSimulation;
        delete pSwing;
        delete pBall;
        delete pEnvironment;
    }
    system("pause");
    return 0;
}


