#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <iomanip>
#include <sstream>
#include <cmath>
using namespace std;

int main(int argc, char* argv[]) {

    string iterations = argv[1];
    string initial_value = argv[2];
    ifstream myfile (argv[3]);

    int iter = atoi(iterations.c_str());
    double error;
    if(iter == 0) {
        error = pow(0.1, 5);
        cout << setprecision(7) << "error set to: " << error << endl;
    }
    if(iter < 0) {
        error = pow(0.1, -iter);
        cout << setprecision(7) << "error set to: " << error << endl;
    }

    int init_value = atoi(initial_value.c_str());
    int num_vertices;
    int num_edges;
    float d = 0.85;

    string x;
    myfile >> x;
    num_vertices = atoi(x.c_str());
    cout << "num_vertices = " << num_vertices << endl;
    if(num_vertices > 10){
        init_value = -1;
        error = 0.00001;
        iter = 0;
    }

    float sqrt_n = sqrt(num_vertices);

    myfile >> x;
    num_edges = atoi(x.c_str());
    cout << "num_edges = " << num_edges << endl;
    vector<vector<int> > lists;
    lists.reserve(num_vertices);

    //Initialize the source vector according to init_value
    vector<float> src;
    if(init_value == 0){
        for(int i = 0; i < num_vertices; i++)
            src.push_back(0);
    }
    if(init_value == 1){
        for(int i = 0; i < num_vertices; i++)
            src.push_back(1);
    }
    if(init_value == -1){
        for(int i = 0; i < num_vertices; i++)
            src.push_back(float(1)/float(num_vertices));
    }
    if(init_value == -2){
        for(int i = 0; i < num_vertices; i++)
            src.push_back(1/sqrt_n);
    }

    //Convert input into adjacency list
    for(int i=0; i < num_edges; i++){
        int index1,index2;
        myfile >> x;
        index1 = atoi(x.c_str());
        myfile >> x;
        index2 = atoi(x.c_str());

        lists[index1].push_back(index2);
    }
/*
    //printing the adjacency list
    for(int i = 0; i < num_vertices; i++) {
        cout << i << ": ";
        for (int j = 0; j < lists[i].size(); j++)
            cout << lists[i][j] << " ";
        cout << endl;
    }
*/
    if(iter > 0) {
        for (int it = 0; it <= iter; it++) {
            vector<float> D;
            for (int n = 0; n < num_vertices; n++)
                D.push_back(0);

            for (int i = 0; i < num_vertices; i++) {
                vector<int> pointing_list;
                vector<int> outdegrees;

                for (int j = 0; j < num_vertices; j++) {
                    vector<int> adjlist1 = lists[j];

                    for (int k = 0; k < adjlist1.size(); k++) {
                        ostringstream str1;
                        ostringstream str2;
                        str1 << adjlist1[k];
                        string x1 = str1.str();
                        str2 << i;
                        string x2 = str2.str();

                        if (x1.compare(x2) == 0) {
                            pointing_list.push_back(j);
                            outdegrees.push_back(adjlist1.size());
                            //cout << j << " added to " << "the pointing list of " << i << endl;
                        }
                    }
                }
                for (int p = 0; p < pointing_list.size(); p++)
                    D[i] += float(src[pointing_list[p]]) / float(outdegrees[p]);
            }

            for (int v = 0; v < num_vertices; v++) {
                D[v] = d * D[v] + (1 - d) / float(num_vertices);
            }

            if(num_vertices <= 10) {
                cout << "Iter:" << it << " :";
                for (int i = 0; i < src.size(); i++)
                    cout << setprecision(7) << "P[" << i << "] = " << src[i] << " ";
                cout << endl;
            }
            if(num_vertices > 10 && it == iter){
                cout << "Iter:" << it << endl;
                for (int i = 0; i < src.size(); i++)
                    cout << setprecision(7) << "P[" << i << "] = " << src[i] << endl;
            }
            src = D;
        }
    }
    else {
        int stop = 0;
        int iters = 0;
        while (stop == 0) {
            vector<float> D;
            for (int n = 0; n < num_vertices; n++)
                D.push_back(0);

            for (int i = 0; i < num_vertices; i++) {
                vector<int> pointing_list;
                vector<int> outdegrees;

                for (int j = 0; j < num_vertices; j++) {
                    vector<int> adjlist1 = lists[j];

                    for (int k = 0; k < adjlist1.size(); k++) {
                        ostringstream str1;
                        ostringstream str2;
                        str1 << adjlist1[k];
                        string x1 = str1.str();
                        str2 << i;
                        string x2 = str2.str();

                        if (x1.compare(x2) == 0) {
                            pointing_list.push_back(j);
                            outdegrees.push_back(adjlist1.size());
                            //cout << j << " added to " << "the pointing list of " << i << endl;
                        }
                    }
                }
                for (int p = 0; p < pointing_list.size(); p++)
                    D[i] += float(src[pointing_list[p]]) / float(outdegrees[p]);
            }

            for (int v = 0; v < num_vertices; v++) {
                D[v] = d * D[v] + (1 - d) / float(num_vertices);
            }

            if(num_vertices <= 10) {
                cout << "Iter:" << iters << " :";
                for (int i = 0; i < src.size(); i++)
                    cout << setprecision(7) << "P[" << i << "] = " << src[i] << " ";
                cout << endl;
            }

            int checker = 1;
            for(int n = 0; n < num_vertices; n++) {
                float diff = fabs(D[n] - src[n]);
                if (diff >= error) {
                    checker = 0;
                }
            }

            if(checker == 1) {
                stop = 1;
                if(num_vertices <= 10 ){
                    cout << "Iter:" << iters + 1 << " :";
                    for (int i = 0; i < src.size(); i++)
                        cout << setprecision(7) << "P[" << i << "] = " << D[i] << " ";
                    cout << endl;
                }
                if(num_vertices > 10) {
                    cout << "Iter:" << iters + 1 << endl;
                    for (int i = 0; i < src.size(); i++)
                        cout << setprecision(7) << "P[" << i << "] = " << D[i] << endl;
                }
            }
            src = D;
            iters += 1;
        }
    }
}