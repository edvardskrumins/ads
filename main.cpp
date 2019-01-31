#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
 using namespace std;


 // funkcija lai izprintētu 2d vektoru
void printVector(vector< vector <double> > v)
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
bool isFloat( string myString ) {
    std::istringstream iss(myString);
    float f;
    iss >> noskipws >> f;
    return iss.eof() && !iss.fail();
}
*/

//  funkcija ielasa failu un to pārkopē uz 2d vektoru kā double tipu
void readFile(char* filename, vector< vector<double> > &twodvector)
{
    ifstream myfile(filename, ios::in);
    string line;
    string field;
    vector<double> v;                // info par vienu reklāmu (viena faila rinda)
    double result;
    if(myfile.is_open()) {
    while ( getline(myfile,line) )    // getline nākamajai faila rindai
    {
        v.clear();
        stringstream ss(line);          // stringstream, lai apstrādātu ielādēto faila rindu

        while (getline(ss,field,','))  // komats atdala 'field' elementus
        {
            istringstream convert(field);
            if ( !(convert >> result) )     // konvertē field vērtību string uz double
                result = 0;
            v.push_back(result);  // double vērtības pievieno 1D vektoram

        }

        twodvector.push_back(v);  // 1D vektoru pievieno 2D vektoram
    }

        }else {
    cout  << "Fails netika korekti ielādēts";
    }
myfile.close();

}


// funkcija biggestScoreSlot atrod slotu, kuram ir lielākais attiecīgo reklāmas trp vērtību score
int biggestScoreSlot(vector< vector<double> > &adsvector, vector < vector<double> > &slotsvector, double trpArr[11], int adrow)
{
 double slotscore;
 double biggestScore = 0;
 int id = 0;
    for(size_t i = 0; i < slotsvector.size(); ++i)  // iterē caur slotvector rindām
        {
        slotscore = 0;
        double timeleft = slotsvector[i][2] - adsvector[adrow][1] ;
        if(timeleft >= -10) // ja pievienojot reklāmu šajā slota rindā, tad jāpārbauda - vai slota garums nepārsniegs 10 sekundes
            {
            for(size_t j = 2; j < slotsvector[i].size(); j++) // iterē caur slotvector trp kolonnām
                {

                    if(trpArr[j] != 0)
                    {
                        slotscore += slotsvector[i][j+1]; // j+1, jo trp0 vērtība adsvectoram atbilst trp1 vērtībai slotsvectoram
                        //cout << "slotscore for : " << slotsvector[i][0] << " IS : " << slotscore << endl;
                    }
                }

                if(slotscore > biggestScore)
                    {
                        biggestScore = slotscore;
                        id = slotsvector[i][0];
                    }

        } else {
            continue;
        }
 }

if(biggestScore != 0)
    {
    return id;
    }
else {
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
void subtracttrp(vector< vector<double> > &adsvector, vector< vector<double> > &slotsvector, int adRow, int slotId)
{
    int slotRow = 0;
    vector<double> v = {0,0,0,0,0,0,0,0,0,0,0}; // īslaicīgais vektors, kurā saglabā adsvector vērtības, lai no adsvector var atņemt slotsvector vērtības UN otrādi


    for(size_t i = 0;i < adsvector[adRow].size(); ++i)
    {
        v[i] = adsvector[adRow][i];   // pārkopē adsvectoru uz vektoru 'v'
    }

   while(slotsvector[slotRow][0] != slotId) // atrod slotsvector rindu ar pareizo 'slotId'
    {
        slotRow++;
    }
        for(size_t i = 1;i < slotsvector[slotRow].size(); ++i)
        {
            adsvector[adRow][i+1] -= slotsvector[slotRow][i+2]; // atņēm no adsvektora trp vērtībām slotsvektora trp vērtības
            slotsvector[slotRow][i+1] -= v[i];  // atņem no slotsvector trp vērtībām 'v' trp vērtības
        }
}


// visas masīva vērtības nomaina uz 0
void clearArrayValues(double arr[])
{
    for(int i = 0;i < 11; ++i)
    {
        arr[i] = 0;
    }
}


// funkcija, kas sakārto reklāmas ar tām vispiemērotākajiem slotiem
void fillSlots(vector< vector<double> > &adsvector, vector< vector<double> > &slotsvector)
{                // n n 0 1 2 3 4 5 6 7 grp
 double trpArr[] = {0,0,0,0,0,0,0,0,0,0,0};   // masīvs, kurā īslaicīgi saglabā 'ads' trp vērtības size 11
 int slotId = 0;    // slotId paredzēts, lai saglabātu 'slotvector' id vērtību
 int adId;  // adId paredzēts, lai saglabātu 'advector' id vērtību
 double adsscore;

 size_t i = 0;
 while(i < adsvector.size())
    {
    adsscore = 0;
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
            slotId = biggestScoreSlot(adsvector, slotsvector, trpArr, i);
            if(slotId != -1)
            {
                adId = adsvector[i][0];
                outPutFile(slotId, adId);
                int adRow = i;
                subtracttrp(adsvector,slotsvector, adRow, slotId);
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
   vector< vector<double> > slots;
    readFile("slots.txt", slots);   // ielādē faila 'slots.txt saturu 2d vektorā 'slots'
    printVector(slots);


    vector< vector<double> > ads;
    readFile("ads.txt", ads);   // ielādē faila 'ads.txt' saturu 2d vektorā 'slots'
    printVector(ads);


    fillSlots(ads, slots);
    cout << "DONE";

return 0;
}

