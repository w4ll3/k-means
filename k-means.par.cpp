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
#include <numeric>
#include <boost/range/adaptor/indexed.hpp>
#include <pthread.h>

using namespace std;

vector<vector<int>> examples;
vector<vector<int>> centroids;
pthread_barrier_t barrier;
pthread_mutex_t lock;
int *iteration, num_threads;
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
            closest.at("id") = centroid.index() - 1;
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

    if (total)
        for (auto centroid_axis : new_centroid | boost::adaptors::indexed(1))
        {
            centroid_axis.value() /= total;
            if (centroid_axis.value() != centroid.at(centroid_axis.index() - 1))
                finished = false;
        }
    return total ? new_centroid : centroid;
}

void *k_means(void *t_id)
{
    int id = *((int *)t_id);
    int exampleStripe = examples.size() / num_threads;
    int exampleBegin = id * exampleStripe;
    int exampleEnd = exampleBegin + exampleStripe;

    int centroidStripe = centroids.size() / num_threads;
    int centroidBegin = id * centroidStripe;
    int centroidEnd = centroidBegin + centroidStripe;
    while (!finished)
    {
        for (int example = exampleBegin; example != exampleEnd; example++)
            examples.at(example).at((examples.at(example).size() - 1)) = closest_centroid(examples.at(example));
        finished = true;
        pthread_barrier_wait(&barrier);
        for (int centroid = centroidBegin; centroid != centroidEnd; centroid++)
            centroids.at(centroid) = recalculate(centroid, centroids.at(centroid));
        pthread_barrier_wait(&barrier);
        iteration[id] += 1;
    }
}

int main(int argc, char *argv[])
{
    string extracted, comma, basename(argv[1]);
    ifstream base, centroid;
    num_threads = atoi(argv[2]) - 1;
    iteration = (int *)calloc(sizeof(int) * num_threads, 0);
    int ids[num_threads];
    ids[0] = 0;
    pthread_t threads[num_threads];
    pthread_barrier_init(&barrier, NULL, num_threads);
    vector<int> temp;

    base.open("bases/int_base_" + basename + ".data");
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
    centroid.open("bases/int_centroid_" + basename + "_20.data");
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
    for (int i = 1; i < num_threads; i++)
    {
        ids[i] = i;
        pthread_create(&threads[i], NULL, k_means, (ids + i));
    }
    k_means(ids);
    for (int i = 1; i < num_threads; i++)
        pthread_join(threads[i], NULL);
    ofstream myfile("results/my_saida_" + basename + "_par");
    myfile << "numero de iteracoes:" << accumulate(iteration, iteration + num_threads, 0) << endl;
    for (auto value : examples | boost::adaptors::indexed(1))
        myfile << "id=" << value.index() - 1 << ", classe=" << value.value().at(value.value().size() - 1) << endl;
    pthread_barrier_destroy(&barrier);
    return 0;
}