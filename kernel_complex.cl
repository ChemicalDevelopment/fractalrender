#ifdef cl_khr_fp64

typedef double2 complex_t;

inline double re(complex_t a){
     return a.x;
}
inline double im(complex_t a){
     return a.y;
}
inline double ang(complex_t a){
    return atan2(a.y, a.x);
}

inline double cnorm_sqr(complex_t a){
    return (a.x*a.x + a.y*a.y);
}

inline double cnorm(complex_t a){
    return sqrt(a.x*a.x + a.y*a.y);
}

inline complex_t cscale(complex_t a, double b) {
    return (complex_t)(a.x * b, a.y * b);
}

inline complex_t cmul(complex_t a, complex_t b){
    return (complex_t)( a.x*b.x - a.y*b.y, a.x*b.y + a.y*b.x);
}


inline complex_t cdiv(complex_t a, complex_t b){
    double _nrm = cnorm(b);
    return (complex_t)((a.x*b.x + a.y*b.y)/_nrm, (a.y*b.x - a.x*b.y)/_nrm);
}

inline complex_t csqr(complex_t a) {
    return (complex_t)(a.x*a.x - a.y*a.y, 2 * a.x*a.y);
}

inline complex_t cexp(complex_t a) {
    return (complex_t)(cos(a.y) * exp(a.x), sin(a.y) * exp(a.x));
}

inline complex_t clog(complex_t a) {
    return (complex_t)(log(cnorm(a)), ang(a));
}

inline complex_t cpow(complex_t a, complex_t b) {
    return cexp(cmul(clog(a), b));
}
inline complex_t cpowi(complex_t a, int b) {
    complex_t pa = (complex_t)(1, 0);
    while (b > 0) {
        pa = cmul(pa, a);   
        b--;
    }
    return pa;
}


__kernel void mand(__global uchar * color_pattern, int color_pattern_length, double color_pattern_scale, double color_pattern_disp, int do_simple_color, __global uchar * color_output, int width, int height, int max_iter, double center_x, double center_y, double zoom, double time)
{
    int px = get_global_id(0), py = get_global_id(1);

    complex_t c = (complex_t)(center_x + (2 * px - width) / (zoom * width), center_y + (height - 2 * py) / (zoom * width));
    complex_t z = c;

    int col_dest = 3 * (py * width + px);

    double er = 16.0;
    double er2 = er * er;

    double tmp;

    int ci;
    for (ci = 1; ci <= max_iter && cnorm_sqr(z) <= er2; ++ci) {
        //z = cpow(z, (complex_t)(4, 0)) + c;
        //z = cpow((complex_t)(2.71828, 0), z) + c;
        //z = cpow(z, (complex_t)(2 + time / 4.0, 0)) + c;
        z = csqr(z) + c;
    }

    if (do_simple_color) {
        int color_off;
        if (ci > max_iter) {
            color_off = 0;
        } else {
            color_off = 3*((int)floor(ci * color_pattern_scale + color_pattern_disp) % color_pattern_length);
        }

        color_output[col_dest + 0] = color_pattern[color_off + 0];
        color_output[col_dest + 1] = color_pattern[color_off + 1];
        color_output[col_dest + 2] = color_pattern[color_off + 2];
                
    } else {
        
        double zn = cnorm(z);
        double hue;
        if (zn <= er2) {
            hue = 0;
        } else {
            hue = ci + 1.0 - log(fabs(zn)) / log(er2);
        }


        hue = hue * color_pattern_scale + color_pattern_disp;
        
        hue = fmod(fmod(hue, color_pattern_length) + color_pattern_length, color_pattern_length);

        tmp = hue - floor(hue);
        int color_off0 = 3 * ((int)floor(hue) % color_pattern_length);
        int color_off1;
        if (color_off0 >= 3 *(color_pattern_length - 1)) {
            color_off1 = 0;
        } else {
            color_off1 = color_off0 + 3;
        }

        color_output[col_dest + 0] = ((uchar)floor(tmp*color_pattern[color_off1 + 0]+(1-tmp)*color_pattern[color_off0 + 0]));
        color_output[col_dest + 1] = ((uchar)floor(tmp*color_pattern[color_off1 + 1]+(1-tmp)*color_pattern[color_off0 + 1]));
        color_output[col_dest + 2] = ((uchar)floor(tmp*color_pattern[color_off1 + 2]+(1-tmp)*color_pattern[color_off0 + 2]));
        
    }
}

#endif



typedef float2 fcomplex_t;

inline float fre(fcomplex_t a){
     return a.x;
}
inline float fim(fcomplex_t a){
     return a.y;
}
inline float fang(fcomplex_t a){
    return atan2(a.y, a.x);
}

inline float fcnorm_sqr(fcomplex_t a){
    return (a.x*a.x + a.y*a.y);
}

inline float fcnorm(fcomplex_t a){
    return sqrt(a.x*a.x + a.y*a.y);
}

inline fcomplex_t fcscale(fcomplex_t a, float b) {
    return (fcomplex_t)(a.x * b, a.y * b);
}

inline fcomplex_t fcmul(fcomplex_t a, fcomplex_t b){
    return (fcomplex_t)( a.x*b.x - a.y*b.y, a.x*b.y + a.y*b.x);
}


inline fcomplex_t fcdiv(fcomplex_t a, fcomplex_t b){
    float _nrm = fcnorm(b);
    return (fcomplex_t)((a.x*b.x + a.y*b.y)/_nrm, (a.y*b.x - a.x*b.y)/_nrm);
}

inline fcomplex_t fcsqr(fcomplex_t a) {
    return (fcomplex_t)(a.x*a.x - a.y*a.y, 2 * a.x*a.y);
}

inline fcomplex_t fcexp(fcomplex_t a) {
    return (fcomplex_t)(cos(a.y) * exp(a.x), sin(a.y) * exp(a.x));
}

inline fcomplex_t fclog(fcomplex_t a) {
    return (fcomplex_t)(log(fcnorm(a)), fang(a));
}

inline fcomplex_t fcpow(fcomplex_t a, fcomplex_t b) {
    return fcexp(fcmul(fclog(a), b));
}
inline fcomplex_t fcpowi(fcomplex_t a, int b) {
    fcomplex_t pa = (fcomplex_t)(1, 0);
    while (b > 0) {
        pa = fcmul(pa, a);   
        b--;
    }
    return pa;
}


__kernel void mand_32(__global uchar * color_pattern, int color_pattern_length, float color_pattern_scale, float color_pattern_disp, int do_simple_color, __global uchar * color_output, int width, int height, int max_iter, float center_x, float center_y, float zoom, float time)
{
    int px = get_global_id(0), py = get_global_id(1);

    fcomplex_t c = (fcomplex_t)(center_x + (2 * px - width) / (zoom * width), center_y + (height - 2 * py) / (zoom * width));
    fcomplex_t z = c;

    int col_dest = 3 * (py * width + px);

    float er = 16.0f;
    float er2 = er * er;

    float tmp;

    int ci;
    for (ci = 1; ci <= max_iter && fcnorm_sqr(z) <= er2; ++ci) {
        //z = cpow(z, (fcomplex_t)(4, 0)) + c;
        //z = cpow((fcomplex_t)(2.71828, 0), z) + c;
        //z = cpow(z, (fcomplex_t)(2 + time / 4.0, 0)) + c;
        z = fcsqr(z) + c;
    }

    if (do_simple_color) {
        int color_off;
        if (ci > max_iter) {
            color_off = 0;
        } else {
            color_off = 3*((int)floor(ci * color_pattern_scale + color_pattern_disp) % color_pattern_length);
        }

        color_output[col_dest + 0] = color_pattern[color_off + 0];
        color_output[col_dest + 1] = color_pattern[color_off + 1];
        color_output[col_dest + 2] = color_pattern[color_off + 2];
                
    } else {
        
        float zn = fcnorm_sqr(z);
        float hue;
        if (zn <= er2) {
            hue = 0;
        } else {
            hue = ci + 1.0f - log(fabs(zn)) / log(er2);
        }


        hue = hue * color_pattern_scale + color_pattern_disp;
        
        hue = fmod(fmod(hue, color_pattern_length) + color_pattern_length, color_pattern_length);

        tmp = hue - floor(hue);
        int color_off0 = 3 * ((int)floor(hue) % color_pattern_length);
        int color_off1;
        if (color_off0 >= 3 *(color_pattern_length - 1)) {
            color_off1 = 0;
        } else {
            color_off1 = color_off0 + 3;
        }

        color_output[col_dest + 0] = ((uchar)floor(tmp*color_pattern[color_off1 + 0]+(1-tmp)*color_pattern[color_off0 + 0]));
        color_output[col_dest + 1] = ((uchar)floor(tmp*color_pattern[color_off1 + 1]+(1-tmp)*color_pattern[color_off0 + 1]));
        color_output[col_dest + 2] = ((uchar)floor(tmp*color_pattern[color_off1 + 2]+(1-tmp)*color_pattern[color_off0 + 2]));
        
    }
}



