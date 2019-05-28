#include <iostream>
#include <fstream>
#include <iostream>
#include <sstream>
#include <istream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <algorithm>
using namespace std;

const string bases[] = {"59", "161", "256", "1380", "1601"};
vector<vector<int>> examples;
vector<vector<int>> centroids;

void k_means()
{
    while (true)
    {
        for (auto example : examples)
            example.at(example.size() - 1) = closest_centroid();
        for (auto centroid : centroids)
        {
            count << find(examples.begin(), examples.end(), centroid);
        }
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
                    temp.push_back(stoi(extracted));
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
            stringstream is(extracted);
            while (getline(is, comma, ','))
                temp.push_back(stoi(extracted));
            centroids.push_back(temp);
            temp.clear();
        }
        centroid.close();
    }
    return 0;
}