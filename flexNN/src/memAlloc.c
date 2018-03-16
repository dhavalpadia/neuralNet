//
//  memAlloc.c
//  flexNN
//
//  Created by Nihit Bhavsar on 3/15/18.
//  Copyright © 2018 Nihit Bhavsar. All rights reserved.
//

#include "decl.h"


//const int Neurons[5] = {6, 5, 5, 3};

void allocateMem(NN* nn)
{
    
    nn->Inputsize =  Neurons[0];
    nn->Outputsize = Neurons[MAX_LAYERS-2];
    
    nn->HiddenLayer =  (HIDDENLAYER**)calloc(MAX_LAYERS, sizeof(HIDDENLAYER*));  //Declaring row pointers for all layers
    nn->inputLayer =   (double*)calloc(nn->Inputsize, sizeof(double));
    nn->outputLayer =  (double*)calloc(nn->Outputsize, sizeof(double));
    
    // nn->targetOutput = (double*)calloc(Neurons[MAX_LAYERS-2], sizeof(double));
    
    for(int i=0; i<MAX_LAYERS; i++)
    {
        nn->HiddenLayer[i] =          (HIDDENLAYER*)malloc(sizeof(HIDDENLAYER));
        nn->HiddenLayer[i]->Neurons = Neurons[i];
        nn->HiddenLayer[i]->input =   (double*)calloc(Neurons[i], sizeof(double));
        nn->HiddenLayer[i]->output =  (double*)calloc(Neurons[i], sizeof(double));
        //nn->HiddenLayer[i]->Error =   (double*)calloc(Neurons[i], sizeof(double));
        nn->HiddenLayer[i]->delta =   (double*)calloc(Neurons[i], sizeof(double));
        
        if(i!=0)            // except input
        {
            nn->HiddenLayer[i]->Theta = (double**)calloc(Neurons[i], sizeof(double*));            // cols
            nn->HiddenLayer[i]->changeTheta = (double**)calloc(Neurons[i], sizeof(double*));
            for(int j=0; j<Neurons[i-1]; j++)
            {
                nn->HiddenLayer[i]->Theta[j] = (double*)calloc(Neurons[i-1],sizeof(double));      //  rows
                nn->HiddenLayer[i]->changeTheta[j] = (double*)calloc(Neurons[i-1], sizeof(double));
            }
        }
        
    }
    
    nn->inputLayer = (double*)nn->HiddenLayer[0];
    //nn->Inputs = (double*)nn->HiddenLayer[1]->Input;
    nn->outputLayer = (double*)nn->HiddenLayer[MAX_LAYERS-2];
    
}


