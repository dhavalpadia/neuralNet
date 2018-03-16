//
//  main.c
//  flexNN
//  Credits : Mr. Kutza
// g(z)=z/2(1+abs(z))+0.5              signmoid with 2 divides and no multiplication

/*
 STEPS :
 1. ALLOCATE MEMORY FOR EVERY MATRIX
 2. DISTRIBUTE RANDOM WEIGHTS
 3. INITIALIZE INPUTS
 4. CALCULATE INCOMING SIGNAL(SUM) FOR EACH NEURON
 5. CALCULATE OUTGOING SIGNAL(ACTICATION) FOR EACH NEURON
 6. FORWARD PROPAGATE ACTIVATION VALUES TO NEXT LAYER
 7. CALCULATE ERROR TOTAL (DESIRED - CURRENT)
 8. BACK PROPAGATE NEURON BY NEURON
 9. REPLACE WEIGHTS BY CORRECTED WEIGHTS
 10.TRAIN THE NETWORK!
 */



//#include "initInput.h"


#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include "decl.h"

#define MAX_LAYERS 5

const int Neurons[MAX_LAYERS] = {6, 5, 5, 3};             // Number of neurons in each layer


#define rando() ((double)rand()/((double)RAND_MAX+1))


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
    double          eta;                 //   learning rate
    double          gain;
    double*         outputLayer;         //   output layer
    double*         inputLayer;          //   input layer
    //double*         targetOutput;
}NN;*/



//const int Neurons[5] = {6, 5, 5, 3};             // Number of neurons in each layer

double target[3] = { 0.66, 0.99, 0.01};

double outputError[MAX_LAYERS-2];



void initRandoms()
{
    srand(2000);
}


double sigmoid(double in)
{
    return (1.0/(1.0+exp(-in)));
}


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




void initOutput()
{
    /*    NN* nn; */
    
    
    
    
}

void initInput()
{
    //readInput();
    NN* nn;
    printf("Input Vector\n");
    
    for(int j=1; j < nn->Inputsize; j++)
    {
        nn->HiddenLayer[0]->output[0] = 1;                           // BIAS FOR INPUT
        nn->HiddenLayer[0]->output[j] = 2*(rando()-0.5)*0.6;
        nn->HiddenLayer[0]->input[j] = nn->HiddenLayer[0]->output[j];
        printf("%f\n", nn->HiddenLayer[0]->output[j]);
    }
    
    
    
}



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

void print(double* a)
{
    
    for(int j=0; j<100; j++)
    {
        printf("\t%f",a[j]);
    }
    
    
}


void calcz(NN* nn, HIDDENLAYER* higherLayer, HIDDENLAYER* lowerLayer)
{
    
    double sum = 0.0;
    
    for(int c=0; c < higherLayer->Neurons; c++)
    {
        sum = 0.0;
        for(int r=0; r < lowerLayer->Neurons; r++)
        {
            
            sum += lowerLayer->output[r] * higherLayer->Theta[c][r];
            printf("\t%f", lowerLayer->output[r]);
            printf("\t%f\n", higherLayer->Theta[c][r]);
            
        }
        
        
        higherLayer->input[c] = sum;
        // printf("\t higher layer input : %f\n", higherLayer->input[c]);       // without activation
        
        
        higherLayer->output[c] = sigmoid(sum);
        
        printf("\t higher layer output : %f\n\n", higherLayer->output[c]);        // with activation
        
        
        // printf("\t lower layer input : %f\n", lowerLayer->input[c]);
        // printf("\t lower layer output : %f\n\n", lowerLayer->output[c]);
        
        
    }
}

void calcActivation(NN* nn)
{
    
    for(int i=1; i <= MAX_LAYERS-2; i++)
    {
        
        HIDDENLAYER* higherLayer = nn->HiddenLayer[i];
        HIDDENLAYER* lowerLayer = nn->HiddenLayer[i-1];
        
        //nn->HiddenLayer[i-1]->output[0] = 1;                       // bias to previous layer output
       // lowerLayer->output[0] = 1;
        if(i<MAX_LAYERS - 2){                            // bias to all layers except output
            //higherLayer->output[0] = 1;
        }
        
        calcz(nn,higherLayer,lowerLayer);
    }
}


void forwardPass()
{
    NN nn;
    initRandoms();
    allocateMem(&nn);           // Allocate memory for NN
    initInput();
    distributeWeights(&nn);             // Initialize inputs and randomize weights
    calcActivation(&nn);
    printf("\n prediction 1 : %f",nn.HiddenLayer[MAX_LAYERS-2]->output[0]);
    printf("\n prediction 2 : %f",nn.HiddenLayer[MAX_LAYERS-2]->output[1]);
    printf("\n prediction 3 : %f\n\n",nn.HiddenLayer[MAX_LAYERS-2]->output[2]);
}

void outputDelta(NN* nn)
{
    
    
    //printf("\nOUTPUT LAYER DELTA \n");
    for(int r=0; r < nn->HiddenLayer[MAX_LAYERS-2]->Neurons ; r++)
    {
        
        outputError[r] = 0.5*((nn->HiddenLayer[MAX_LAYERS-2]->output[r] - target[r]) * (nn->HiddenLayer[MAX_LAYERS-2]->output[r] - target[r]));
        
        //printf("node %d : %f\n",r+1, nn->HiddenLayer[MAX_LAYERS-2]->output[r]);
        printf("node %d ERROR : %f\n",r+1, outputError[r]);
        
        nn->HiddenLayer[MAX_LAYERS-2]->delta[r] = outputError[r] * (nn->HiddenLayer[MAX_LAYERS-2]->output[r]) * (1 - (nn->HiddenLayer[MAX_LAYERS-2]->output[r]));
        printf("node %d DELTA : %f\n\n",r+1, nn->HiddenLayer[MAX_LAYERS-2]->delta[r]);
        
    }
    
    printf("\n");
    
}

void changeWeights(HIDDENLAYER* higherLayer, HIDDENLAYER* lowerLayer)
{
    //for(int c =0; c < higherLayer)
    
    
}

void updateWeights(NN* nn)
{
    for(int i = 1; i < MAX_LAYERS; i++)
    {
        HIDDENLAYER* higherLayer = nn->HiddenLayer[i];
        HIDDENLAYER* lowerLayer = nn->HiddenLayer[i-1];
        changeWeights(higherLayer, lowerLayer);
        
    }
}

void calcDeltas(NN* nn, HIDDENLAYER* higherLayer, HIDDENLAYER* lowerLayer)
{
    
    double sumD = 0.0;
    
    
    
    for(int d=0; d < lowerLayer->Neurons; d++)
    {
        for(int c=0; c < higherLayer->Neurons; c++)
        {
           
            sumD += higherLayer->delta[c] * higherLayer->Theta[c][d];
            printf("\t%f", higherLayer->delta[c]);
            printf("\t%f\n", higherLayer->Theta[c][d]);
        }
        
       lowerLayer->delta[d] = lowerLayer->output[d] * (1- lowerLayer->output[d])  * sumD;
        //printf("\n%f", lowerLayer->output[d]);
        printf("\n%f", lowerLayer->delta[d]);
        
        
    }
    
}

void backprop(NN* nn)
{
    outputDelta(nn);
    for(int i=MAX_LAYERS-2; i>0; i--)
    {
        
        HIDDENLAYER* higherLayer = nn->HiddenLayer[i];
        HIDDENLAYER* lowerLayer = nn->HiddenLayer[i-1];
        calcDeltas(nn, higherLayer, lowerLayer);
        
        updateWeights(nn);
        
        
    }


        
        
    }
    
    
    




    
    







void backwardPass()
{
    NN nn;
    //outputError();
    
    backprop(&nn);
    
    
}


int main()
{
    
    
    forwardPass();
    backwardPass();
    
    
    
    
    
}




























