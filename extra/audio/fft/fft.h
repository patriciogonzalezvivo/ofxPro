
#ifndef _FFT
#define _FFT

#ifndef M_PI
#define	M_PI		3.14159265358979323846
#endif


class fft {
public:
    
    fft();
    ~fft();
		
    void powerSpectrum(int start, int half, float *data, int windowSize,float *magnitude,float *phase, float *power, float *avg_power);
    void inversePowerSpectrum(int start, int half, int windowSize, float *finalOut,float *magnitude,float *phase);
    void processLogXScale(const float * data,unsigned int insize, double fMax,
						  float * avout, float * pkout, unsigned int outsize,
						  float f1, float f2);	

    float avout[15];
    float pkout[15];		
};
	
	
#endif	
