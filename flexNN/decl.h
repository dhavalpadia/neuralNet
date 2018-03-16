//
//  decl.h
//  flexNN
//
//  Created by Nihit Bhavsar on 3/15/18.
//  Copyright © 2018 Nihit Bhavsar. All rights reserved.
//

#ifndef decl_h
#define decl_h




/*typedef struct
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
    double          errorTotal;
    double          eta;                 //   learning rate
    double          gain;
    double*         outputLayer;         //   output layer
    double*         inputLayer;          //   input layer
    //double*         targetOutput;
}NN;*/

//void distributeWeights(NN* nn);
//void randomizeWeights(HIDDENLAYER* higherLayer, HIDDENLAYER* lowerLayer);
//double sigmoid(double in);
//void allocateMem(NN* nn);



#endif /* decl_h */
