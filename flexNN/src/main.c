// NIHIT BHAVSAR
//  main.c
//  flexNN
// g(z)=z/2(1+abs(z))+0.5              signmoid with 2 divides and no multiplication

// CAUTION :  HIDDEN LAYER NEURONS CAN NEVER EXCEED INPUT LAYER NEURONS

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include "decl.h"

#define MAX_LAYERS 5
#define rando() ((double)rand()/((double)RAND_MAX+1))
const int Neurons[MAX_LAYERS] = {4, 4, 4, 3};             // Number of neurons in each layer
const double target[3] = { 0.66, 0.99, 0.01};
double squaredError[MAX_LAYERS-2];

int irows = 150;
int icols = 4;

char* inputAddress = "/Users/nihit/Desktop/flexNN/X.txt";
char* outputAddress = "/Users/nihit/Desktop/flexNN/y.txt";


/************************************************************
            ALIGN STRUCTURES TO 4-BYTE BOUNDARY
 ************************************************************/

# pragma pack(4)

typedef struct
{
    int                      Neurons;         // Neurons
    volatile double*         input;           // Incoming weighted sum to each neuron
    volatile double*         output;          // Outgoing activated signal from each neuron
    volatile double*         delta;           // Error associated with each neuron
    volatile double**        Theta;           // Weights
    volatile double**        changeTheta;     // Weight change
} HIDDENLAYER;

typedef struct {
    HIDDENLAYER**            HiddenLayer;         //   layers of this net
    double                   errorTotal;
    volatile double*         outputLayer;         //   output layeR
    volatile double*         inputLayer;          //   input layer
    double                   Inputsize;           //   Number of input neurons
    double                   Outputsize;          //   Number of output neurons
    double                   eta;                 //   learning rate
    double                   gain;
    double                   momentum;            // size of steps taken towards minimum to avoid local minima
}NN;


/************************************************************
           INITIALIZE RANDOMS FOR DISTRIBUTION
 ************************************************************/

void initRandoms()
{
    srand(2000);
}

double sigmoid(double in)
{
    return (1.0/(1.0+exp(-in)));
}


/************************************************************
           ALLOCATE MEMORY TO WHOLE STRUCTURE
 ************************************************************/

void allocateMem(NN* nn)
{

    nn->Inputsize =  Neurons[0];
    nn->Outputsize = Neurons[MAX_LAYERS-2];
    nn->HiddenLayer =  (HIDDENLAYER**)malloc(sizeof(HIDDENLAYER*));
    nn->inputLayer =   (double*)calloc(nn->Inputsize, sizeof(double));
    nn->outputLayer =  (double*)calloc(nn->Outputsize, sizeof(double));
    
    for(int i=0; i<MAX_LAYERS; i++)
    {
        nn->HiddenLayer[i] =          (HIDDENLAYER*)malloc(sizeof(HIDDENLAYER));
        nn->HiddenLayer[i]->Neurons = Neurons[i];
        nn->HiddenLayer[i]->input =   (double*)calloc(Neurons[i], sizeof(double));
        nn->HiddenLayer[i]->output =  (double*)calloc(Neurons[i], sizeof(double));
        nn->HiddenLayer[i]->delta =   (double*)calloc(Neurons[i], sizeof(double));
        
        if(i!=0)            // except input
        {
            nn->HiddenLayer[i]->Theta = (volatile double**)calloc(Neurons[i], sizeof(double*));            // cols
            nn->HiddenLayer[i]->changeTheta = (volatile double**)calloc(Neurons[i], sizeof(double*));
            for(int j=0; j<Neurons[i-1]; j++)
            {
                nn->HiddenLayer[i]->Theta[j] = (volatile double*)calloc(Neurons[i-1],sizeof(double));                //  rows
                nn->HiddenLayer[i]->changeTheta[j] = (volatile double*)calloc(Neurons[i-1], sizeof(double));
            }
        }
       
    }
    
    nn->inputLayer = (double*)nn->HiddenLayer[0];
    nn->outputLayer = (double*)nn->HiddenLayer[MAX_LAYERS-2];
    
}

/***************************************************************************
                        INITIALIZE INPUTS
 ***************************************************************************/

double iMat[150][4];


void readInputs()
{
    int i;
    int j;
    
    
    /*matrix*/
    
    
    
    FILE *file;
    file=fopen(inputAddress, "r");
    printf("INPUT FILE \n");
    for(i = 0; i < irows; i++)
    {
        for(j = 0; j < icols; j++)
        {
            if (!fscanf(file, " %lf%*c", &iMat[i][j]))
                break;
           printf("%lf\t",iMat[i][j]);
        }
        printf("\n");
        
    }
    fclose(file);
    
    
}

void initInput(NN* nn)
{
    //readInput();
    
    
    printf("\nInput Vector\n");
    
    //for(int r=0; r <= irows; r++)
    {
        for(int c=0; c<= nn->Inputsize; c++)
        {
        
        nn->HiddenLayer[0]->output[0] = 1;                           // BIAS FOR INPUT
        
        nn->HiddenLayer[0]->output[c+1] = iMat[0][c];
            
        //nn->HiddenLayer[0]->output[c] =
        //nn->HiddenLayer[0]->input[j] = nn->HiddenLayer[0]->output[j];
        printf("%f\n", nn->HiddenLayer[0]->output[c]);
        
        }
    }
}

/***************************************************************************
                        INITIALIZE WEIGHTS
 ***************************************************************************/

void randomizeWeights(HIDDENLAYER* higherLayer, HIDDENLAYER* lowerLayer)
{
    
    for(int j=0; j< lowerLayer->Neurons ; j++)
    {
        for(int k=0; k< higherLayer->Neurons; k++)
        {
            higherLayer->Theta[k][j] =  2*(rando()-0.5)*0.6;
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

/**********************************************************************
                CALCULATE WEIGHTED SUMS AND ACTIVATIONS
 **********************************************************************/

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



/*********************************************************************
                    CALCULATE OUTPUT ERRORS
 *********************************************************************/

void outputDelta(NN* nn)
{

    for(int r=0; r < nn->HiddenLayer[MAX_LAYERS-2]->Neurons ; r++)
    {
        double error = nn->HiddenLayer[MAX_LAYERS-2]->output[r] - target[r];
        squaredError[r] = 0.5 * (error * error);
        printf("NODE %d ERROR : %f\n",r+1, squaredError[r]);
        
        nn->HiddenLayer[MAX_LAYERS-2]->delta[r] = squaredError[r] * (error * (1 - error));
        printf("NODE %d DELTA : %f\n\n",r+1, nn->HiddenLayer[MAX_LAYERS-2]->delta[r]);
        nn->errorTotal += squaredError[r];
    }
    printf("TOTAL ERROR : %f ", nn->errorTotal);
    printf("\n");
    
}

/*********************************************************************
                    CALCULATE GRADIENTS
 *********************************************************************/

void calcDeltas(NN* nn, HIDDENLAYER* higherLayer, HIDDENLAYER* lowerLayer)
{
    
    double sumD = 0.0;
    for(int d=0; d < lowerLayer->Neurons; d++)
    {
        printf("\n");
        for(int c=0; c < higherLayer->Neurons; c++)
        {
            sumD += higherLayer->delta[c] * higherLayer->Theta[c][d];
            printf("\t%f", higherLayer->delta[c]);
            printf("\t%f\n", higherLayer->Theta[c][d]);
        }
        
       lowerLayer->delta[d] = lowerLayer->output[d] * (1- lowerLayer->output[d])  * sumD;
        //printf("\n%f", lowerLayer->output[d]);
        printf("\nLOWER LAYER DELTA : %f\n", lowerLayer->delta[d]);
        
        
    }
    
}

/*********************************************************************
                        UPDATE WEIGHTS
 *********************************************************************/

void changeWeights(NN* nn, HIDDENLAYER* higherLayer, HIDDENLAYER* lowerLayer)
{
    
    for(int r =0; r < lowerLayer->Neurons; r++)
    {
        printf("\n\n\n");
         for(int c=0; c < higherLayer->Neurons; c++)
        {
            higherLayer->changeTheta[c][r] =  higherLayer->delta[c] -   higherLayer->Theta[c][r];    // add learning rate to delta

            printf("Weight : %f \n", higherLayer->Theta[c][r]);
            printf("Desired Weight : %f \n", higherLayer->changeTheta[c][r]);
            printf("delta : %f \n\n", higherLayer->delta[c]);
            higherLayer->Theta[c][r] =  higherLayer->changeTheta[c][r];                             // add momentum term
            //printf("changeWeight : %f \n\n", higherLayer->changeTheta[c][r]);
        }
    }
}

void updateWeights(NN* nn)
{
    for(int i = MAX_LAYERS-2; i > 0; i--)
    {
        HIDDENLAYER* higherLayer = nn->HiddenLayer[i];
        HIDDENLAYER* lowerLayer = nn->HiddenLayer[i-1];
        printf("\n\nLAYER %d \n",i);
        changeWeights(nn, higherLayer, lowerLayer);
    }
}


void backprop(NN* nn)
{
    
    for(int i=MAX_LAYERS-2; i>1; i--)
    {
        
        HIDDENLAYER* higherLayer = nn->HiddenLayer[i];
        HIDDENLAYER* lowerLayer = nn->HiddenLayer[i-1];
        calcDeltas(nn, higherLayer, lowerLayer);
    }
        updateWeights(nn);
}





void train()
{
    
    
    
}

void crossValidate()
{
    
    
    
    
}

void test()
{
    
    
    
}

/*********************************************************************
                INITIALIZING NETWORK ONLY ONCE
 *********************************************************************/

void initializeNetwork()                // Functions that run only once in the program
{
    NN nn;
    initRandoms();
    allocateMem(&nn);                   // Allocate memory for NN
    distributeWeights(&nn);             // Initialize inputs and randomize weights
    readInputs();
}

/*********************************************************************
                        FORWARD PROPAGATION
*********************************************************************/

void forwardPass()
{
    NN nn;
    
    initInput(&nn);
    calcActivation(&nn);
    
    for(int i=0; i<Neurons[MAX_LAYERS-2]; i++)
    {
        printf("\n Prediction %d : %f",i+1,nn.HiddenLayer[MAX_LAYERS-2]->output[i]);
    }
    
    
}

/*********************************************************************
                        BACKWARD PROPAGATION
 *********************************************************************/

void backwardPass()
{
    NN nn;
    outputDelta(&nn);
    backprop(&nn);
}






/*********************************************************************
                              MAIN
 *********************************************************************/
int main()
{
    
    initializeNetwork();            // FUNCTIONS THAT NEEDS TO RUN ONLY ONCE
    
    forwardPass();
    backwardPass();
    

    return 0;
}




























