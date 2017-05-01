#include <stdio.h>
#include <stdlib.h>
#include "math.h"
#include "memory.h"

typedef unsigned long long uint64;

uint64 trail_number = 0;
long double overall_bias = 0;
long double active_sbox1 = 0;
long double progress_number = 0;

#define TRAILFILE "Trails.txt"

#define SBOX_TO_UINT64(a) ( ((uint64)((a)[0]&0x0f) << 60) | ((uint64)((a)[1]&0x0f) << 56) | ((uint64)((a)[2]&0x0f) << 52) | ((uint64)((a)[3]&0x0f) << 48) | ((uint64)((a)[4]&0x0f) << 44) | ((uint64)((a)[5]&0x0f) << 40) | ((uint64)((a)[6]&0x0f) << 36) | ((uint64)((a)[7]&0x0f) << 32) |\
	((uint64)((a)[8]&0x0f) << 28) | ((uint64)((a)[9]&0x0f) << 24) | ((uint64)((a)[10]&0x0f) << 20) | ((uint64)((a)[11]&0x0f) << 16) | ((uint64)((a)[12]&0x0f) << 12) | ((uint64)((a)[13]&0x0f) << 8) | ((uint64)((a)[14]&0x0f) << 4) | ((uint64)((a)[15]&0x0f) << 0)  )


#define ARRAY_TO_UINT64(a) ( ((uint64)((a)[0]&0xff) << 56) | ((uint64)((a)[1]&0xff) << 48) | ((uint64)((a)[2]&0xff) << 40) | ((uint64)((a)[3]&0xff) << 32) |\
	((uint64)((a)[4]&0xff) << 24) | ((uint64)((a)[5]&0xff) << 16) | ((uint64)((a)[6]&0xff) << 8) | ((uint64)((a)[7]&0xff) << 0) )


#define UINT64_TO_ARRAY(i, a) do{ \
	(a)[0] = ((i)>>56) & 0xff; \
	(a)[1] = ((i)>>48) & 0xff; \
	(a)[2] = ((i)>>40) & 0xff; \
	(a)[3] = ((i)>>32) & 0xff; \
	(a)[4] = ((i)>>24) & 0xff; \
	(a)[5] = ((i)>>16) & 0xff; \
	(a)[6] = ((i)>> 8) & 0xff; \
	(a)[7] = ((i)>> 0) & 0xff; \
}while(0);


//sbox
const unsigned char sbox[16] = { 0xC,0x5,0x6,0xB,0x9,0x0,0xA,0xD,0x3,0xE,0xF,0x8,0x4,0x7,0x1,0x2};

//permutation table
const unsigned char ptable[64] = {0, 16, 32, 48, 1, 17, 33, 49, 2, 18, 34, 50, 3, 19, 35, 51,  4, 20, 36, 52, 5, 21, 37, 53, 6, 22, 38, 54, 7, 23, 39, 55, 8, 24, 40, 56, 9, 25, 41, 57, 10, 26, 42, 58, 11, 27, 43, 59, 12, 28, 44, 60, 13, 29, 45, 61, 14, 30, 46, 62, 15, 31, 47, 63};

//reverse permutation table
const unsigned char rptable[64] = {0 , 4 , 8 , 12 , 16 , 20 , 24 , 28 , 32 , 36 , 40 , 44 , 48 , 52 , 56 , 60 , 1 , 5 , 9 , 13 , 17 , 21 , 25 , 29 , 33 , 37 , 41 , 45 , 49 , 53 , 57 , 61 , 2 , 6
, 10 , 14 , 18 , 22 , 26 , 30 , 34 , 38 , 42 , 46 , 50 , 54 , 58 , 62 , 3 , 7 ,11 , 15 , 19 , 23 , 27 , 31 , 35 , 39 , 43 , 47 , 51 , 55 , 59 , 63 };

//bias table [input mask][output mask]
const char biastable[16][16] = {
	{8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, -4, 0, -4, 0, 0, 0, 0, 0, -4, 0, 4},
	{0, 0, 2, 2, -2, -2, 0, 0, 2, -2, 0, 4, 0, 4, -2, 2},
	{0, 0, 2, 2, 2, -2, -4, 0, -2, 2, -4, 0, 0, 0, -2, -2},
	{0, 0, -2, 2, -2, -2, 0, 4, -2, -2, 0, -4, 0, 0, -2, 2},
	{0, 0, -2, 2, -2, 2, 0, 0, 2, 2, -4, 0, 4, 0, 2, 2},
	{0, 0, 0, -4, 0, 0, -4, 0, 0, -4, 0, 0, 4, 0, 0, 0},
	{0, 0, 0, 4, 4, 0, 0, 0, 0, -4, 0, 0, 0, 0, 4, 0},
	{0, 0, 2, -2, 0, 0, -2, 2, -2, 2, 0, 0, -2, 2, 4, 4},
	{0, 4, -2, -2, 0, 0, 2, -2, -2, -2, -4, 0, -2, 2, 0, 0},
	{0, 0, 4, 0, 2, 2, 2, -2, 0, 0, 0, -4, 2, 2, -2, 2},
	{0, -4, 0, 0, -2, -2, 2, -2, -4, 0, 0, 0, 2, 2, 2, -2},
	{0, 0, 0, 0, -2, -2, -2, -2, 4, 0, 0, -4, -2, 2, 2, -2},
	{0, 4, 4, 0, -2, -2, 2, 2, 0, 0, 0, 0, 2, -2, 2, -2},
	{0, 0, 2, 2, -4, 4, -2, -2, -2, -2, 0, 0, -2, -2, 0, 0},
	{0, 4, -2, 2, 0, 0, -2, -2, -2, 2, 4, 0, 2, 2, 0, 0}
};


void Linear_trails(uint64 * trail_log, unsigned char total_rounds, unsigned char rounds , unsigned char * active_sbox_last2 , unsigned char mask_log[32][16] , FILE* trailfile );





//permutation 16*4 = 64 bits = 8 bytes
uint64 permutation(uint64 in){
	unsigned char count = 0; 
	uint64  out = 0;
	uint64 mask = 0;

	for(count = 0 ; count < 64 ; count++){
		mask = 1;
		mask = mask << count;
		//printf("%d count %llx \n" , count, mask);

		if((in & mask) != 0){
		  
		   mask = 1;
		   out = out | (mask << (63 -  ptable[63-count]));
		}
	
	}


	return out;

};


//reverse permutation 16*4 = 64 bits = 8 bytes
uint64 reverse_permutation(uint64 out){
	unsigned char count = 0; 
	uint64  in = 0;
	uint64 mask = 0;

	for(count = 0 ; count < 64 ; count++){
		mask = 1;
		mask = mask << count;
		//printf("%d count %llx \n" , count, mask);

		if((out & mask) != 0){
		   //printf("%d route to %d \n", 63-count, rptable[63-count]);
		   mask = 1;
		   in = in | (mask << (63 - rptable[63-count]));
		}
	
	}


	return in;

};

//bias compute: only 2^ - bits  if -1 then the sign is -1

int bias(unsigned char round, uint64 * trails_log){
	int bias_bit = 0;
	int active = 0;
	int temp = 0;
	unsigned char sboxin[8];
	unsigned char sboxout[8];
	int sign = 1;
	int i,j;	
	for(i  = 0; i< round ; i++){
		UINT64_TO_ARRAY(trails_log[i],sboxin);
		UINT64_TO_ARRAY(reverse_permutation(trails_log[i+1]),sboxout);
		//active sbox
		for(j = 0 ; j< 8 ; j++){
		
			if((sboxin[j] & 0x0f)!=0)
			{
				active++;
				temp = biastable[(sboxin[j] & 0x0f)][(sboxout[j] & 0x0f)];
				if(temp < 0){
					temp = temp * -1;
					sign = sign * -1;
				}

				if(temp == 2)bias_bit +=3;
				if(temp == 4)bias_bit +=2;
			}

			if((sboxin[j] & 0xf0)!=0){
				active++;
				temp = biastable[((sboxin[j] >> 4) & 0x0f)][((sboxout[j] >> 4) & 0x0f)];
				if(temp < 0){
					temp = temp * -1;
					sign = sign * -1;
				}

				if(temp == 2)bias_bit +=3;
				if(temp == 4)bias_bit +=2;
			}
		}



	}
	
	bias_bit = (bias_bit +1 - active) * sign;

	return bias_bit;
}


//recursive all the possible combination
void combination(unsigned char * temparray, unsigned char sbox_count, uint64 * trail_log, const unsigned char total_rounds, unsigned char rounds , unsigned char * active_sbox_last2 , unsigned char mask_log[32][16] , FILE* trailfile ){
	unsigned char shift_bits = 0;
	uint64 tempmask = 0;
	unsigned char sboxmask = 0;
	
//one active sbox per round, changed by only looking at 1 2 4 8 
	if(sbox_count<15){	
		for(mask_log[rounds][sbox_count] = 1 ; mask_log[rounds][sbox_count] < 16 ; mask_log[rounds][sbox_count]=((mask_log[rounds][sbox_count])<<1) ){
	//if(sbox_count<15){	
	//	for(mask_log[rounds][sbox_count] = 1 ; mask_log[rounds][sbox_count] < 16 ; mask_log[rounds][sbox_count]++){
				if((sbox_count % 2) ==0)
					shift_bits = 4;
				else
					shift_bits = 0;
				sboxmask = (temparray[sbox_count/2]>>shift_bits) & 0x0f;
				if(rounds == total_rounds - 1){					
					if(active_sbox_last2[sbox_count] == 0 && sboxmask != 0){
						return;
					}
				}
				if(sboxmask == 0 ){
					mask_log[rounds][sbox_count] = 0;	
																	
				}
				else{
					if(biastable[sboxmask][mask_log[rounds][sbox_count]] == 0)continue;
				}
				combination(temparray,sbox_count+1,trail_log,total_rounds,rounds,active_sbox_last2 ,mask_log,trailfile );
	

			if(mask_log[rounds][sbox_count] == 0)break;

				
		}

	

	}// sbox count
	if(sbox_count==15)
	{ //sbox count ==15
		sboxmask = temparray[7] & 0x0f;	
		if(rounds == total_rounds - 1){					
			if(active_sbox_last2[sbox_count] == 0 && sboxmask != 0){
				return;
			}
		}
	
		if(sboxmask == 0 ){
			mask_log[rounds][15] = 0;	
			tempmask = SBOX_TO_UINT64(mask_log[rounds]);
			if(tempmask != 0){
				trail_log[rounds] = permutation(tempmask);
				//recursive next round
				Linear_trails(trail_log ,total_rounds, rounds+1 , active_sbox_last2 , mask_log ,trailfile );
				 
			}
		}
		else{
//one active sbox per round, changed by only looking at 1 2 4 8 	
			for(mask_log[rounds][15] = 1 ; mask_log[rounds][15] < 16 ; mask_log[rounds][15]=((mask_log[rounds][15])<<1) ){
				if(biastable[sboxmask][mask_log[rounds][15]] != 0){
					tempmask = SBOX_TO_UINT64(mask_log[rounds]);
					if(tempmask != 0){
						trail_log[rounds] = permutation(tempmask);
						//recursive next round
						Linear_trails(trail_log ,total_rounds, rounds+1 , active_sbox_last2 , mask_log ,trailfile);
					}
				}
			}
		}
					
	}
	return;
}



//recursive find every possible trails
void Linear_trails(uint64 * trail_log, const unsigned char total_rounds, unsigned char rounds , unsigned char * active_sbox_last2 , unsigned char mask_log[32][16] , FILE* trailfile ){
	//rounds should be 1 to total_rounds
	
	unsigned char temparray[8];
	unsigned char lastoutput[8];
	UINT64_TO_ARRAY(trail_log[rounds - 1] , temparray);
	//first several rounds normal way
	if(rounds<total_rounds ){
		/////////////////////////////
				
		//brute force all the possbile combination
		combination(temparray, 0,trail_log,total_rounds,rounds,active_sbox_last2 ,mask_log,trailfile );
	
		/////////////////////////////
		//output progress	
		if(rounds == 2){
			progress_number = progress_number +1;
			printf("Progress: %02.3lf%%\n", (double)(progress_number / active_sbox1*100.0));

		}
	
	
	} // rounds if

	//output linear trails to disk
	if(rounds == total_rounds){
		int count = 0;
		UINT64_TO_ARRAY(reverse_permutation(trail_log[total_rounds]) , lastoutput);
					for(count = 0; count < 8 ; count++){
						if(temparray[count] == 0 && lastoutput[count] != 0){
							break;
						}
						else{
							if((temparray[count] & 0xf0 ) == 0 && (lastoutput[count] &0xf0) != 0)
								break;
							else{
								if(biastable[(temparray[count] & 0xf0 )>>4][(lastoutput[count] &0xf0)>>4] == 0)
									break;
							}
							if((temparray[count] & 0x0f ) == 0 && (lastoutput[count] &0x0f) != 0)
								break;
							else{
								if(biastable[(temparray[count] & 0x0f )][(lastoutput[count] &0x0f)] == 0)
									break;
							}
						}


						if(temparray[count] != 0 && lastoutput[count] == 0){
							break;
						}
						else{
							if((temparray[count] & 0xf0 ) != 0 && (lastoutput[count] &0xf0) == 0)
								break;
							else{
								if(biastable[(temparray[count] & 0xf0 )>>4][(lastoutput[count] &0xf0)>>4] == 0)
									break;
							}
							if((temparray[count] & 0x0f ) != 0 && (lastoutput[count] &0x0f) == 0)
								break;
							else{
								if(biastable[(temparray[count] & 0x0f )][(lastoutput[count] &0x0f)] == 0)
									break;
							}
						}


					}
		
					if(count >=8){
						//printf("Found %ld linear trail(s)\n" , ++trail_number);
						trail_number++;
						int tempint = bias(total_rounds, trail_log);

						if(tempint > 0){
							tempint = -1 * tempint;
						}
							overall_bias = overall_bias + pow(2.0, (2.0*tempint));
							//for(count = 0; count <total_rounds ; count++)
							//fprintf(trailfile, "%016llX -- ", trail_log[count]); 
							//fprintf(trailfile, "%016llX  Bias: 2^%d\n", trail_log[total_rounds] , tempint ); 
							
							//fflush(trailfile);
							
						//long double TEMP_overall_bias = pow(overall_bias,0.5);
						//double overall_bits = log(TEMP_overall_bias);
						//fprintf(trailfile, "%llu linear trails found\n Overall bias: %.10Lf , which is 2^(%lf)\n",trail_number, TEMP_overall_bias, overall_bits/log(2.0));
						//printf("%ld linear trails found\n Overall bias: %.10Lf , which is 2^(%lf)\n",trail_number, TEMP_overall_bias, overall_bits/log(2.0));



					}
	
	}


	return;


}






int main(int   argc,   char   *argv[])
{
	FILE *trailfile;
	int rounds = 0;
	uint64 inputmask = 8;
	uint64 outputmask = 0;
	uint64 tempmask = 0;
	uint64 lastsbox_outmask = 0; 
	
	uint64 trail_log[33];
	unsigned char temparray[8];
	unsigned char mask_log[32][16];
	unsigned char active_sbox_last2[16];
	unsigned char outputarray[8];
	unsigned char tempbyte = 0;
	int i,j; // for count

while(1){
		trail_number = 0;
		overall_bias = 0;
		memset(mask_log,0xff,32*16);
		printf("How many rounds(2-32),otherwise quit: ");
		scanf("%d" , &rounds);
		if(rounds<2 || rounds>32)break;

		//open file each round
		if(argc ==1){
  		if((trailfile = fopen(TRAILFILE,"at+"))==NULL)
   		{
       		 	printf("cannot open file\n");
        		exit(0);
    		}
		}
		else if(argc ==2){
  		if((trailfile = fopen(argv[1],"at+"))==NULL)
   		{
       		 	printf("cannot open file\n");
        		exit(0);
    		}
		} else {
			printf("too many commandline arguments\n");
			exit(0);
		}
		fprintf(trailfile, "Number of rounds: %d\n", rounds);
		fflush(trailfile);
		printf("Input mask:  ");
		scanf("%llX" , &inputmask);
		//active sbox of first round
		
		progress_number = 1;
		for(i = 0; i < 16 ;i++){
			if((tempbyte =((inputmask >>(i*4)) & 0x0f))!=0){
				active_sbox1=0;
				for(j = 0; j<16 ;j++){
					if(biastable[tempbyte][j] != 0){
						active_sbox1 = active_sbox1+1;
					}
				}
				progress_number = active_sbox1 * progress_number;
			}			
				
		}
		active_sbox1 = progress_number;
		progress_number = 0;
		fprintf(trailfile, "Input mask:  %016llX\n", inputmask);
		fflush(trailfile);
		printf("Output mask: ");
		scanf("%llX" , &outputmask);

		fprintf(trailfile, "Output mask: %016llX\n", outputmask);
		fflush(trailfile);
		trail_log[0] = inputmask;
		trail_log[rounds] = outputmask;
		lastsbox_outmask = reverse_permutation(outputmask);
		UINT64_TO_ARRAY(lastsbox_outmask, outputarray);
		for(i = 0; i < 8 ; i++){
			if(((outputarray[i] >> 4) & 0x0f) != 0 ){
			
				temparray[i] =( 0x0f << 4);
			}
			else{
				temparray[i] =( 0x00 << 4);
		
			}
			if((outputarray[i] & 0x0f) != 0 ){
			
				temparray[i] = (temparray[i] | 0x0f); 
			}

		}

		tempmask = reverse_permutation(ARRAY_TO_UINT64(temparray));
		UINT64_TO_ARRAY(tempmask, temparray); 
		// set the last 2 round active sbox
		//printf( "\nlast round mask: %llx\n", tempmask);
		for(i = 0; i < 8 ; i++){
			if(((temparray[i] >> 4) & 0x0f) != 0 ){
				active_sbox_last2[2*i] = 1;
				
			}
			else{
				
				active_sbox_last2[2*i] = 0;
			}
			if((temparray[i] & 0x0f) != 0 ){
				active_sbox_last2[2*i+1] = 1;
			
			}
			else{
				active_sbox_last2[2*i+1] = 0;
			}
		}
		//for(i = 0; i < 16 ; i++){
			
		//	printf("Possible active sbox of second last round[%d] = %x \n", i , active_sbox_last2[i]);
		//}


		
		Linear_trails(trail_log ,rounds, 1 , active_sbox_last2 , mask_log ,trailfile );
		

		//if(overall_bias< 0)overall_bias = -1.0 * overall_bias;
		overall_bias = pow(overall_bias,0.5);
		double overall_bits = log(overall_bias);
		fprintf(trailfile, "Total %llu linear trails found\n Total overall bias: %.10Lf , which is 2^(%lf)\nQED\n",trail_number, overall_bias, overall_bits/log(2.0));
		fflush(trailfile);
	        fclose(trailfile);




} //main while



	return 0;

}

