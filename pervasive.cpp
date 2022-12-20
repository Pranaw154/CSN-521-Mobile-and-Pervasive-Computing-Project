#include <bits/stdc++.h>

using namespace std;

const double PI = 3.1415926535897932384626;
const double speed_of_light = 3e8;
const int N = 500; // number of user equipments
const int B = 20; // number of base stations
const double thresold_distance = 2000; // maximum distance over which base stations can interfere

double transmit_power = 0.5;  // transmit power per user in watt
double frequency_of_signal = 5e9; // Band = 5 GHz
double noise = 5e-21; // default value of noise
double bitrate_bandwidth = 1e6; // shanon equation with 1 MHz bandwidth per user

// Base station class
class BS
{
private:
    int id;
    int x_cord;
    int y_cord;
    vector<int> associated_UE_id; // this array stores ids of user equipments connected with it

public:
    // setter and getter functions for base station class
    void set_X(int x)
    {
        x_cord = x;
    }

    void set_Y(int y)
    {
        y_cord = y;
    }

    void set_id(int z)
    {
        id = z;
    }

    void add_associated_UE(int UE_id)
    {
        associated_UE_id.push_back(UE_id);
    }

    int get_X()
    {
        return x_cord;
    }

    int get_Y()
    {
        return y_cord;
    }

    int get_id()
    {
        return id;
    }

    // prints the information for the Base station object
    void BS_information(){
        cout<<"***************************************************"<<"\n\n";
        cout<<"Base Station "<<id<<" is connected to: ";
        for(auto i: associated_UE_id) cout<<i<<" ";
        cout<<"\n\n";
    }
};

// User Equipment class
class UE
{
private:
    int id;
    int x_cord;
    int y_cord;
    int target_BS; // stores id of the target base station to which it connects
    double UE_SINR = 0;
    double UE_bitrate = 0;

public:
    // setter and getter functions for user equipment class
    void set_X(int x)
    {
        x_cord = x;
    }

    void set_Y(int y)
    {
        y_cord = y;
    }

    void set_id(int z)
    {
        id = z;
    }

    void set_targetBS(int t_bs)
    {
        target_BS = t_bs;
    }

    void set_SINR(double SINR)
    {
        UE_SINR = SINR;
    }

    void set_bitrate(double bitrate)
    {
        UE_bitrate = bitrate;
    }

    int get_X()
    {
        return x_cord;
    }

    int get_Y()
    {
        return y_cord;
    }

    int get_id()
    {
        return id;
    }

    int get_targetBS()
    {
        return target_BS;
    }

    double get_bitrate()
    {
        return UE_bitrate;
    }

    // free space path loss calculator function
    double calculate_fspl(double dis)
    {
        double tmp = 4 * PI * dis * frequency_of_signal / speed_of_light;
        return tmp * tmp;
    }

    // calculates the receiving power from a base station with distance 'dis'
    double calculate_receiving_power(double dis)
    {
        double FSPL = calculate_fspl(dis);
        double received_power = transmit_power / FSPL;
        return received_power;
    }

    // calculates bitrate
    double calculate_bitrate(double SINR)
    {
        double bitrate = bitrate_bandwidth * log2(1 + SINR);
        return bitrate;
    }

    // calculated distance of user equipment with a given base staion.
    double distance(BS &baseStation)
    {
        int bs_x = baseStation.get_X();
        int bs_y = baseStation.get_Y();

        int ue_x = this->x_cord;
        int ue_y = this->y_cord;

        int squared_dist = (bs_x - ue_x) * (bs_x - ue_x) + (bs_y - ue_y) * (bs_y - ue_y);

        double dist = sqrt((double)squared_dist * 1.0);
        return dist;
    }

    // computes distance of user equipment with each base station and sort them in increasing order
    void calculate_sorted_distance_with_BS(vector<pair<double, int>> &tmp_dis, BS baseStation[])
    {
        for (int j = 0; j < B; j++)
        {
            double dis = this->distance(baseStation[j]);
            tmp_dis.push_back({dis, j});
        }
        sort(tmp_dis.begin(), tmp_dis.end());
    }

    // calculates SINR
    double calculate_SINR(vector<pair<double, int>> &tmp_dis)
    {
        double power_received_from_targetBS = this->calculate_receiving_power(tmp_dis[0].first);

        double total_interference_power = 0;

        for (int i = 1; i < B; i++)
        {
            if (tmp_dis[i].first <= thresold_distance)
            {
                total_interference_power += this->calculate_receiving_power(tmp_dis[i].first);
            }
        }

        double interference_plus_noise = total_interference_power + noise;

        double SINR = power_received_from_targetBS / interference_plus_noise;

        return SINR;
    }

    // prints info about User equipment
    void UE_information(){
        cout<<"***************************************************"<<"\n\n";
        cout<<"User Equipment "<<id<<": \n";
        cout<<"Target Base Station = "<<target_BS<<"\n";
        cout<<"SINR = "<<UE_SINR<<"\n";
        cout<<"Bitrate = "<<UE_bitrate/(1e6*1.0)<<" Mbps\n\n";
    }
};

class System
{
public:
    UE userEquipment[N];
    BS baseStation[B];
    set<pair<int, int>> is_used_coordinate;
    // initialize user equipments
    void setup_user_equipments(UE userEquipment[])
    {
        for (int i = 0; i < N; i++)
        {
            userEquipment[i].set_id(i);
            int x_c = rand() % 5000;
            int y_c = rand() % 5000;

            // check if the current randomly generated coordinates already have a base station on it
            while(is_used_coordinate.find({x_c, y_c})!=is_used_coordinate.end()){
                x_c = rand() % 5000;
                y_c = rand() % 5000;
            }

            userEquipment[i].set_X(x_c);
            userEquipment[i].set_Y(y_c);
        }
    }

    // initialize base stations
    void setup_base_stations(BS baseStation[])
    {
        for (int i = 0; i < B; i++)
        {
            baseStation[i].set_id(i);
            int x_c = rand() % 5000;
            int y_c = rand() % 5000;
            
            // check if the current randomly generated coordinates already have a base station on it
            while(is_used_coordinate.find({x_c, y_c})!=is_used_coordinate.end()){
                x_c = rand() % 5000;
                y_c = rand() % 5000;
            }
            
            baseStation[i].set_X(x_c);
            baseStation[i].set_Y(y_c);
            is_used_coordinate.insert({x_c, y_c});
        }
    }

    // calculates syatem throughput
    double calculate_system_throughput()
    {
        double system_Throughput = 0;
        for (int i = 0; i < N; i++)
        {
            system_Throughput += userEquipment[i].get_bitrate();
        }
        return system_Throughput;
    }

    void simulate_for_userEquipment(BS baseStation[], UE &userEquipment)
    {
        vector<pair<double, int>> tmp_dis;
        userEquipment.calculate_sorted_distance_with_BS(tmp_dis, baseStation);

        userEquipment.set_targetBS(tmp_dis[0].second); // adding the target base station to the user equipment
        baseStation[tmp_dis[0].second].add_associated_UE(userEquipment.get_id()); // adding the user equipment to the array of associated UE in base station

        double SINR = userEquipment.calculate_SINR(tmp_dis);
        userEquipment.set_SINR(SINR);

        double bitrate = userEquipment.calculate_bitrate(SINR);
        userEquipment.set_bitrate(bitrate);

    }

    // information about simulation
    void print_result(){
        for (int i = 0; i < N; i++)
        {
            userEquipment[i].UE_information();
        }
        cout<<"***************************************************"<<"\n\n";
        cout<<"System Throughput = "<<System::calculate_system_throughput()/(1e6*1.0)<<" Mbps\n\n";

        for (int i = 0; i < B; i++)
        {
            baseStation[i].BS_information();
        }
        cout<<"***************************************************"<<"\n";
        
    }

    void system_simulation()
    {
        System::setup_base_stations(baseStation);
        System::setup_user_equipments(userEquipment);

        for (int i = 0; i < N; i++)
        {
            System::simulate_for_userEquipment(baseStation, userEquipment[i]);
        }

        System::print_result();
    }


};

int main()
{

    System celluler_network;
    celluler_network.system_simulation();

    return 0;
}