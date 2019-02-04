#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
using namespace std;


 // funkcija lai izprintētu 2d vektoru
void printVector(vector< vector <float> > v)
{
 for (size_t i=0; i<v.size(); ++i)
    {
        for (size_t j=0; j<v[i].size(); ++j)
        {
            cout << fixed << setprecision(1) << v[i][j] << "\t";
        }
     cout << endl;
    }
    cout << endl << endl;
}

/* Funckcija netiek pielietota
bool isfloat( string myString ) {
    std::istringstream iss(myString);
    float f;
    iss >> noskipws >> f;
    return iss.eof() && !iss.fail();
}
*/

//  funkcija ielasa failu un to pārkopē uz 2d vektoru kā float tipu
void readFile(string filename, vector< vector<float> > &twodvector)
{
    ifstream myfile(filename, ios::in);
    string line;
    string field;
    vector<float> v;                // info par vienu reklāmu (viena faila rinda)
    float result;
    if(myfile.is_open())
        {
    while ( getline(myfile,line) )    // getline nākamajai faila rindai
    {
        v.clear();
        stringstream ss(line);          // stringstream, lai apstrādātu ielādēto faila rindu

        while (getline(ss,field,','))  // komats atdala 'field' elementus
        {
            istringstream convert(field);
            if ( !(convert >> result) )  result = 0;   // konvertē field vērtību string uz float

            v.push_back(result);  // float vērtības pievieno 1D vektoram

        }

        twodvector.push_back(v);  // 1D vektoru pievieno 2D vektoram
    }

        }
        else
        {
            cout  << "Fails netika korekti ielādēts";
        }
myfile.close();

}


// funkcija biggestScoreSlot atrod slotu, kuram ir lielākais attiecīgo reklāmas trp vērtību score
int biggestScoreSlot(vector< vector<float> > &adsvector, vector < vector<float> > &slotsvector, float trpArr[11], int adrow)
{

 float slotscore;
 float biggestScore = 0;
 int id;
 float timeleft;


     for (size_t i=0; i<slotsvector.size(); i++)  // iterē caur slotvector rindām
        {
        slotscore = 0;
        timeleft = slotsvector[i][2] - adsvector[adrow][1];

        if(timeleft >= -10) // ja pievienojot reklāmu šajā slota rindā, tad jāpārbauda - vai slota garums nepārsniegs 10 sekundes
             {
            for (size_t j=3; j<slotsvector[i].size(); ++j) // iterē caur slotvector trp kolonnām
                {
                    if(trpArr[j-1] > 0)
                    {
                        slotscore += slotsvector[i][j];
                    }
                }


                if(slotscore > biggestScore)
                    {
                        biggestScore = slotscore;
                        id = slotsvector[i][0];
                    }

            }



        }


    if(biggestScore != 0)
        {
        return id;
        }
    else
        {
        return -1; // gadījumā, ja nav palikusi neviena reklāma ar trp ,kas interesē
        }
}


// izvada reklāmas un tai piemērotā slota ID failā 'result.csv'
void outPutFile(int slotId, int adId)
{
    ofstream newfile("result.txt", ios::app);
    if(newfile.is_open() )
    {
        newfile << adId <<"," << slotId << "\n";
        newfile.close();
    }
    else
    {
        cout << "Neizdevās atvērt result.txt failu";
    }
}


// funkcija paredzēta, lai pēc reklāmas ievietošānas tai piemērotā slotā, atņemtu trp vērtības, kā arī slotam atņem reklāmas laiku
void subtracttrp(vector< vector<float> > &adsvector, vector< vector<float> > &slotsvector, int adRow, int slotId)
{
    int slotRow = 0;
    vector<float> v = {0,0,0,0,0,0,0,0,0,0,0}; // īslaicīgais vektors, kurā saglabā adsvector vērtības, lai no adsvector var atņemt slotsvector vērtības UN otrādi


    for(size_t i = 0;i < adsvector[adRow].size(); ++i)
    {
        v[i] = adsvector[adRow][i];   // pārkopē adsvectoru uz vektoru 'v'
    }

   while(slotsvector[slotRow][0] != slotId) // atrod slotsvector rindu ar pareizo 'slotId'
    {
        slotRow++;
    }

    for(size_t i = 2;i < slotsvector[slotRow].size(); ++i)
    {
            adsvector[adRow][i] -= slotsvector[slotRow][i+1]; // atņēm no adsvektora trp vērtībām slotsvektora trp vērtības
            //slotsvector[slotRow][i] -= v[i-1];  // atņem no slotsvector trp vērtībām 'v' trp vērtības
            slotsvector[slotRow][i] -= adsvector[adRow][i-1]; // atņēm no visa slota laika reklāmas laiku
    }
   return;
}


// visas masīva vērtības nomaina uz 0
void clearArrayValues(float arr[])
{
    for(int i = 2;i < 11; ++i)
    {
        arr[i] = 0;
    }
}


// funkcija, kas sakārto reklāmas ar tām vispiemērotākajiem slotiem
void fillSlots(vector< vector<float> > &adsvector, vector< vector<float> > &slotsvector)
{                // n n 0 1 2 3 4 5 6 7 grp
 float trpArr[] = {0,0,0,0,0,0,0,0,0,0,0};   // masīvs, kurā īslaicīgi saglabā 'ads' trp vērtības size 11
 int slotId = 0;    // slotId paredzēts, lai saglabātu 'slotvector' id vērtību
 int adId;  // adId paredzēts, lai saglabātu 'advector' id vērtību
 float adsscore;
 vector< vector<float> > remainingAds;

 size_t i = 0;
 while(i < adsvector.size())
    {
    adsscore = 0;
   //if(adsvector[i][1] <= 0) // ja reklāma jau ir parādīta
  // {
      //  i++;

    //}
        for(size_t j = 2; j < adsvector[i].size(); j++) // iterē caur 'adsvector' kolonnām, kur it trp vērtības
        {

            if(adsvector[i][j] > 0) // ja trp vērtība lielāka par 0 tad to pieskaita pie adsscore
            {
                trpArr[j] += adsvector[i][j]; // trpArr masīvā saglabā trp vērtību
                adsscore += adsvector[i][j];
            }
        }


        if(adsscore > 0)    // ja reklāmai ir trp vērtības, kas lielākas par 0 , tad šai reklāmai ir jāpievieno slots
         {
            int adRow = i;

            slotId = biggestScoreSlot(adsvector, slotsvector, trpArr, i);

            cout << "row: " << i << " SLOTID: " << slotId << endl;
            if(slotId != -1)
            {
                adId = adsvector[i][0];
                outPutFile(slotId, adId);

                subtracttrp(adsvector,slotsvector, adRow, slotId);

            }
            else // ja nav piemērota slota
             {
                //storeRemainingAds(adsvector,adRow);

               i++;
             }
            clearArrayValues(trpArr);
            continue;
        }
        else      // iterē uz nākamo rindu tikai tad, kad apmierinātas visas esošās rindas reklāmas trp vērtības
            {
                ++i;
            }
    }
}



int main()
{
   vector< vector<float> > slots;
    readFile("slots.txt", slots);   // ielādē faila 'slots.txt saturu 2d vektorā 'slots'
    printVector(slots);


    vector< vector<float> > ads;
    readFile("ads.txt", ads);   // ielādē faila 'ads.txt' saturu 2d vektorā 'slots'
    cout << "SLOTSVECTOR SIZE " << slots.size();

    fillSlots(ads, slots);
    //float trpArr[11] = {0,0,0,0,0,0,0,0,0.4,0.2,2};

     //int id = biggestScoreSlot(ads, slots, trpArr, 0);
    //cout << "ID: " << id;

    cout << "DONE" << endl;
    cout << "PRINTING SLOTS: " << endl;
    printVector(slots);

return 0;
}

