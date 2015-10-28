#include "predictor.h"




void init_predictor ()
{	
	for(int i = 0; i < MAX_TABLE_SIZE; ++i){
		for(int j = 0; j < PATTERN_SIZE; ++j){
			pb_arr[i][j] = 2;	
		}
	}
	for(int i = 0; i < MAX_TABLE_SIZE; ++i){
		hist_arr[i] = 0;
	}
	for(int i = 0; i < MAX_GLOBAL_SIZE; ++i){
		bit_arr[i] = 2;
	}
	for(int i = 0; i < PREDICT_SIZE; ++i){
		vote_arr[i] = 1;
	}
	shift_hist = 0;
}

bool make_prediction (unsigned int pc)
{
	
	int i = pc & PREDICT_MASK;
	bool l_or_g; // false is Local, true is Gshare
	if(vote_arr[i] == 2 || vote_arr[i] == 3){
		l_or_g = true;
	}
	else if(vote_arr[i] == 0 || vote_arr[i] == 1){
		l_or_g = false;
	}
	
	if(!l_or_g){
		//Local 
		int index1 = pc & LOCAL_TABLE_MASK;
		int index2 = hist_arr[index1] & LOCAL_PATTERN_MASK;
		int local_res = pb_arr[index1][index2];
		if(local_res == 0||local_res == 1){
			return false;
		}
		else if(local_res == 2||local_res == 3){
			return true;
		}
	}
	else{
		//Global
		int index = (shift_hist ^ pc) & GLOBAL_MASK;
		int global_res = bit_arr[index];
		if(global_res == 0||global_res == 1){
			return false;
		}
		else if(global_res == 2||global_res == 3){
			return true;
		}
	}

	return false;

}

void train_predictor (unsigned int pc, bool outcome)
{
	bool is_local_right, is_global_right;

	//Local
	int index1 = pc & LOCAL_TABLE_MASK;
	int index2 = hist_arr[index1] & LOCAL_PATTERN_MASK;
	int local_res = pb_arr[index1][index2];
	if(local_res == 0){
		if(outcome){
			pb_arr[index1][index2] = 1;
			is_local_right = false;
		}
		else{
			is_local_right = true;
		}
	}
	else if(local_res == 1){
		if(outcome){
			pb_arr[index1][index2] = 2;
			is_local_right = false;
		}
		else{
			pb_arr[index1][index2] = 0;
			is_local_right = true;
		}
	}
	else if(local_res == 2){
		if(outcome){
			pb_arr[index1][index2] = 3;
			is_local_right = true;
		}
		else{
			pb_arr[index1][index2] = 1;
			is_local_right = false;
		}
	}
	else if(local_res == 3){
		if(!outcome){
			pb_arr[index1][index2] = 2;
			is_local_right = false;
		}
		else{
			is_local_right = true;
		}			
	}
	hist_arr[index1] = (hist_arr[index1]<<1)|outcome;
	
	//Global
	int index = ( shift_hist ^ pc ) & GLOBAL_MASK;
	int global_res = bit_arr[index];

	if(global_res == 0){
		if(outcome){
			bit_arr[index] = 1;
			is_global_right = false;
		}
		else{
			is_global_right = true;
		}
	}
	else if(global_res == 1){
		if(outcome){
			bit_arr[index] = 2;
			is_global_right = false;
		}
		else{
			bit_arr[index] = 0;
			is_global_right = true;
		}
	}
	else if(global_res == 2){
		if(outcome){
			bit_arr[index] = 3;
			is_global_right = true;
		}
		else{
			bit_arr[index] = 1;
			is_global_right = false;
		}
	}
	else if(global_res == 3){
		if(!outcome){
			bit_arr[index] = 2;
			is_global_right = false;
		}	
		else{
			is_global_right = true;
		}		
	}
	
	shift_hist = (shift_hist<<1)| outcome;
	
	//predictor to predictor
	//false is local, true is global
	int i = pc & PREDICT_MASK;

	if(!is_local_right && is_global_right){
		if(vote_arr[i] == 3){

		}
		else{
			++vote_arr[i];
		}
	}
	else if(is_local_right && !is_global_right){
		if(vote_arr[i] == 0){

		}
		else{
			--vote_arr[i];
		}
	}

	return;
}
