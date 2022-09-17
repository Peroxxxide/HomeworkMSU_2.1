#include "helper.h"
#define E 0.00001

field::field() // field's constructor
{
    Np = 0; //number of points (changeable)
    Nc = 0; //number of clouds (changeable)
}

field::~field() //field's destructor
{
    for (auto& el : clouds) delete el;
}

point::point(double xx, double yy) //point's constructor
{
    x = xx;
    y = yy;
    id_point = ++pid;
}

point::point(const point& p)
{
    x = p.x;
    y = p.y;
    tag = p.tag;
    metka = p.metka;
    mark = p.mark;
}

point::point(point* p)
{
    x = p->x;
    y = p->y;
    tag = p->tag;
    metka = p->metka;
    mark = p->mark;
}

cloud::cloud(double mX, double mY, double sX, double sY, int Np)
{
    cid++;
    Id = cid;
    N = Np;
    double tempx = 0, tempy = 0, constant = 0;
    srand(time_t(NULL));
    for (int i = 0; i < Np * 10; i++)
    {
        int t = rand();
        if ((t % 4) > 1) constant += (double)t * (-1) / (Np / 2);
        else constant += (double)t / (Np / 2);
    }
    for (int i = 0; i < Np; i++)
    {
        for (int j = 0; j < Np * 3; j++)
        {
            int t = rand();
            if ((t % 4) > 1) tempx += (double)t * (-1) / constant;
            else tempx += (double)t / constant;
            t = rand();
            if ((t % 4) > 1) tempy += (double)t * (-1) / constant;
            else tempy += (double)t / constant;
        }
        point* Pnt = new point(mX + (tempx / (3 * Np)) * sX, mY + (tempy / (3 * Np)) * sY);
        points.push_back(Pnt);
        tempx = 0;
        tempy = 0;
    }
}

cloud::cloud()
{
    cid++;
    Id = cid;
    N = 0;
}

cloud::~cloud() //cloud's destructor
{
    for (auto& el : points) delete el;
}


Find_Cluster::Find_Cluster()
{
    Ncl = 0;
    matrixB = {};
}

Find_Cluster::~Find_Cluster()
{
    for (auto& el : matrixB) delete el;
}

cluster::cluster()
{
    clustid++;
    Nclusters = clustid;
    Single_Cluster = {};
}

cluster::~cluster()
{
    for (auto& el : Single_Cluster) delete el;
}

interface::interface()
{
    log_interface.open("/home/ilya/CLionProjects/untitled10/log_interface.txt", std::ofstream::out | std::ofstream::trunc);
    log_interface.close();
    log_interface.open("/home/ilya/CLionProjects/untitled10/log_interface.txt", ios_base::app);
    id++;
    ID = id;
}

interface::~interface()
{
    id--;
}


controller::controller()
{
    log_controller.open("/home/ilya/CLionProjects/untitled10/log_controller.txt", std::ofstream::out | std::ofstream::trunc);
    log_controller.close();
    log_controller.open("/home/ilya/CLionProjects/untitled10/log_controller.txt", ios_base::app);
}

controller::~controller() //controller's destructor
{
    delete Field;
}

void interface::writeLog(const string& message)
{
    log_interface << " "  << message << endl;
}

void interface::ReadCommand() //scan command from file to vector instruction
{
    ifstream in("/home/ilya/CLionProjects/untitled10/instruction.txt");
    if (!in) return;
    string temp;
    while (!in.eof())
    {
        getline(in, temp);
        instructions.push_back(temp);
    }
    in.close();
    writeLog("scanning file command");
    cout << "scanning command from file to vector of instruction" << endl;
}

void controller::writeLog(const string& message) //writes log for controller
{
    log_controller << " " << message << endl;
}

void controller::GEN_CLOUD(double mX, double mY, double sX, double sY, int N) // generation of clouds
{
    Field->Nc++;
    Field->Np += N;
    cloud* New = new cloud(mX, mY, sX, sY, N);
    for (auto& el : New->points) Field->all_points.push_back(el); //placeholder for all points
    Field->clouds.push_back(New); //placeholder for all clouds
    writeLog("generating clouds");
    cout << "generating clouds" << endl;
}

void interface::Fill() // creation of command vector
{
    ifstream in("/home/ilya/CLionProjects/untitled10/help.txt");
    string temp;
    while (!in.eof())
    {
        getline(in, temp);
        comands.push_back(temp);
    }
    in.close();
    writeLog("creating accessible command vector");
    cout << "creating accessible command vector" << endl;
}

int interface::CONTROLLER(string Command) //commands recognition center for controller
{
    writeLog("controller recognised " + Command);
    if (Command == "INIT") Controller.INIT();
    if (Command == "INFO") Controller.INFO();

    if (Command.find("GC") == 0)
    {
        if (Controller.Field->state)
        {
            while (Command.find(".") != string::npos) Command.replace(Command.find("."), 1, ",");
            double mX, mY, sX, sY, temp;
            int N;
            Command += " ";
            Command.erase(0, 3);
            mX = stod(Command.substr(0, Command.find(" ")));
            Command.erase(0, Command.find(" ") + 1);
            mY = stod(Command.substr(0, Command.find(" ")));
            Command.erase(0, Command.find(" ") + 1);
            sX = stod(Command.substr(0, Command.find(" ")));
            Command.erase(0, Command.find(" ") + 1);
            sY = stod(Command.substr(0, Command.find(" ")));
            Command.erase(0, Command.find(" ") + 1);
            temp = stod(Command.substr(0, Command.find(" ")));
            if ((double)((int)temp) != temp) { writeLog("ERROR " + Command + "! ERROR in log_mistake.txt");  return 0; }
            N = (int)temp;
            if (N < 1) return 0;
            Controller.GEN_CLOUD(mX, mY, sX, sY, N);
        }
        else { writeLog(" ERROR " + Command + "! ERROR in log_mistake.txt");  return 2; }
    }

}

void controller::INIT() //
{
    Field = new field;
    Find_Clusters = new Find_Cluster;
    writeLog("controller recognised INIT");
    cout << "controller recognised INIT" << endl;
}

void controller::INFO() // put points into file "cloud"
{
    for (auto& el : (*Field).clouds)
    {
        ofstream out("/home/ilya/CLionProjects/untitled10/cloud" + to_string(el->Id) + ".txt");
        out.precision(6);
        for (auto& points : el->points) out << points->x << "\t" << points->y << endl;
        out.close();
        writeLog("controller recognised INFO, output of points in the file \"cloud\" ");
        cout << "controller recognised INFO, output of points in the file \"cloud\" " << endl;
    }
}

double dist(point first, point second)
{
    return sqrt((first.x - second.x) * (first.x - second.x) + (first.y - second.y) * (first.y - second.y));
}

double srednee(vector<double> z)
{
    return accumulate(z.begin(), z.end(), 0.0) / z.size();
}