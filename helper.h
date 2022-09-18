#include <iostream> //212 Стариков Илья
#include <string>
#include <vector>
#include <fstream>
#include <cmath>
#include <map>
#include <set>
#include <cstdlib>
#include <algorithm>
#include <numeric>
using namespace std;

static int id = 0;
static int pid = 0;
static int cid = 0;
static int clustid = 0;

class point
{
public:
    double x, y;
    int id_point;
    string metka = "dust";
    int tag = 0;
    int mark = 0;
    point(double xx, double yy);
    point(const point& p);
    point(point* p);
};


class cloud
{
public:
    int N, Id;
    vector<point*> points;
    cloud(double mX, double mY, double sX, double sY, int Np);
    cloud();
    ~cloud();
};

class field
{
public:
    int Np, Nc, state = 1;
    vector<cloud*> clouds;
    vector<point*> all_points;
    vector<cloud*> clusters;
    double** D;
    double maxD;
    field();
    ~field();
};

class formal_element
{
public:
    vector<point*> elements;
    formal_element(int k); //will be used later
    ~formal_element();
};

class cluster
{
public:
    int Nclusters;
    string type;
    vector<point*> Single_Cluster;
    cluster();
    ~cluster();
};

class Find_Cluster
{
public:
    int Ncl;
    vector<int**> matrixB;
    vector<cluster*> Cluster;
    vector<formal_element*> Formal_elements;
    Find_Cluster();
    ~Find_Cluster();
};


class controller
{
public:
    ofstream log_controller;
    field* Field{};
    Find_Cluster* Find_Clusters{};
    controller();
    void writeLog(const string& message);
    void INIT();
    void INFO();
    void GEN_CLOUD(double mX, double mY, double sX, double sY, int N);
    ~controller();
};


class interface
{
public:
    int ID;
    controller Controller;
    vector<string> comands;
    ofstream log_interface;
    vector<string> instructions;
    interface();
    ~interface();
    void writeLog(const string& message);
    void ReadCommand();
    void Fill();
    int CONTROLLER(string Command);
};

double dist(point first, point second);
double srednee(vector<double> z);
vector<point*> K_MEANS_function(int k, vector<point> all_points); //will be used later
