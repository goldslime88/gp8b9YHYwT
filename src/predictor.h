#include <stdio.h>
#include <stdint.h>
#ifndef PREDICTOR_H
#define PREDICTOR_H

/*
  Define all your tables and their sizes here.
  All tables must be statically-sized.
  Please don't call malloc () or any of its relatives within your
  program.  The predictor will have a budget, namely (32K + 256) bits
  (not bytes).  That encompasses all storage (all tables, plus GHR, if
  necessary -- but not PC) used for the predictor.  That may not be
  the amount of storage your predictor uses, however -- for example,
  you may implement 2-bit predictors with a table of ints, in which
  case the simulator will use more memory -- that's okay, we're only
  concerned about the memory used by the simulated branch predictor.
*/

#define MAX_TABLE_SIZE 512
#define PATTERN_SIZE 16
#define LOCAL_TABLE_MASK 0x01FF
#define LOCAL_PATTERN_MASK 0x000F

#define MAX_GLOBAL_SIZE 4096
#define G_LENGTH 12
#define GLOBAL_MASK 0x0FFF

#define PREDICT_SIZE 2048
#define PREDICT_LENGTH 11
#define PREDICT_MASK 0x07FF


//Local Predictor
static uint8_t hist_arr[MAX_TABLE_SIZE];
static uint8_t pb_arr[MAX_TABLE_SIZE][PATTERN_SIZE]; 

//Global Predictor
static int shift_hist;
static uint8_t bit_arr[MAX_GLOBAL_SIZE];

//Predictor to predictor
//0,1->Local, 2,3->Gshare
static uint8_t vote_arr[PREDICT_SIZE];




/*
  Initialize the predictor.
*/
void init_predictor ();

/*
  Make a prediction for conditional branch instruction at PC 'pc'.
  Returning true indicates a prediction of taken; returning false
  indicates a prediction of not taken.
*/
bool make_prediction (unsigned int pc);

/*
  Train the predictor the last executed branch at PC 'pc' and with
  outcome 'outcome' (true indicates that the branch was taken, false
  indicates that the branch was not taken).
*/
void train_predictor (unsigned int pc, bool outcome);

#endif
