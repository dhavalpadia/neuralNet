//
//  sig.c
//  flexNN
//
//  Created by Nihit Bhavsar on 3/15/18.
//  Copyright © 2018 Nihit Bhavsar. All rights reserved.
//

#include "decl.h"

double sigmoid(double in)
{
    return (1.0/(1.0+exp(-in)));
}
