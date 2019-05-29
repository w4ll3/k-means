#include <iostream>
#include <fstream>
#include <iostream>
#include <sstream>
#include <istream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <map>
#include <cmath>
#include <algorithm>
#include <boost/range/adaptor/indexed.hpp>
using namespace std;

const string bases[] = {"59", "161", "256", "1380", "1601"};
vector<vector<int>> examples;
vector<vector<int>> centroids;
int iteration = 0;
bool finished = false;

int closest_centroid(vector<int> example)
{
    map<string, int> closest = {{"id", -1}, {"distance", INT_MAX}};
    for (auto centroid : centroids | boost::adaptors::indexed(1))
    {
        long long int distance = 0;
        for (auto axis : example | boost::adaptors::indexed(1))
        {
            if (axis.index() - 1 == centroid.value().size())
                break;
            int centroid_point = centroid.value().at(axis.index() - 1);
            int axis_value = axis.value();
            int sub = centroid_point - axis_value;
            distance += pow(sub, 2);
        }
        distance = sqrt(distance);
        if (distance < closest.at("distance"))
        {
            closest.at("id") = centroid.index();
            closest.at("distance") = distance;
        }
    }
    return closest.at("id");
}

vector<int> recalculate(int centroid_id, vector<int> centroid)
{
    int total = 0;
    vector<int> new_centroid(examples.at(0).size() - 1, 0);
    for (auto example : examples)
    {
        if (example.at(example.size() - 1) == centroid_id)
        {
            for (auto example_axis : example | boost::adaptors::indexed(1))
            {
                if (example_axis.index() - 1 == new_centroid.size())
                    break;
                new_centroid.at(example_axis.index() - 1) += example_axis.value();
            }
            total++;
        }
    }

    if (total)
        for (auto &centroid_axis : new_centroid)
            centroid_axis /= 60;

    for (auto axis : new_centroid | boost::adaptors::indexed(1))
        if (axis.value() != centroid.at(axis.index() - 1))
        {
            finished = false;
            break;
        }
    return total ? new_centroid : centroid;
}

void k_means()
{
    while (!finished)
    {
        for (auto &example : examples)
            example.at(example.size() - 1) = closest_centroid(example);
        for (auto centroid : centroids | boost::adaptors::indexed(1))
            centroid.value() = recalculate(centroid.index() - 1, centroid.value());

        iteration++;
        cout << iteration << endl;
    }
}

int main(int argc, char *argv[])
{
    string extracted, comma;
    ifstream base, centroid;
    for (int i = 0; i < 1; i++)
    {
        vector<int> temp;

        base.open("bases/int_base_" + bases[i] + ".data");
        while (!base.eof())
        {
            getline(base, extracted);
            stringstream is(extracted);
            if (extracted.size() > 0)
            {
                while (getline(is, comma, ','))
                    temp.push_back(stoi(comma));
                temp.push_back(-1);
                examples.push_back(temp);
                temp.clear();
            }
        }
        base.close();
        centroid.open("bases/int_centroid_" + bases[i] + "_20.data");
        while (!centroid.eof())
        {
            getline(centroid, extracted);
            istringstream is(extracted);
            if (extracted.size() > 0)
            {
                while (getline(is, comma, ','))
                    temp.push_back(stoi(comma));
                centroids.push_back(temp);
                temp.clear();
            }
        }
        centroid.close();
        k_means();
    }
    return 0;
}