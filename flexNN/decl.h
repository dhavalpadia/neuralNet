//
//  decl.h
//  flexNN
//
//  Created by Nihit Bhavsar on 3/15/18.
//  Copyright © 2018 Nihit Bhavsar. All rights reserved.
//

#ifndef decl_h
#define decl_h

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>


#define MAX_LAYERS 5

#define rando() ((double)rand()/((double)RAND_MAX+1))


typedef struct
{
    int             Neurons;         // Neurons
    double*         input;           // Incoming weighted sum to each neuron
    double*         output;          // Outgoing activated signal from each neuron
    double*         delta;           // Error associated with each neuron
    double**        Theta;           // Weights
    double**        changeTheta;      // Weight change
} HIDDENLAYER;

typedef struct {
    HIDDENLAYER**   HiddenLayer;         //   layers of this net
    double          Inputsize;           //   Number of input neurons
    double          Outputsize;          //   Number of output neurons
    double          eta;                 //   learning rate
    double          gain;
    double*         outputLayer;         //   output layer
    double*         inputLayer;          //   input layer
    //double*         targetOutput;
}NN;

void distributeWeights(NN* nn);
void randomizeWeights(HIDDENLAYER* higherLayer, HIDDENLAYER* lowerLayer);
double sigmoid(double in);

#endif /* decl_h */
