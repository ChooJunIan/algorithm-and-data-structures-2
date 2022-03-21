#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
using namespace std;

/**
 * The Planet Class
 * 
 * This class will help us to store the values of each Planet contained within the the textfile.
 * Each Planet will contain the following fields:
 * -> Name, Number, Coordinates (x,y,z), Weight, Profit and Benefit.
 * 
 */
class Planet {

    public: 
        string p_name;
        int p_number;
        int x,y,z;
        int weight, profit, benefit = 0;
        
        /**
         * Set the name for a planet
         * 
         * @param name The name of the planet
         */
        void setPName (string name) {
            this->p_name = name;
        }

        /**
         * Set the number of the planet
         * 
         * @param number The number of the planet
         */
        void setPNumber (int number) {
            this->p_number = number;
        }

        /**
         * Set the coordinates of the planet
         * 
         * @param x The x-coordinate of the planet 
         * @param y The y-coordinate of the planet
         * @param z The z-coordinate of the planet
         */
        void setXYZ (int x, int y, int z) {
            this->x = x;
            this->y = y;
            this->z = z;
        }

        /**
         * Set the weight of the planet
         * 
         * @param weight The weight of the planet
         */
        void setWeight (int weight) {
            this->weight = weight;
        }

        /**
         * Set the weight of the planet
         * 
         * @param profit The profit of the planet
         */
        void setProfit (int profit) {
            this->profit = profit;
        }

        /**
         * Set the benefit of the planet.
         * 
         * @param benefit The benefit of the planet
         */
        void setBenefit (int benefit) {
            this->benefit = benefit;
        }

        /**
         * Displays information of the planet.
         */
        void displayInfo () {
            cout << this->p_number << ": " << this->p_name << " (" << this->x << ' ' << this->y << ' ' << this->z << ") Weight: " << this->weight << " Profit: " << this->profit << endl; 
        }

};

/**
 * Creates the resulting matrix for calculating the benefit of the planets.
 * 
 * @param p[] The array of planets
 * @param pcount The number of planets
 * @param shipsize The maximum size of the ship
 */
void knapsack(Planet p[], int pcount, int shipsize) {

    //Create 2d array for 01 knapsack and variables relevant for calculating the benefit of each planet
    int table[pcount+1][shipsize+1] = {0};
    int benefit = 0;
    int benefits[pcount] = {0};
    
    // Start time to calculate the benefit in table
   auto t_start = std::chrono::high_resolution_clock::now();

    // Calculate benefits of each planet and put value of benefit into an array
    for(int i = 0; i<pcount; ++i) {
        if(i == 0) {
            p[i].setBenefit(0);
            benefits[i] = p[i].benefit;
        }
        else {
            p[i].setBenefit((p[i].profit/p[i].weight));
            benefits[i] = p[i].benefit;
        }
    }

    // Assign values to the 2d array
    for(int i = 0; i <= pcount; i++) {
        for(int size = 0; size <= shipsize; size++) {
            if( i == 0 || size == 0) 
                table[i][size] = 0;
            else if (p[i].weight <= size)
                table[i][size] = max(p[i].profit + table[i-1][size-p[i].weight], table[i-1][size]);
            else
                table[i][size] = table[i-1][size];
        }
    }

    // Print the 2D array
    cout << "Resulting matrix: " << endl;
    for(int i = 0; i < pcount; i++) {
        cout << "Line " << i << ": ";
        for(int j = 0; j < shipsize; j++) {
            cout << table[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;


    int currentShipSize = 0;
    int currentProfit = 0;
    int currentPCount = pcount;

    while (true) {
        int largestBenefit = 0;
        int planetNumber = 0;
    
        //find max benefit in the array, and get the planet number that is related to that benefit
        for (int i = 0; i < pcount; ++i) {
            if (p[i].benefit > largestBenefit) {
                planetNumber = i;
                largestBenefit = p[i].benefit; 
            }
        }

        //add first, if exceed the shipsize, quit the while loop and minus the value outside, if not, continue like normal.
        currentShipSize += p[planetNumber].weight;
        currentProfit += p[planetNumber].profit;
        
        if (currentShipSize > shipsize) {
            currentShipSize -= p[planetNumber].weight;
            currentProfit -= p[planetNumber].profit;            
            break;
        }

        // display the planet that will be selected, its weight and its benefit.
        cout << p[planetNumber].p_name << " with weight " << p[planetNumber].weight << " and profit " << p[planetNumber].profit << " is selected." << endl;
        

        // remove the benefit from the benefit array, and the corresponding planet from the array. delete by index since you get the index.
        for( int i = planetNumber; i <= (sizeof(benefits)/sizeof(benefits[0]) - 1); i++)
            {
                benefits[i]=benefits[i+1];
                p[i]=p[i+1];
            }
        benefits[currentPCount-1]={};
        p[currentPCount-1] = {};
    }

     // End time to calculate benefit in table
    auto t_end = std::chrono::high_resolution_clock::now();
    double elapsed_time_s = std::chrono::duration<double>(t_end-t_start).count();

    cout << "Total Weight: " << currentShipSize << endl;
    cout << "Total Profit: " << currentProfit << endl;
    cout << "Time Taken: " << elapsed_time_s << "s" << endl;
      
}

int main()
{
    const int n = 10;
    Planet p[n];
    int shipsize = 80;

    // Start off by reading the file 
    ifstream file("A2planets.txt", ios::in );

    // Declare variables to be used to store data in the text file
    string planet;
    int coordinate1, coordinate2, coordinate3, weight, profit;
    int pos = 0;

    if( !file )
        cerr << "Cant open " << endl;

    // Read the strings and integers that represent the planets. Store them into their respective arrays
    while( file >> planet >> coordinate1 >> coordinate2  >> coordinate3 >> weight >> profit)
    {
        p[pos].setPName(planet);
        p[pos].setPNumber(pos);
        p[pos].setXYZ(coordinate1, coordinate2, coordinate3);
        p[pos].setWeight(weight);
        p[pos].setProfit(profit);
        pos++;
    }

    file.close();
    cout << endl;
    
    // Display the data inside each the Planet array
    cout << "Planets List" << endl;
    for(int i = 0; i< 10; ++i) {
        p[i].displayInfo();
    }
    cout << endl; 

    // 0/1 knapsack Solution function.
    knapsack(p, n, shipsize);
    
    return 0;
}