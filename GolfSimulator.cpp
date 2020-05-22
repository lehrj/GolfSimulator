// GolfSimulator
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
        std::cout << "Welcome to GolfSimulator!\n";
        Environment* pEnvironment = new Environment();
        pEnvironment->SetDefaultEnvironment();   
        GolfSwing* pSwing = new GolfSwing();
        pSwing->SetDefaultSwingValues(pEnvironment->GetGravity());
       
        bool isInDevMode = false;
        if (isInDevMode == false) // Toggle to skip input options and run on defaults to speed up testing
        {
            bool isInputValid = false;
            bool isInputTrue;
            while (isInputValid == false)
            {
                pEnvironment->PrintEnvironmentData();
                std::cout << "Would you like to input environment values? (1 or 0) : ";
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
                        pEnvironment->InputEnvironmentDataBasic();
                        pEnvironment->PrintEnvironmentData();
                        isInputValid = false;  // allow users to edit their inputs
                    }
                }
            }

            isInputValid = false;
            while (isInputValid == false)
            {
                pSwing->PrintSwingInputData();
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
                        pSwing->InputSwingValuesBasic();
                        isInputValid = false; // allow users to edit their inputs
                    }   
                }
            }
        }

        pSwing->UpdateGolfSwingValues();
        GolfBall* pBall = new GolfBall();
        pBall->SetDefaultBallValues(pEnvironment);

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


