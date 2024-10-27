#include "City.h"

City::City(string filename) 
{
    char *in;
    int i, j;

    for (i = 0; i < MAX_DIM; i++) {
        for (j = 0; j < MAX_DIM; j++) {
            layout[i][j] = char(32);
        }
    }

    ifstream inputfile;
    inputfile.open(filename, ios::in);

    if (!inputfile)
    {
        cout << "Configuration file not found!";
        exit(0);
    }

    // Load configuration file contents to initialize simulation parameters
    inputfile.seekg(14, ios::beg);
    in = new char[15];
    i = 0;
    while (true) {
        inputfile.read((in + i), 1);
        if (*(in + i) == '\n') { *(in + i) = '\0'; break; }
        i += 1;
    }
    this->regionName = in;

    in = new char[5];
    inputfile.seekg(37, ios::beg);
    i = 0;
    while (true) {
        inputfile.read((in + i), 1);
        if (*(in + i) == '\n') { *(in + i) = '\0'; break; }
        i += 1;
    }
    this->time_limit = atoi(in);

    inputfile.seekg(53, ios::beg);
    i = 0;
    while (true) {
        inputfile.read((in + i), 1);
        if (*(in + i) == '\n') { *(in + i) = '\0'; break; }
        i += 1;
    }
    this->refresh_rate = atoi(in);
    inputfile.close();

    inputfile.open(regionName, ios::in);
    //Reading CSV File       

    if(!inputfile)
    {
        cout << "Region file not found!";
        exit(0);
    }

    inputfile.seekg(0, ios::end);
    if (inputfile.tellg()/2 > MAX_DIM*MAX_DIM) {
        cout << "Filesize too big. Not enough memory allocated!";
        exit(0);
    }

    in = new char;
    i = 0; j = 0;
    inputfile.seekg(0, ios::beg);
    while (!inputfile.eof())
    {
        inputfile.read(in, 1);
        if (*in == '\n') {
            *in = inputfile.peek();
            if (*in != EOF)
            {
                ++i;
                j = 0;
            }
        }
        else if (*in == ',') ++j;
        else
        {
            layout[i][j] = *in;
        }
    }

    length = j + 1;
    width = i + 1;

    InitializeZones();
    CreateAdjacency();
    InitContamination();
}

City::~City(){}

void City::display() 
{
    int i, j;
    for (i = 0; i < 2 * length + 1; i++)cout << "-";
    cout << "\n";
    for (i = 0; i < width; i++) {
        cout << "|";
        for (j = 0; j < length; j++) {
            if (j > 0)cout << " ";
            cout << layout[i][j];
        }
        cout << "|";
        cout << "\n";
    }
    for (i = 0; i < 2 * length + 1; i++)cout << "-";
    cout << "\n";
}

void City::displayContamination()
{
    int i, j;
    for (i = 0; i < 2 * length + 1; i++)cout << "-";
    cout << "\n";
    for (i = 0; i < width; i++) {
        cout << "|";
        for (j = 0; j < length; j++) {
            if (j > 0)cout << " ";
            cout << pollution[i][j];
        }
        cout << "|";
        cout << "\n";
    }
    for (i = 0; i < 2 * length + 1; i++)cout << "-";
    cout << "\n";
}


void City::InitializeZones()
{
    int i, j;
    for (i = 0; i < width; i++) {
        for (j = 0; j < length; j++) {
            switch (layout[i][j]) {
            case 'C':
                czones.push_back(new CZone(i, j, adjacentToPowerGrid(i, j), this));
                break;
            case 'R':
                rzones.push_back(new RZone(i, j, adjacentToPowerGrid(i, j)));
                break;
            case 'I':
                izones.push_back(new IZone(i, j, adjacentToPowerGrid(i, j), this));
                break;
            }
        }
    }

}

void City::PrepareLayout()
{
    int i, j, p;
    vector<CZone*>::iterator c1;
    vector<RZone*>::iterator r1;
    vector<IZone*>::iterator i1;

    for (c1 = czones.begin(); c1 != czones.end(); c1++) {
        p = (*c1)->getPopulation();
        if (p > 0) {
            j = (*c1)->getX();
            i = (*c1)->getY();
            layout[i][j] = '0' + p;
        }
    }
    for (r1 = rzones.begin(); r1 != rzones.end(); r1++) {
        p = (*r1)->getPopulation();
        if (p > 0) {
            j = (*r1)->getX();
            i = (*r1)->getY();
            layout[i][j] = '0' + p;
        }
    }
    for (i1 = izones.begin(); i1 != izones.end(); i1++) {
        p = (*i1)->getPopulation();
        if (p > 0) {
            j = (*i1)->getX();
            i = (*i1)->getY();
            layout[i][j] = '0' + p;
        }
    }
}

void City::InitContamination()
{
    for (int i = 0; i < width; i++)
        for (int j = 0; j < length; j++)
            pollution[i][j] = 0;
}

void City::CreateAdjacency() {
    vector<CZone*>::iterator c1, c2;
    vector<RZone*>::iterator r1, r2;
    vector<IZone*>::iterator i1, i2;

    for (c1 = czones.begin(); c1 != czones.end(); c1++) {
        for (c2 = czones.begin(); c2 != czones.end(); c2++) {
            if (c1 == c2) continue;
            if (abs((*c1)->getX() - (*c2)->getX()) <= 1 &&
                abs((*c1)->getY() - (*c2)->getY()) <= 1)
            {
                (*c1)->addAdjs((*c2));
            }
        }
    }

    for (r1 = rzones.begin(); r1 != rzones.end(); r1++) {
        for (r2 = rzones.begin(); r2 != rzones.end(); r2++) {
            if (r1 == r2) continue;
            if (abs((*r1)->getX() - (*r2)->getX()) <= 1 &&
                abs((*r1)->getY() - (*r2)->getY()) <= 1)
            {
                (*r1)->addAdjs((*r2));
            }
        }
    }

    for (i1 = izones.begin(); i1 != izones.end(); i1++) {
        for (i2 = izones.begin(); i2 != izones.end(); i2++) {
            if (i1 == i2) continue;
            if (abs((*i1)->getX() - (*i2)->getX()) <= 1 &&
                abs((*i1)->getY() - (*i2)->getY()) <= 1)
            {
                (*i1)->addAdjs((*i2));
            }
        }
    }
}

bool City::adjacentToPowerGrid(int y, int x)
{
    for (int i = y - 1; i <= y + 1; i++) {
        for (int j = x - 1; j <= x + 1; j++) {
            if (i < 0 || j < 0 || i >= width || j >= length) continue;
            if (layout[i][j] == '#' || layout[i][j] == 'T')
                return true;
        }
    }
    return false;
}

bool City::haschanged(char prev[MAX_DIM][MAX_DIM])
{
    for (int i = 0; i < width; i++)
        for (int j = 0; j < length; j++)
            if (layout[i][j] != prev[i][j])return true;

    return false;
}

vector<Laborer*> City::getAvailLaborers()
{
    vector<Laborer*> AvailLaborers, temp;
    vector<RZone*>::iterator it;

    for (it = rzones.begin(); it != rzones.end(); it++)
    {
        temp = (*it)->getLaborersAvailable();
        AvailLaborers.insert(AvailLaborers.end(), temp.begin(), temp.end());
    }

    return AvailLaborers;
}

vector<Product*> City::getSoldableProducts()
{
    vector<Product*> SoldableProducts, temp;
    vector<IZone*>::iterator it;

    for (it = izones.begin(); it != izones.end(); it++)
    {
        temp = (*it)->getProductsSoldable();
        SoldableProducts.insert(std::end(SoldableProducts), std::begin(temp), std::end(temp));
    }

    return SoldableProducts;
}

void City::DisperseContamination()
{
    vector<IZone*> ::iterator i1;
    int pollution_level, x, y;
    for (i1 = izones.begin(); i1 != izones.end(); i1++) {
        pollution_level = (*i1)->getPopulation();
        x = (*i1)->getX();
        y = (*i1)->getY();
        DisperseContaminationNode(pollution_level, x, y);
    }
}

void City::DisperseContaminationNode(int plevel, int x, int y)
{
    if (plevel == 0) return;

    for (int i = y - 1; i <= y + 1; i++) {
        for (int j = x - 1; j <= x + 1; j++) {
            if (i < 0 || j < 0 || i >= width || j >= length)continue;
            if (i == y && j == x && pollution[i][j] < plevel)
                pollution[i][j] = plevel;
            else
                DisperseContaminationNode(plevel - 1, j, i);
        }
    }
}

template<class Z>
void City::PriorityActivateation(vector<Z*> zones) {

    typename vector<Z*>::iterator itr;
    priority_queue<pair<int, Z*> > p_queue;

    for (itr = zones.begin(); itr != zones.end(); itr++) {
        p_queue.push(make_pair((*itr)->getPopulation(), (*itr)));
    }

    vector<Z*> temp;
    pair<int, Z*> tpair;
    while(p_queue.size() > 0){
        temp.push_back(p_queue.top().second);
        p_queue.pop();
        
        while (p_queue.size() > 0 && temp.front()->getPopulation() == p_queue.top().second->getPopulation()) {
            temp.push_back(p_queue.top().second);
            p_queue.pop();
        }

        PriorityActivateation2(temp);
        temp.clear();

    }
    
}

template<class Z>
void City::PriorityActivateation2(vector<Z*> zones) {
    typename vector<Z*>::iterator itr;
    priority_queue<pair<int, Z*> > p_queue;

    for (itr = zones.begin(); itr != zones.end(); itr++) {
        p_queue.push(make_pair((*itr)->getAdjPopulation(), (*itr)));
    }

    vector<Z*> temp;
    while (p_queue.size() > 0) {
        temp.push_back(p_queue.top().second);
        p_queue.pop();
        while (p_queue.size() > 0 && temp.front()->getAdjPopulation() == p_queue.top().second->getAdjPopulation()) {
            temp.push_back(p_queue.top().second);
            p_queue.pop();
        }

        PriorityActivateation3(temp);
        temp.clear();
    }
}

template<class Z>
void City::PriorityActivateation3(vector<Z*> zones) {
    typename vector<Z*>::iterator itr;
    typedef pair<int, Z*> px;
    priority_queue<px, vector<px>, greater<px> > p_queue;

    for (itr = zones.begin(); itr != zones.end(); itr++) {
        p_queue.push(make_pair((*itr)->getY(), (*itr)));
    }

    vector<Z*> temp;
    while (p_queue.size() > 0) {
        temp.push_back(p_queue.top().second);
        p_queue.pop();
        while (p_queue.size() > 0 && temp.front()->getY() == p_queue.top().second->getY()) {
            temp.push_back(p_queue.top().second);
            p_queue.pop();
        }

        PriorityActivateation4(temp);
        temp.clear();
    }
}

template<class Z>
void City::PriorityActivateation4(vector<Z*> zones) {
    typename vector<Z*>::iterator itr;
    typedef pair<int, Z*> py;
    priority_queue<py, vector<py>, greater<py> > p_queue;

    for (itr = zones.begin(); itr != zones.end(); itr++) {
        p_queue.push(make_pair((*itr)->getX(), (*itr)));
    }

    while (p_queue.size() > 0) {
        p_queue.top().second->Activate();
        p_queue.pop();
    }
}

bool City::ProcessStep(int step)
{
    char PreviousLayout[MAX_DIM][MAX_DIM];
    vector<RZone*>::iterator r1;
    vector<CZone*>::iterator c1;
    vector<IZone*>::iterator i1;

    for (int i = 0; i < width; i++)
        for (int j = 0; j < length; j++) {
            PreviousLayout[i][j] = layout[i][j];
        }

    for (r1 = rzones.begin(); r1 != rzones.end(); r1++)(*r1)->PROGRESS_STEP();
    for (r1 = rzones.begin(); r1 != rzones.end(); r1++)(*r1)->implement();

    PrepareLayout();
    if (!haschanged(PreviousLayout))return false;

    cout << "Time Step: " << step << endl;
    cout << "Available Laborers: " << getAvailLaborers().size();
    cout << " Available Products: " << getSoldableProducts().size() << endl;
    if (step % refresh_rate == 0) {
        display();
    }

    for (c1 = czones.begin(); c1 != czones.end(); c1++)(*c1)->PROGRESS_STEP();
    for (i1 = izones.begin(); i1 != izones.end(); i1++)(*i1)->PROGRESS_STEP();
    PriorityActivateation(czones);
    PriorityActivateation(izones);

    DisperseContamination();

    return haschanged(PreviousLayout);
}

int City::getRPopulation(int x1, int y1, int x2, int y2)
{
    typename vector<RZone*>::iterator it;
    int count = 0, x, y;

    for (it = rzones.begin(); it != rzones.end(); it++) {
        x = (*it)->getX();
        y = (*it)->getY();
        if (x >= x1 && x <= x2 && y >= y1 && y <= y2)
            count += (*it)->getPopulation();
    }
    
    return count;
}

int City::getCPopulation(int x1, int y1, int x2, int y2)
{
    typename vector<CZone*>::iterator it;
    int count = 0, x, y;

    for (it = czones.begin(); it != czones.end(); it++) {
        x = (*it)->getX();
        y = (*it)->getY();
        if (x >= x1 && x <= x2 && y >= y1 && y <= y2)
            count += (*it)->getPopulation();
    }

    return count;
}

int City::getIPopulation(int x1, int y1, int x2, int y2)
{
    typename vector<IZone*>::iterator it;
    int count = 0, x, y;

    for (it = izones.begin(); it != izones.end(); it++) {
        x = (*it)->getX();
        y = (*it)->getY();
        if (x >= x1 && x <= x2 && y >= y1 && y <= y2)
            count += (*it)->getPopulation();
    }

    return count;
}

int City::getTotalContamination(int x1, int y1, int x2, int y2) {
    int i, j;
    int count = 0;

    for (i = y1; i <= y2; i++) {
        for (j = x1; j <= x2; j++) {
            if (pollution[i][j] == 0)continue;
            count += pollution[i][j];
        }
    }

    return count;
}