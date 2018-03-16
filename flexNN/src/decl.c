//
//  randomWeights.c
//  flexNN
//
//  Created by Nihit Bhavsar on 3/15/18.
//  Copyright © 2018 Nihit Bhavsar. All rights reserved.
//

#include <stdint.h>
#include "decl.h"


void randomizeWeights(HIDDENLAYER* higherLayer, HIDDENLAYER* lowerLayer)
{
    
    for(int j=0; j< lowerLayer->Neurons ; j++)           //
    {
        for(int k=0; k< higherLayer->Neurons; k++)         //
        {
            higherLayer->Theta[k][j] =  2*(rando()-0.5)*0.6;
            //printf("%f  %d%d\t", nn->HiddenLayer[i]->Theta[k][j],k,j);
            printf("%f  \t", higherLayer->Theta[k][j]);
        }
        
        printf("\n");
    }
    
}


void distributeWeights(NN* nn)
{
    
    for(int i=1; i < MAX_LAYERS; i++)
    {
        printf("Theta %d Matrix\n", i);
        HIDDENLAYER* higherLayer = nn->HiddenLayer[i];
        HIDDENLAYER* lowerLayer = nn->HiddenLayer[i-1];
        
        randomizeWeights(higherLayer,lowerLayer);
        
        printf("\n");
    }
    
}







