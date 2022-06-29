extern "C" void opentree_(int *_npom, char* filename) ;
extern "C" void treehead_(int *_npom, int *_iversn, int *_laproj, int *_maproj, int *_latarg, int *_matarg, float *_engy, int *_nfull, int *_nfreeze);
extern "C" void filltree_(int *_npom, int *_np, float *_bim, int *_id, int *_ist, int *_ity, int *_ior, int *_jor, float *_zus, float *_px, float *_py, float *_pz,  float *_e, float *_x, float *_y, float *_z, float *_t, int *_iret) ;
extern "C" void closetree_(int *_npom, char* filename) ;

