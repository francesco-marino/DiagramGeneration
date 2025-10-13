#pragma once

int Initialize(int argc=-1, char **argv=NULL) ;
int Finalize() ;

void Barrier();
double Clock(void);
void GetRank(int &ntasks, int &rank);
