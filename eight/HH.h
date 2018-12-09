#include <iostream>
#include <cmath>
#include <vector>
#include <fstream>
#include <sstream>

class Channel {
    public: 
        virtual float I () {return 0.0;}
        float E;
        std::vector<std::vector<float> > solution;
        
        Channel () {E = -49.387;}
        Channel (float E0) {E = E0;}
};

class Sodium: public Channel {
    public:
        Sodium (float _e = 55) : Channel (_e) {};
        float alpha_m(float v) { return 0.1 * ( v + 40.0 ) / (1.0 - std::exp( -(v + 40.0 ) / 10.0 ) ); };
        float alpha_h(float v) { return 0.07 * std::exp( -(v + 65.0 ) / 20.0 ); };
        float beta_m(float v) { return 4.0 * exp( -(v + 65.0) / 18.0 ); };
        float beta_h(float v) { return 1.0 / (1.0 + std::exp( -(v + 35.0 ) / 10.0 ) ); };
        float I(float v, float t, float m, float h) { return 120.0 * std::powf(m, 3.0) * h * (v - E); };

        float m_t(float,float);
        float h_t(float, float);
        float Current(float, float);
        float Conductance(float, float);
        std::vector<float> ConductanceSoln(float, float, float);
        std::vector<std::vector<float> > solution(float, float, float);
};

float Sodium::m_t(float t, float v) {

    float m_Inf;
    float tau_m;
    float m;
    float m_0 = 0.0;

    m_Inf = alpha_m(v) / (alpha_m(v)+beta_m(v));

    tau_m = 1 / (alpha_m(v) + beta_m(v));

    m = m_Inf - (m_Inf - m_0) * (std::expf(-t/tau_m));
    return m;    
}

float Sodium::h_t(float t, float v) {

    float hInf;
    float tau_h;
    float h;
    float h_0 = 1.0;

    hInf = alpha_h(v) / (alpha_h(v)+beta_h(v));
    
    tau_h = 1 / (alpha_h(v)+beta_h(v));

    h = hInf - (hInf - h_0) * (std::expf(-t/tau_h));
    return h;
}

float Sodium::Current(float t, float v) {
    float m = m_t(t,v);
    float h = h_t(t,v);
    float I;
    I = std::powf(m,3.0) * h * (v-E);
    return I;
}

float Sodium::Conductance(float t, float v) {
    float g = Current(t,v) / v;
    return g;
}

std::vector<float> Sodium::ConductanceSoln(float dt, float tFinal, float v) {
    int size = tFinal / dt;
    std::vector<float> g(size);
    for (int i = 0; i < size; i++) {
        g[i] = Conductance(dt * i, v);
    }
    return g;
}

// HH model Solution -----------------------------------------------------------------------
std::vector<std::vector<float> > Sodium::solution(float dt, float tFinal, float V_0) {
    int size = tFinal / dt;
    std::vector<float> I_Na(size), ms(size), hs(size);
    std::vector<float> t(size);

    // for finding m's and h's
    float am = alpha_m(V_0), bm = beta_m(V_0);
    float ah = alpha_h(V_0), bh = beta_h(V_0);
    float m0 = 0.0, h0 = 1.0;

    float mInf = ( am / ( am + bm ) );
    float tau_m = 1 / ( am + bm );
    float hInf = ( ah / ( ah + bh ) );
    float tau_h = 1 / ( ah + bh );
    
    float t_n = 0.0;
    float I_n = 0.0;
    float m, h;
    float m_ss = .0529;
    float h_ss = .5961;
    for (int i = 0; i < size; i++) {
        
        t_n = i * dt;
        m = mInf - ( mInf - m_ss ) * std::exp( (-t_n / tau_m ) );
        h = hInf - ( hInf - h_ss ) * std::exp( (-t_n / tau_h ) );

        I_n = I(V_0, t_n, m, h);
        t[i] = t_n;
        ms[i] = m;
        hs[i] = h;
        I_Na[i] = I_n / (V_0);
    }

    // print out for testing purposes
    std::ofstream output;
    output.open("hh1.txt");
    output << "t, m, h, I\n";
    for (int i = 0; i < size; i++) {
        output << t[i] << "," << ms[i] << "," << hs[i] << "," << I_Na[i] << "\n";
    }
    output.close();

    return {t, ms, hs, I_Na};
    
}
